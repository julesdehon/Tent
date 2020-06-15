CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic -I../include
LDLIBS = -L../lib -lcmark
T_OBJS  = tent.o string_utils.o file_utils.o template.o map.o variable.o insert.o cmark_parser.o

.SUFFIXES: .c .o

.PHONY: all clean

all: tent
	mkdir -p ../bin

clean:
	rm -f *.o
	rm -rf ../bin

tent: $(T_OBJS)
	mkdir -p ../bin
	$(CC) $(CFLAGS) $(T_OBJS) $(LDLIBS) -o ../bin/tent

tent.o: string_utils.h file_utils.h template.h cmark_parser.h

string_utils.o: string_utils.h

file_utils.o: file_utils.h

template.o: template.h file_utils.h map.h

map.o: map.h

variable.o: variable.h string_utils.h file_utils.h map.h

insert.o: insert.h string_utils.h map.h variable.h template.h 

cmark_parser.o: cmark_parser.h variable.h string_utils.h file_utils.h
