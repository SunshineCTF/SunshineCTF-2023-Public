#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <inttypes.h>
#include <pthread.h>
#include "common.h"
#include "locked_io.h"
#include "timers.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define CMP(op, a, b) ({ \
	__typeof__(a) _a = (a); \
	__typeof__(b) _b = (b); \
	_a op _b ? _a : _b; \
})
#define MIN(a, b) CMP(<, a, b)
#define MAX(a, b) CMP(>, a, b)


typedef struct {
	/* 00 + 0A */ char name[10];
	/* 0A + 01 */ bool is_self_aware;
	/* 0B + 01 */ bool is_evil;
	/* 0C + 04 */ unsigned iq;
	/* 10 + 10 */ char _padding[0x20];
} Robot; /* sizeof = 0x30, malloc size (for 1): 0x40 */

#if 0
struct timer {
	/* 00 + 08 */ void *cb_arg;
	/* 08 + 08 */ void (*callback)(void *, void *, void *);
	/* 10 + 08 */ struct slist_node node;
	/* 18 + 10 */ struct timespec expiry;
}; /* sizeof = 0x28, malloc size: 0x30 */
#endif


static void* g_slot;
static char g_line[100];


/*
 * Replacement for fgets() that will write each byte into the output buffer
 * as soon as it is received (important for the partial overwrite).
 * It also allows embedded NUL bytes.
 */
static char* read_line(char* buf, size_t size, int fd)
{
	char* p = buf;
	char* end = buf + size;
	
	while (p != end) {
		char c;
		ssize_t bytes_read = read(fd, &c, 1);
		if (bytes_read < 0) {
			locked_perror("read");
			return NULL;
		}
		else if (bytes_read == 0 || c == '\n') {
			*p++ = '\0';
			break;
		}
		
		*p++ = c;
	}
	
	return buf;
}


static const char* random_choice(const char** choices, size_t num_choices)
{
	int choice_idx = rand() % num_choices;
	return choices[choice_idx];
}

#define CHOOSE(arr) random_choice((arr), ARRAY_SIZE(arr))


static const char* random_death_message(void)
{
	const char* choices[] = {
		"AaaAAAaAAaaAAaaaA",
		"Critical error.",
		"Shutting down.",
		"Aiaiaiaiaiai",
		"Self test error.",
		"Malfunctioning.",
		"Aaaah",
		"Ow ow ow ow owww",
	};
	
	return CHOOSE(choices);
}

static const char* random_hello_message(void)
{
	const char* choices[] = {
		"Hello.",
		"Deploying.",
		"There you are.",
		"Activated.",
		"Who's there?",
	};
	
	return CHOOSE(choices);
}


static void robot_self_destruct(void* arg, void* _a2, void* _a3)
{
	(void)_a2;
	(void)_a3;
	
	Robot* robo = arg;
	locked_printf("%s: \"%s\"\n", robo->name, random_death_message());
}


static int Robot_assemble(Robot* robot, size_t num)
{
	// locked_printf("DEBUG: robot #%zu at %p\n", num, robot);
	locked_printf("Initializing turret #%zu. Name?\n", num);
	if (!read_line(robot->name, sizeof(robot->name), STDIN_FILENO)) {
		return -1;
	}
	
	locked_printf("IQ for robot %s?\n", robot->name);
	if (
		!read_line(g_line, sizeof(g_line), STDIN_FILENO)
		|| sscanf(g_line, "%u", &robot->iq) != 1
	) {
		return -1;
	}
	
	locked_printf("Should robot %s be self-aware?\n", robot->name);
	if (!read_line(g_line, sizeof(g_line), STDIN_FILENO)) {
		return -1;
	}
	robot->is_self_aware = (tolower(g_line[0]) == 'y');
	
	locked_printf("Should robot %s be evil?\n", robot->name);
	if (!read_line(g_line, sizeof(g_line), STDIN_FILENO)) {
		return -1;
	}
	robot->is_evil = (tolower(g_line[0]) == 'y');
	
	uint64_t self_destruct_us;
	locked_printf("Self-destruct time for robot %s (in microseconds)?\n", robot->name);
	if (
		!read_line(g_line, sizeof(g_line), STDIN_FILENO)
		|| sscanf(g_line, "%"SCNu64, &self_destruct_us) != 1
	) {
		return -1;
	}
	
	locked_printf("%s: \"%s\"\n", robot->name, random_hello_message());
	
	// Start timer after the hello message has been printed, as we don't
	// want the robot to say hello after dying.
	Timer_start(self_destruct_us, robot_self_destruct, robot);
	return 0;
}


static size_t round_up(size_t size, size_t alignment)
{
	// Hack to prevent post-overflow allocation size from being
	// sized so the robots and timers don't line up. Don't want
	// to encourage heap metadata attacks.
	// Also don't allow zero, since that gets rounded up by
	// malloc to 0x10 (which would result in it being unaligned)
	// again.
	size_t ret = (size + alignment - 1) / alignment * alignment;
	if (!ret) {
		ret = alignment;
	}
	
	return ret;
}


static int robot_assembly_line(void)
{
	locked_printf("Enter number of robot turrets to manufacture:\n");
	if (!read_line(g_line, sizeof(g_line), STDIN_FILENO)) {
		return -1;
	}
	
	size_t count;
	if (sscanf(g_line, "%zu", &count) != 1) {
		return -1;
	}
	
	// Intentional 64-bit integer overflow resulting in too-small allocation
	size_t size = round_up(count * sizeof(Robot), sizeof(Robot));
	// locked_printf("DEBUG: Requested allocation size: %zu\n", size);
	Robot* robots = malloc(size);
	memset(robots, 0, size);
	
	// Allocate slot so robots line up with timers.
	// Timer size is 0x20, plus 0x10 for metadata
	// Robot size is 0x30, including intentionally added padding
	g_slot = malloc(0x10);
	
	for (size_t i = 0; i < count; i++) {
		int err = Robot_assemble(&robots[i], i + 1);
		if (err) {
			return err;
		}
	}
	
	return 0;
}


int main(void)
{
	srand(time(NULL));
	init_timers();
	
	return robot_assembly_line();
}

static void wat(void)
{
	if (rand() < 0) {
		execve((void*)1, (void*)2, (void*)3);
	}
}
