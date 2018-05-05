ifeq ($(strip $(shell echo $$CC)),)
CC := gcc
endif

TOOLCHAIN_PREFIX :=

W32_PREFIX := i686-w64-mingw32-
W64_PREFIX := x86_64-w64-mingw32-

EXE_NAME := bspbuild

MODULES := bsp codegen ips main optparse patch pbuilder util

.SUFFIXES:
.PHONY: all clean debug w32 w64 windows bspbuild install

all: CFLAGS := -O3
all: bspbuild
	${TOOLCHAIN_PREFIX}strip -s ${EXE_NAME}

debug: CFLAGS := -ggdb -Og
debug: bspbuild

bspbuild:
	${TOOLCHAIN_PREFIX}${CC} ${CFLAGS} ${MODULES:=/*.c} -o ${EXE_NAME}

clean:
	rm -rf bspbuild bspbuild*.exe

install:
	cp ${EXE_NAME} /usr/local/bin/
	chmod 0755 /usr/local/bin/${EXE_NAME}

w32: TOOLCHAIN_PREFIX := ${W32_PREFIX}
w64: TOOLCHAIN_PREFIX := ${W64_PREFIX}

w32 w64: EXE_NAME := bspbuild.exe
w32 w64 windows: CC := gcc

w32: all
w64: all

windows:
	make TOOLCHAIN_PREFIX=${W32_PREFIX} EXE_NAME=bspbuild32.exe
	make TOOLCHAIN_PREFIX=${W64_PREFIX} EXE_NAME=bspbuild64.exe
