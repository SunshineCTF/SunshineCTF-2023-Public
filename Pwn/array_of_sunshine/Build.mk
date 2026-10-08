UBUNTU_VERSION := 22.04

PUBLISH := sunshine
DOCKER_IMAGE := sun23-array-of-sunshine
DOCKER_CHALLENGE_NAME := sunshine
DOCKER_CHALLENGE_PATH := $(DIR)/sunshine
DOCKER_PORTS := 23003
DOCKER_TIMELIMIT := 30

# `pwnmake check`: run the exploit and verify it prints the flag.
$(call ctf_check,$(DIR),$(DOCKER_PORTS),python3 pwn-sunshine.py)
