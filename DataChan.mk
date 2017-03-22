CC	= gcc
CXX	= g++
ifeq ($(OS),Windows_NT)
        CC = gcc
        CXX = g++
else
        UNAME := $(shell uname)
        ifeq ($(UNAME), Linux)
                CC = clang
                CXX = clang++
        endif
        ifeq ($(UNAME), FreeBSD)
                CC = clang
                CXX = clang++
        endif
        ifeq ($(UNAME), Darwin)
                CC = clang
                CXX = clang++
        endif
endif

CFLAGS		= -fPIC -std=gnu99
CXXFLAGS	= -fPIC

SHARED_LIB_EXT	= .so
SHARED_LIB_FLAG	= -shared

ifndef INSTALL_DIR
INSTALL_DIR = /datachan
endif

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
