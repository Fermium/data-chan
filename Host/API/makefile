include ../../flow.mk

CLIB_FLAG = -shared
CLIB_TARGET = libDataChan.$(SHARED_LIB_EXT)

CFLAGS += -DDATACHAN_EXPORT -D__HOST__ -DDATACHAN_CPP_EXPORT
LIBS = -lusb-1.0 -lpthread -lm
LUFA_PATH    = ../../Device/lufa/LUFA

.PHONY: default all clean debug release documentation

default: $(CLIB_TARGET)
	$(MAKE) -C ./examples $@

all: default
debug: CFLAGS += -DDEBUG -g -O0
debug: all
release: CFLAGS += -O3
release: all

documentation: export PROJECT_NUMBER:=$(shell git rev-parse HEAD ; git diff-index --quiet HEAD || echo "(with uncommitted changes)")
documentation: doxygen

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(CLIB_TARGET) $(OBJECTS)

$(CLIB_TARGET): $(OBJECTS)
	$(CC) $(SHARED_LIB_FLAG) -o $(CLIB_TARGET) $(OBJECTS) $(LIBS)

clean:
	-rm -f $(CLIB_TARGET)
	-rm -f *.o
	-rm -f *.a
	-rm -rf ../../Documentation/C_API
	$(MAKE) -C examples clean

flash: all

include $(LUFA_PATH)/Build/lufa_doxygen.mk
