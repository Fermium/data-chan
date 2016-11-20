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
	@echo Executing \"make $@\" on all LUFA library elements.
	@echo
	$(MAKE) -C Bootstrap $@
	@echo
	@echo LUFA \"make $@\" operation complete.
