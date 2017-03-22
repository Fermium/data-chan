TEST_TARGET = Potentiometer
LIBS = -L../../ -lDataChan
CXX = gcc
CFLAGS += -std=gnu99 -I../../../

.PHONY: default all clean debug release documentation install

default: $(TEST_TARGET)
all: default
debug: CFLAGS += -DDEBUG -g -O0
debug: all
release: CFLAGS += -O3
release: all



OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CXX) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TEST_TARGET) $(OBJECTS)

$(TEST_TARGET): $(OBJECTS)
	$(CXX) -Wall $(OBJECTS) -o $@ $(LIBS)

clean:
	-rm -f *.o
	-rm -f *.exe
	-rm -f $(TEST_TARGET)
	-rm -f Potentiometer

flash: all

install: all
documentation:
arduinoupload:
# do nothing. Kept to not break the buildsystem
