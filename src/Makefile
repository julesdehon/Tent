CC      = gcc
CFLAGS  = -Wall -g -D_DEFAULT_SOURCE -Werror -std=c99 -pedantic -I../include -I../lib/utils
LDLIBS = -L../lib/utils -lcmark -lutils
T_OBJS  = tent.o string_utils.o file_utils.o template.o map.o variable.o insert.o cmark_parser.o

.SUFFIXES: .c .o

.PHONY: all clean tests libs

all: tent libs

clean:
	cd tests; make clean
	rm -f *.o
	rm -f ../bin/tent

libs:
	cd ../lib/utils; make

tent: $(T_OBJS)
	mkdir -p ../bin
	$(CC) $(CFLAGS) $(T_OBJS) $(LDLIBS) -o ../bin/tent

tests:
	make
	cd tests; make

tent.o: string_utils.h file_utils.h template.h cmark_parser.h variable.h

string_utils.o: string_utils.h

file_utils.o: file_utils.h string_utils.h

template.o: template.h file_utils.h map.h

map.o: map.h

variable.o: variable.h string_utils.h file_utils.h map.h

insert.o: insert.h string_utils.h map.h variable.h template.h 

cmark_parser.o: cmark_parser.h variable.h string_utils.h file_utils.h
