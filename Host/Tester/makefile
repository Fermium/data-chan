TARGET = Tester
LIBS = -L../CPP_API -lCPPDataChan -L../API -lDataChan -lusb-1.0 -lpthread -lm
CXX = g++
CFLAGS = -std=c++11 -Wall -D__HOST__
# -O2 is to be used only on release 
# -fPIC generates a warning....

.PHONY: default all clean debug release

default: $(TARGET)
all: default
debug: CFLAGS += -DDEBUG -g -O0
debug: all
release: CFLAGS += -O3
release: all

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.hpp)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CXX) -Wall $(OBJECTS) -o $@ $(LIBS)

clean:
	-rm -f *.o
	-rm -f *.exe
	-rm -f $(TARGET)

flash: all
