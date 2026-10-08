TARGET := robots-assemble

UBUNTU_VERSION := 22.04

NX := 1
ASLR := 1
RELRO := 1
CANARY := 1
# STRIP := 1
# DEBUG := 1

LDLIBS := -lpthread

PUBLISH_BUILD := $(TARGET)

DOCKER_IMAGE := sun23-robots-assemble
DOCKER_PORTS := 23000

# `pwnmake check`: the exploit only works some of the time under ASLR, so solve.py
# retries up to 64 times. Give it a generous budget so it reliably lands a win.
CHECK_TIMEOUT := 300
$(call ctf_check_slow,$(DIR),$(DOCKER_PORTS),python3 solve.py)
