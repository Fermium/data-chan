ABS_POSITION 		= $(shell pwd)

.PHONY: default all clean debug release documentation

clean:
	-rm -f libDataChan.*
	-rm -rf node_modules
	-rm -f *.tgz

default:
	cp ../API/libDataChan.* .
	npm install
	npm pack

all: default
debug: all
release: all
flash: all
documentation:
