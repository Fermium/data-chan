#
#             LUFA Library
#     Copyright (C) Dean Camera, 2015.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#

# Makefile to build the LUFA library and the bootstrap AVR program.

# Call with "make all" to rebuild everything, "make clean" to clean everything,
# "make mostlyclean" to remove all intermediary files but preserve any binaries,
# "make doxygen" to document everything with Doxygen (if installed). Call
# "make help" for additional target build information within a specific project.
# Call "make flash" to write the binary to the AVR microcontroller using avrdude.

all:

%:
	@echo Executing \"make $@\" on all host-side data-chan elements.
	@echo
	$(MAKE) -C API $@
	$(MAKE) -C CPP_API $@
	$(MAKE) -C node_binding $@
	$(MAKE) -C Tester $@
	@echo
	@echo host-side data-chan \"make $@\" operation complete.
