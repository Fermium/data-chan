.PHONY: default all clean debug release documentation

clean:
	-rm -rf build
	-rm -f DataChan_wrap.cxx

default: DataChan.i
	swig -javascript -node -c++ DataChan.i
	node-gyp configure
	node-gyp build

all: default
debug: all
release: all
flash: all
