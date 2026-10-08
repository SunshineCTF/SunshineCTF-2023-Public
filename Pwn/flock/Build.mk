UBUNTU_VERSION := 22.04

PUBLISH := flock
DOCKER_IMAGE := sun23-flock-of-seagulls
DOCKER_CHALLENGE_NAME := flock
DOCKER_CHALLENGE_PATH := $(DIR)/flock
DOCKER_PORTS := 23002
DOCKER_TIMELIMIT := 30

# `pwnmake check`: run the exploit and verify it prints the flag.
$(call ctf_check,$(DIR),$(DOCKER_PORTS),python3 pwn-flock.py)
