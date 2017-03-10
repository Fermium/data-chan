ABS_POSITION 		= $(shell pwd)

.PHONY: default all clean debug release documentation

clean:
	-rm -f libDataChan.*
	-rm -rf node_modules
	-rm -f *.tgz

default:
	cp ../libDataChan.* .
	npm install 
	
release: clean default
	npm pack
	

	

all: default
debug: all
flash: all
documentation:
