TARGET = libDataChan.a
LIBS = -lusb-1.0 -lpthread -lm
CC = gcc
CFLAGS = -std=gnu99 -g -Wall -D__HOST__ -O0 
# -O2 is to be used only on release 
# -fPIC generates a warning....

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
