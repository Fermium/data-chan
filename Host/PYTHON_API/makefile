include ../../flow.mk

ADDON				= _DataChan.$(SHARED_LIB_EXT)
SWIG_MODULE_FILE 	= ../DataChan.i
SWIG 				= swig
ABS_POSITION 		= $(shell pwd)
LIBS = -L../API -lDataChan -lusb-1.0 -lpthread -lm

.PHONY: default all clean debug release documentation

DataChan_wrap.c:
	$(SWIG) -python $(SWIG_MODULE_FILE)
	mv ../DataChan_wrap.c ./DataChan_wrap.c
	mv ../DataChan.py ./DataChan.py

DataChan_wrap.o: DataChan_wrap.c
	$(PYTHON_CC) $(PYTHON_FLAGS) -c DataChan_wrap.c $(PYTHON_INCLUDE) -o DataChan_wrap.o

default: DataChan_wrap.o
	$(PYTHON_CC) $(SHARED_LIB_FLAG) DataChan_wrap.o $(OBJECTS) $(LIBS) $(PYTHON_LIB) -o $(ADDON)
	make -C examples import

debug: PYTHON_FLAGS += -DDEBUG -g -O0
debug: all
release: PYTHON_FLAGS += -O3
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
