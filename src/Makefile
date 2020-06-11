CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic
T_OBJS  = string_utils.o file_utils.o

.SUFFIXES: .c .o

.PHONY: all clean

all: tent
	mkdir -p ../bin

clean:
	rm -f *.o
	rm -rf ../bin

tent: tent.o string_utils.o file_utils.o
	$(CC) $(CFLAGS) tent.o string_utils.o file_utils.o -o ../bin/tent

tent.o: string_utils.h file_utils.h

string_utils.o: string_utils.h

file_utils.o: file_utils.h
