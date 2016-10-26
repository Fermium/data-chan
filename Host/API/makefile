TARGET = libDataChan.a
LIBS = -lusb-1.0
CC = gcc
CFLAGS = -g -Wall -O2 -fPIC -D__HOST__

.PHONY: default all clean

default: $(TARGET)
all: default

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
