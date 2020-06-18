#ifndef TEST_H
#define TEST_H

#define MAX_TEST_STRING_LENGTH 120

int test_int(const int got, const int expected, const char* name);

int test_bool(const int got, const char* name);

int test_char(const char got, const char expected, const char* name);

int test_string(const char* got, const char* expected, const char* name);

void print_result(int passed, const char* fmt, ...);

void run_test_suite(int (*tests[])(char* testname), const int count, const char* filename);

#endif
