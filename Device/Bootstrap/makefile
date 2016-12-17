# --------------------------------------
#       data-chan Project Makefile.
# --------------------------------------

# Run "make help" for target help.

MCU          = atmega32u4
ARCH         = AVR8
BOARD        = USBKEY
PROGRAMMER	 = usbasp
F_CPU        = 16000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
TARGET       = Bootstrap
SRC          = $(TARGET).c Descriptors.c datachan.c Settings.c Custom.c $(LUFA_SRC_USB)
LUFA_PATH    = ../lufa/LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/
LD_FLAGS     =

# Default target
all:

debug: all
release: all

install: all
	-avrdude -C./avrdude.conf -p $(MCU) -c $(PROGRAMMER) -Uflash:w:$(TARGET).hex:i

documentation: export PROJECT_NUMBER:=$(shell git rev-parse HEAD ; git diff-index --quiet HEAD || echo "(with uncommitted changes)")
documentation: doxygen

# Include LUFA build script makefiles
include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_sources.mk
include $(LUFA_PATH)/Build/lufa_build.mk
include $(LUFA_PATH)/Build/lufa_cppcheck.mk
include $(LUFA_PATH)/Build/lufa_doxygen.mk
include $(LUFA_PATH)/Build/lufa_dfu.mk
include $(LUFA_PATH)/Build/lufa_hid.mk
include $(LUFA_PATH)/Build/lufa_avrdude.mk
include $(LUFA_PATH)/Build/lufa_atprogram.mk
