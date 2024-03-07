#ifndef TIMERS_H
#define TIMERS_H

#include "common.h"

/* Set up a callback function to run after some delay in microseconds */
void Timer_start(
	uint64_t duration_us,
	void (*callback)(void *, void *, void *),
	void *arg);

/* Spawn the timer thread */
void init_timers(void);

#endif /* TIMERS_H */
