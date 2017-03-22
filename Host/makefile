include ../DataChan.mk

CLIB_FLAG = -shared
CLIB_TARGET = libDataChan.$(SHARED_LIB_EXT)

CFLAGS += -DDATACHAN_EXPORT -D__HOST__ -DDATACHAN_CPP_EXPORT -I ../
LIBS = -lusb-1.0 -lpthread -lm
LUFA_PATH    = ../Device/lufa/LUFA

.PHONY: default all clean debug release documentation install

default: $(CLIB_TARGET)
	$(MAKE) -C ./examples $@

all: default
debug: CFLAGS += -DDEBUG -g -O0 -Wall
debug: all
release: CFLAGS += -O3 -w
release: all
release: 
	$(MAKE) -C ./examples $@

lib: release #just.... build a lib out of sources
arduinoupload: all

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
	-rm -rf ../Documentation/C_API
	$(MAKE) -C examples clean

install: all documentation
	-mkdir $(INSTALL_DIR)
	cp ../DataChan.mk $(INSTALL_DIR)/
	-mkdir $(INSTALL_DIR)/doc
	cp -rf ../Documentation/C_API/html/ $(INSTALL_DIR)/doc/
	-mkdir $(INSTALL_DIR)/lib
	cp $(CLIB_TARGET) $(INSTALL_DIR)/lib/
	-mkdir $(INSTALL_DIR)/include
	cp $(HEADERS) $(INSTALL_DIR)/include/
	-mkdir $(INSTALL_DIR)/include/Protocol
	cp ../Protocol/data_management.h ../Protocol/measure.h $(INSTALL_DIR)/include/Protocol/
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(INSTALL_DIR)/lib/
	-ldconfig $(INSTALL_DIR)/lib/
	-ln -s $(INSTALL_DIR)/lib/$(CLIB_TARGET) /usr/lib/$(CLIB_TARGET)

include $(LUFA_PATH)/Build/lufa_doxygen.mk
