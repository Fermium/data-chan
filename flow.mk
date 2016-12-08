CC	= clang
CXX	= clang++

PYTHON_CC		= gcc
PYTHON_FLAGS	= -fPIC
PYTHON_LIB		= -lpython2.7
PYTHON_INCLUDE	= -I/usr/include/python2.7

CFLAGS		= -Wall -fPIC -std=gnu99
CXXFLAGS	= -Wall -fPIC

SHARED_LIB_EXT	= .so
SHARED_LIB_FLAG	= -shared


SHARED_LIB_FLAG	= -shared
ifeq ($(OS),Windows_NT)
	SHARED_LIB_EXT := dll
else
	UNAME := $(shell uname)
	ifeq ($(UNAME), Linux)
		SHARED_LIB_EXT := so
	endif
	ifeq ($(UNAME), Darwin)
		SHARED_LIB_FLAG = -dynamiclib
		SHARED_LIB_EXT := dylib
	endif
endif
