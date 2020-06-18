CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic
LIB     = libutils.a
LIBOBJS = log.o test.o fileio.o
BUILD	= $(LIB)

all:	$(BUILD)

clean:
	rm -f *.o libutils.a

$(LIB): $(LIBOBJS)
	ar rcs $(LIB) $(LIBOBJS)
	ranlib $(LIB)

log.o: log.h

test.o: test.h

fileio.o: fileio.h
