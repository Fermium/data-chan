TARGET = libDataChan.a
CC = gcc
CFLAGS = -std=gnu99 -Wall -D__HOST__
# -O2 is to be used only on release 
# -fPIC generates a warning....

.PHONY: default all clean debug release

default: $(TARGET)
all: default
debug: CFLAGS += -DDEBUG -g -O0
debug: all
release: CFLAGS += -O3
release: all

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	ar rsc $(TARGET) $(OBJECTS)

clean:
	-rm -f *.o
	-rm -f *.a

flash: all
