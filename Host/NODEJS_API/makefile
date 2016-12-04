SWIG_MODULE_FILE 	= ../DataChan.i
SWIG 			= swig
ABS_POSITION 		=$(shell pwd)

.PHONY: default all clean debug release documentation

clean:
	-rm -rf build
	-rm -f DataChan_wrap.cxx
	-rm -f ../DataChan_wrap.cxx
	-rm -f binding.gyp
	-rm -f binding.gype

binding.gyp : binding.gyp.in
	cp binding.gyp.in binding.gyp
	sed -ie 's|_PATH_|'$(ABS_POSITION)'|g'  binding.gyp

default: binding.gyp
	$(SWIG) -javascript -node -c++ $(SWIG_MODULE_FILE)
	mv ../DataChan_wrap.cxx ./DataChan_wrap.cxx
	npm run exec -- node-gyp configure
	npm run exec -- node-gyp build

all: default
debug: all
release: all
flash: all
