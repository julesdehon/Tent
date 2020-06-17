CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all:
	cd src; make

clean:
	rm bin/tent
	cd src; make clean

install: all
	cp bin/tent /usr/local/bin/tent
