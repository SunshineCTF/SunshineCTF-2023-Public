# House of Sus
A fun pwn challenge

## Compilation
`gcc -O0 -w -g -no-pie house_of_sus.c -o house_of_sus`
 - There's a lot of compiler warnings (because security is a thing I guess)

## Deployment

Built from source and deployed with stock PwnableHarness (`pwnmake`). It must run
on Ubuntu 18.04 (glibc 2.27), selected via `UBUNTU_VERSION := 18.04` in `Build.mk`.
Because PwnableHarness compiles the binary, it links in the `stdio_unbuffer.c`
constructor automatically (so stdout is unbuffered over the socket with no
LD_PRELOAD/`--inject`), and the binary uses the standard interpreter and the base
image's glibc directly (no shipped ld/libc or custom Dockerfile needed). Run
`pwnmake docker-start` to build and run it, and `pwnmake publish` to publish the
player files (binary + the exact libc/ld).

## Thanks
* Thanks to my friend knittingirl for developing the solution script
* Thanks to my friend Milo Gilad for helping me come up with the theme and idea

See [writeup.md](writeup.md) for the solution.
