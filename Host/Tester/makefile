TEST_TARGET = Tester
LIBS = -L../CPP_API -lCPPDataChan -L../API -lDataChan -lusb-1.0 -lpthread -lm
CXX = g++
CFLAGS = -std=c++11 -Wall

.PHONY: default all clean debug release documentation

default: $(TEST_TARGET)
all: default
debug: CFLAGS += -DDEBUG -g -O0
debug: all
release: CFLAGS += -O3
release: all

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.hpp)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TEST_TARGET) $(OBJECTS)

$(TEST_TARGET): $(OBJECTS)
	$(CXX) -Wall $(OBJECTS) -o $@ $(LIBS)

clean:
	-rm -f *.o
	-rm -f *.exe
	-rm -f $(TEST_TARGET)

flash: all

documentation:
# do nothing. Kept to not break the buildsystem