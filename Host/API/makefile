CLIB_TARGET = libDataChan.a
CC = gcc
CFLAGS = -std=gnu99 -Wall -D__HOST__
LUFA_PATH    = ../../Device/lufa/LUFA

.PHONY: default all clean debug release documentation

default: $(CLIB_TARGET)
all: default
debug: CFLAGS += -DDEBUG -g -O0
debug: all
release: CFLAGS += -O3
release: all

documentation: export PROJECT_NUMBER:=$(shell git rev-parse HEAD ; git diff-index --quiet HEAD || echo "(with uncommitted changes)")
documentation: doxygen

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(CLIB_TARGET) $(OBJECTS)

$(CLIB_TARGET): $(OBJECTS)
	ar rsc $(CLIB_TARGET) $(OBJECTS)

clean:
	-rm -f *.o
	-rm -f *.a
	-rm -rf ../../Documentation/C_API

flash: all

include $(LUFA_PATH)/Build/lufa_doxygen.mk
