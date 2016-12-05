ADDON				= _DataChan.so
SWIG_MODULE_FILE 	= ../DataChan.i
SWIG 				= swig
ABS_POSITION 		= $(shell pwd)
CC = gcc
CFLAGS = -std=gnu99 -Wall -fPIC -D__HOST__
LIBS = -L../API -lDataChan -lusb-1.0 -lpthread -lm
PYTHON_LIB			= -lpython2.7
PYTHON_INCLUDE 		= -I/usr/include/python2.7

.PHONY: default all clean debug release documentation

DataChan_wrap.c:
	$(SWIG) -python $(SWIG_MODULE_FILE)
	mv ../DataChan_wrap.c ./DataChan_wrap.c
	mv ../DataChan.py ./DataChan.py

DataChan_wrap.o: DataChan_wrap.c
	$(CC) $(CFLAGS) -c DataChan_wrap.c $(PYTHON_INCLUDE) -o DataChan_wrap.o

default: DataChan_wrap.o
	$(CC) -Wall -shared DataChan_wrap.o $(OBJECTS) $(LIBS) $(PYTHON_LIB) -o $(ADDON)
	make -C examples import

debug: CFLAGS += -DDEBUG -g -O0
debug: all
release: CFLAGS += -O3
release: all

clean:
	-rm -f *.o
	-rm -f *.so
	-rm -f *.dll
	-rm -rf build
	-rm -f DataChan_wrap.c
	-rm -f ../DataChan_wrap.c
	-rm -f DataChan.py
	-rm -f ../DataChan.py
	-rm -f DataChan.pyc
	-rm -f ../DataChan.pyc
	-rm -f $(ADDON)
	-$(MAKE) -C examples clean

all: default
flash: all
documentation:
