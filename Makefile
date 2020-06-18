BUILD	= tent libs
CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean libs

all: $(BUILD)
	mkdir -p bin

tent:
	cd src; make

libs:
	cd lib/utils; make

clean:
	rm -f bin/tent
	cd src; make clean
	cd lib/utils; make clean

install: all
	cp bin/tent /usr/local/bin/tent
