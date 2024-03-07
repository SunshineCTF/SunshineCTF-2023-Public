#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

/* Taken from Linux kernel with modifications */

/* Not using RCU, so these can be dumb */
#define READ_ONCE(x) (x)
#define WRITE_ONCE(x, val) (x = (val))

#define container_of(ptr, type, member) ({ \
	const typeof(((type*)0)->member)* __mptr = (ptr); \
	(type*)((char*)__mptr - offsetof(type, member)); \
})

#endif /* COMMON_H */
