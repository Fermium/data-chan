ABS_POSITION 		= $(shell pwd)

.PHONY: default all clean debug release documentation

clean:
	-rm -f libDataChan.*
	-rm -rf node_modules
	-rm -f *.tgz

default:
	cp ../libDataChan.* .
	npm install 
	
release: default
	npm pack
	npm run publish-libraries
	

	

all: default
debug: all
flash: all
documentation:
