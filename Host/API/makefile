TARGET = libDataChan.a
LIBS = -lusb-1.0 -lpthread
CC = gcc
CFLAGS = -g -Wall -O2 -D__HOST__ 
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
