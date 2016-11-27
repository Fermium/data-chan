TARGET = libCPPDataChan.a
LIBS = -L../API -lDataChan -lusb-1.0 -lpthread -lm
CXX = g++
CFLAGS = -std=c++11 -g -Wall -D__HOST__ -O0 
# -O2 is to be used only on release 
# -fPIC generates a warning....

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.hpp)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	ar rsc $(TARGET) $(OBJECTS)

clean:
	-rm -f *.o
	-rm -f *.a

flash: all

