CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -pedantic -I../../lib/utils -I../../lib
LDLIBS  = -L../../lib/utils -lutils -lcmark
TOBJS   = run_tests.o ../cmark_parser.o ../variable.o ../string_utils.o ../map.o ../file_utils.o

.PHONY: tests

tests: $(TOBJS)
	$(CC) $(CFLAGS) $(TOBJS) $(LDLIBS) -o run_tests
	./run_tests

run_tests.o: ../cmark_parser.h ../variable.h ../string_utils.h

clean:
	rm -f *.o run_tests
