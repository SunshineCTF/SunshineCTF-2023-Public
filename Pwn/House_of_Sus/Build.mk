PUBLISH := house-of-sus ld-linux-x86-64.so.2 libc.so.6
DOCKER_IMAGE := house-of-sus
DOCKER_CHALLENGE_NAME := house_of_sus
DOCKER_CHALLENGE_PATH := $(DIR)/house_of_sus
DOCKER_PORTS := 23001
DOCKER_TIMELIMIT := 30
DOCKER_PWNABLESERVER_ARGS := --inject /usr/local/lib/pwnablepreload.so
