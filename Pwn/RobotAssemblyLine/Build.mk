TARGET := robots-assemble

NX := 1
ASLR := 1
RELRO := 1
CANARY := 1
# STRIP := 1
# DEBUG := 1

LDLIBS := -lpthread

PUBLISH_BUILD := $(TARGET)

DOCKER_IMAGE := robots-assemble
DOCKER_PORTS := 23000
