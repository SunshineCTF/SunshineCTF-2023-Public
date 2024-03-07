#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include "timers.h"
#include "common.h"
#include "slist.h"
#include "locked_io.h"


#define USEC_PER_SEC 1000000
#define NSEC_PER_SEC 1000000000


struct timer {
	/* 00 + 08 */ void *cb_arg;
	/* 08 + 08 */ void (*callback)(void *, void *, void *);
	/* 10 + 08 */ struct slist_node node;
	/* 18 + 10 */ struct timespec expiry;
}; /* sizeof = 0x28, malloc size: 0x30 */

static SLIST_HEAD(g_timer_list);
static pthread_mutex_t g_timer_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_timer_cond = PTHREAD_COND_INITIALIZER;


/*
 * Taken from https://github.com/solemnwarning/timespec/blob/master/timespec.c
 * Translated to American English :)
 */
static struct timespec timespec_normalize(struct timespec ts)
{
	while(ts.tv_nsec >= NSEC_PER_SEC)
	{
		++(ts.tv_sec);
		ts.tv_nsec -= NSEC_PER_SEC;
	}
	
	while(ts.tv_nsec <= -NSEC_PER_SEC)
	{
		--(ts.tv_sec);
		ts.tv_nsec += NSEC_PER_SEC;
	}
	
	if(ts.tv_nsec < 0)
	{
		/* Negative nanoseconds isn't valid according to POSIX.
		 * Decrement tv_sec and roll tv_nsec over.
		*/
		
		--(ts.tv_sec);
		ts.tv_nsec = (NSEC_PER_SEC + ts.tv_nsec);
	}
	
	return ts;
}


static struct timespec timespec_add(
	struct timespec ts1,
	struct timespec ts2)
{
	/* Normalize inputs to prevent tv_nsec rollover if whole-second values
	 * are packed in it.
	*/
	ts1 = timespec_normalize(ts1);
	ts2 = timespec_normalize(ts2);
	
	ts1.tv_sec  += ts2.tv_sec;
	ts1.tv_nsec += ts2.tv_nsec;
	
	return timespec_normalize(ts1);
}


static int timespec_cmp(struct timespec ts1, struct timespec ts2)
{
	ts1 = timespec_normalize(ts1);
	ts2 = timespec_normalize(ts2);
	
	if(ts1.tv_sec == ts2.tv_sec && ts1.tv_nsec == ts2.tv_nsec)
	{
		return 0;
	}
	else if((ts1.tv_sec > ts2.tv_sec)
		  || (ts1.tv_sec == ts2.tv_sec && ts1.tv_nsec > ts2.tv_nsec))
	{
		return 1;
	}
	else {
		return -1;
	}
}


// static double timespec_to_double(struct timespec ts)
// {
// 	return ((double)(ts.tv_sec) + ((double)(ts.tv_nsec) / NSEC_PER_SEC));
// }


static void process_timers_locked(struct timespec *next_expiry)
{
	int err;
	struct timespec now;
	struct slist_node **pcur, **pnext;
	bool saw_next = false;
	
	if (clock_gettime(CLOCK_REALTIME, &now)) {
		locked_perror("clock_gettime");
		exit(EXIT_FAILURE);
	}
	
	slist_for_each_safe(pcur, pnext, &g_timer_list) {
		struct timer *cur = slist_entry(*pcur, typeof(*cur), node);
		if (timespec_cmp(now, cur->expiry) < 0) {
			/*
			 * This timer is the next one that will trigger,
			 * so sleep until it's ready.
			 */
			*next_expiry = cur->expiry;
			break;
		}
		
		slist_del(pcur, &g_timer_list);
		/* Hack to make it easy to replace with execve() */
		cur->callback(cur->cb_arg, NULL, NULL);
		// locked_printf("DEBUG: free timer %p\n", cur);
		free(cur);
	}
}


static void *timer_thread(void *ignored)
{
	int err;
	
	pthread_mutex_lock(&g_timer_mutex);
	for (;;) {
		struct timespec next_expiry = {};
		process_timers_locked(&next_expiry);
		
		if (next_expiry.tv_sec) {
			/*
			 * Sleep until the first of:
			 *  - Next timer's expiry time
			 *  - g_timer_cond is triggered (an earlier timer was added)
			 */
			err = pthread_cond_timedwait(&g_timer_cond, &g_timer_mutex, &next_expiry);
			if (err && err != ETIMEDOUT) {
				locked_printerr("pthread_cond_timedwait", err);
				
				/* Try again in 10ms */
				usleep(10 * 1000);
			}
		}
		else {
			err = pthread_cond_wait(&g_timer_cond, &g_timer_mutex);
			if (err) {
				locked_printerr("pthread_cond_wait", err);
				
				/* Try again in 10ms */
				usleep(10 * 1000);
			}
		}
	}
}


static void start_timer_thread(void)
{
	pthread_t thread;
	int err = pthread_create(&thread, NULL, timer_thread, NULL);
	if (err) {
		locked_printerr("pthread_create", err);
		exit(err);
	}
	
	pthread_detach(thread);
}


void init_timers(void)
{
	static pthread_once_t thread_started = PTHREAD_ONCE_INIT;
	int err = pthread_once(&thread_started, start_timer_thread);
	if (err) {
		locked_printerr("pthread_once", err);
		exit(err);
	}
}


/* Set up a callback to run after some delay in microseconds */
void Timer_start(
	uint64_t duration_us,
	void (*callback)(void *, void *, void *),
	void *arg)
{
	init_timers();
	
	struct timespec now;
	if (clock_gettime(CLOCK_REALTIME, &now)) {
		locked_perror("clock_gettime");
		exit(EXIT_FAILURE);
	}
	
	struct timespec duration = {
		.tv_sec = duration_us / USEC_PER_SEC,
		.tv_nsec = (duration_us % USEC_PER_SEC) * 1000,
	};
	
	struct timer* timer = malloc(sizeof(*timer));
	if (!timer) {
		locked_perror("malloc");
		exit(EXIT_FAILURE);
	}
	
	// locked_printf("DEBUG: timer at %p\n", timer);
	timer->expiry = timespec_add(now, duration);
	timer->callback = callback;
	timer->cb_arg = arg;
	
	pthread_mutex_lock(&g_timer_mutex);
	
	/* Look for list entry with a later expiry time */
	bool added = false;
	struct slist_node **pcur, **pnext;
	slist_for_each_safe(pcur, pnext, &g_timer_list) {
		struct timer *cur = slist_entry(*pcur, typeof(*cur), node);
		// locked_printf(
		// 	"DEBUG: cmp(%.2f, %.2f)\n"
		// 	"DEBUG: left=%p, right=%p\n",
		// 	timespec_to_double(timer->expiry),
		// 	timespec_to_double(cur->expiry),
		// 	timer, cur
		// );
		
		/* Does new timer expire before cur timer? */
		if (timespec_cmp(timer->expiry, cur->expiry) < 0) {
			/* Insert new timer before this entry */
			__slist_insert(&timer->node, pcur);
			added = true;
			break;
		}
	}
	
	if (!added) {
		/*
		 * Nothing in the list with a later time than the new timer,
		 * so add at the tail.
		 */
		// locked_printf("DEBUG: Adding %p at the tail...\n", timer);
		slist_add_tail(&timer->node, &g_timer_list);
	}
	
	/* If added to front of list, signal condition variable */
	if (timer == slist_first_entry(&g_timer_list, struct timer, node)) {
		pthread_cond_signal(&g_timer_cond);
	}
	
	// locked_printf("DEBUG: timer added, next=%p\n", timer->node.next);
	
	pthread_mutex_unlock(&g_timer_mutex);
}
