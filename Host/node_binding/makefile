SWIG_MODULE_FILE = ../DataChan.i
SWIG 			 = /swig/bin/swig

.PHONY: default all clean debug release documentation

clean:
	-rm -rf build
	-rm -f DataChan_wrap.cxx

default: $(SWIG_MODULE_FILE)
	$(SWIG) -javascript -node -c++ $(SWIG_MODULE_FILE)
	mv ../DataChan_wrap.cxx ./DataChan_wrap.cxx
	node-gyp configure
	node-gyp build

all: default
debug: all
release: all
flash: all
