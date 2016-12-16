CC	= gcc
CXX	= g++

CFLAGS		= -fPIC -std=gnu99
CXXFLAGS	= -fPIC

SHARED_LIB_EXT	= .so
SHARED_LIB_FLAG	= -shared

INSTALL_DIR = /datachan

SHARED_LIB_FLAG	= -shared
ifeq ($(OS),Windows_NT)
	SHARED_LIB_EXT := dll
else
	UNAME := $(shell uname)
	ifeq ($(UNAME), Linux)
		SHARED_LIB_EXT := so
	endif
	ifeq ($(UNAME), FreeBSD)
		SHARED_LIB_EXT := so
	endif
	ifeq ($(UNAME), Darwin)
		SHARED_LIB_FLAG = -dynamiclib
		SHARED_LIB_EXT := dylib
	endif
endif

DATACHAN_INCLUDE 	= $(INSTALL_DIR)/include
DATACHAN_LIB		= $(INSTALL_DIR)/lib
