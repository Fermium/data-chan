ifndef IDE_DIR
$(error You have to define IDE_DIR (without the final /))
endif

ARDUINO_MK     = ../Make-Arduino/
ARDUINO_DIR     = ../Make-Arduino/
USER_LIB_PATH   = sketchbook
AVR_TOOLS_DIR = $(IDE_DIR)/hardware/tools/avr/
#ARDUINO_LIBS  +=
BOARD_TAG       = leonardo
MCU             = atmega32u4
F_CPU           = 16000000
CFLAGS          += -I../Bootstrap/ -D__ARDUINO__

include $(ARDUINO_MK)/Arduino.mk

CFLAGS_STD = -D__DISABLE_USB_ISR
OTHER_OBJS = -L ../Bootstrap -ldatachan
