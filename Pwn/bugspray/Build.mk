UBUNTU_VERSION := 22.04

PUBLISH := bugspray
DOCKER_IMAGE := sun23-bugspray
DOCKER_CHALLENGE_NAME := bugspray
DOCKER_CHALLENGE_PATH := $(DIR)/bugspray
DOCKER_PORTS := 23004
DOCKER_TIMELIMIT := 30

# `pwnmake check`: run the exploit and verify it prints the flag.
$(call ctf_check,$(DIR),$(DOCKER_PORTS),python3 pwn-bugspray.py)
