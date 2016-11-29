CPPLIB_TARGET = libCPPDataChan.a
CXX = g++
CFLAGS = -std=c++11 -Wall
LUFA_PATH    = ../../Device/lufa/LUFA

.PHONY: default all clean debug release documentation

default: $(CPPLIB_TARGET)
all: default
debug: CFLAGS += -DDEBUG -g -O0
debug: all
release: CFLAGS += -O3
release: all

documentation: export PROJECT_NUMBER:=$(shell git rev-parse HEAD ; git diff-index --quiet HEAD || echo "(with uncommitted changes)")
documentation: doxygen

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.hpp)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(CPPLIB_TARGET) $(OBJECTS)

$(CPPLIB_TARGET): $(OBJECTS)
	ar rsc $(CPPLIB_TARGET) $(OBJECTS)

clean:
	-rm -f *.o
	-rm -f *.a

flash: all

include $(LUFA_PATH)/Build/lufa_doxygen.mk