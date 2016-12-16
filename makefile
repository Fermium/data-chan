

# Makefile to build the data-chan project: Host and Device side

# Call with "make all" to rebuild everything, "make clean" to clean everything,
# "make mostlyclean" to remove all intermediary files but preserve any binaries,
# "make doxygen" to document everything with Doxygen (if installed). Call
# "make help" for additional target build information within a specific project.
# Call "make flash" to write the binary to the AVR microcontroller using avrdude.

all:

%:
	@echo working on data-chan....
	@echo
	-mkdir Documentation
	$(MAKE) -C Device $@
	$(MAKE) -C Host $@
	@echo
	@echo \"make $@\" operation complete on data-chan.

