TARGET = DataChan
LIBS = -lm -lusb
CC = gcc
CFLAGS = -g -Wall -O2 -fPIC

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	ar rsc lib$(TARGET).a $(OBJECTS)
#	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f *.a
	-rm -f $(TARGET)

flash: all
