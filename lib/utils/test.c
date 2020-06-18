#include "test.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

int test_int(const int got, const int expected, const char* name) {

	print_result(got == expected, "(int)  %d == %d  (?)  %s", got, expected, name);	

	return got == expected;
}

int test_bool(const int got, const char* name) {

	print_result(got, "(bool)  got = %d  (?)  %s", got, name);	

	return got;
}


int test_char(const char got, const char expected, const char* name) {

	print_result(got == expected, "(char)  %c == %c  (?)  %s", got, expected, name);	

	return got == expected;
}


int test_string(const char* got, const char* expected, const char* name) {

	print_result(got == expected, "(string)  %s == %s  (?)  %s", got, expected, name);	

	return got == expected;
}


void print_result(int passed, const char* fmt, ...) {
	fprintf(stdout, "[\x1b[93m TEST \x1b[0m]  -  ");
	va_list args;
	va_start(args, fmt);
	vfprintf(stdout, fmt, args);
	va_end(args);
	fprintf(stdout, "  -  ");
	fprintf(stdout, passed ? "\x1b[92mPASS\x1b[0m" : "\x1b[91mFAIL\x1b[0m");
	fprintf(stdout, "\n");
	fflush(stdout);
}

/*
 * Receives an array of function pointers, which receive a string which they
 * **MUST** copy the name of their test function into.
 * Example:
 *		int test_some_feature(char* tname) {
 *			strcpy(tname, "test_some_feature");	// MUST DO
 *			return 1 == 2;
 *		}
 */
void run_test_suite(int (*tests[])(char* testname), const int count, const char* filename) {
	char* passed = (char*) calloc(count * MAX_TEST_STRING_LENGTH, sizeof(char));
	char* failed = (char*) calloc(count * MAX_TEST_STRING_LENGTH, sizeof(char));

	int p_count = 0;
	int f_count = 0;

	printf("\nRunning test suite - %s\n", filename);

	char* tname = calloc(MAX_TEST_STRING_LENGTH, sizeof(char));
	char* l = calloc(MAX_TEST_STRING_LENGTH, sizeof(char));

	for (int i = 0; i < count; i++) {
		int pass = tests[i](tname);
		if (tname == NULL) {
			printf("Test function must set the test name.\n");
			return;
		}
		sprintf(l, "[\x1b[93m TEST \x1b[0m]  -  %s  -  ", tname);
		if (pass) {
			strcat(passed, l);
			strcat(passed, "\x1b[92mPASS\x1b[0m\n");
			p_count++;
		} else {
			strcat(failed, l);
			strcat(failed, "\x1b[91mFAIL\x1b[0m\n");
			f_count++;
		}
	}	

	free(tname);
	free(l);

	printf("%d / %d tests passed\n", p_count, count);
	printf("\n");
	printf("Passing tests: (%d)\n%s", p_count, passed);
	printf("\n");
	printf("Failing tests: (%d)\n%s\n", f_count, failed);

	free(passed);
	free(failed);
}
