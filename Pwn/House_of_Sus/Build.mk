# House of Sus: built from source by PwnableHarness on Ubuntu 18.04 (glibc 2.27,
# which the intended solution requires). Because PH
# compiles the binary, it automatically links in the stdio_unbuffer.c constructor
# (an __attribute__((constructor)) that disables stdio buffering before main), so
# no LD_PRELOAD/--inject or pwnablepreload.so is needed, and the binary uses the
# standard interpreter and the base image's glibc 2.27 directly (no shipped ld/
# libc or custom Dockerfile).
UBUNTU_VERSION := 18.04

TARGET := house_of_sus
SRCS := house_of_sus.c

# Match the original challenge binary's mitigations.
NX := 1
CANARY := 1
PIE := 0
RELRO := partial

PUBLISH_BUILD := $(TARGET)
# Publish the exact ld/libc the challenge runs against (copied from the image).
PUBLISH_LIBC := libc.so.6
PUBLISH_LD := ld-linux-x86-64.so.2

DOCKER_IMAGE := sun23-house-of-sus
DOCKER_PORTS := 23001
DOCKER_TIMELIMIT := 30

# `pwnmake check`: run the exploit and verify it prints the flag
$(call ctf_check,$(DIR),$(DOCKER_PORTS),python3 house_of_sus_exp.py)
