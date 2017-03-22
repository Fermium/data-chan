ifndef IDE_DIR
echo "Warning: sYou should to define IDE_DIR (without the final /)"
echo "Warning: Failing back to /usr/bin/arduino (Ubuntu)"
IDE_DIR="/usr/bin/arduino"
endif

ARDUINO_MK     = ../Make-Arduino
ARDUINO_DIR     = ../Make-Arduino
USER_LIB_PATH   = sketchbook
AVR_TOOLS_DIR = /usr
#ARDUINO_LIBS  +=
BOARD_TAG       = leonardo
MCU             = atmega32u4
F_CPU           = 16000000
CFLAGS          += -I../Bootstrap/ -D__ARDUINO__

include $(ARDUINO_MK)/Arduino.mk

CFLAGS_STD = -D__DISABLE_USB_ISR
OTHER_OBJS = -L ../Bootstrap -ldatachan
ISP_PROG   = usbasp
