#ifndef STRING_UTILS_H
#define STRING_UTILS_H

char* str_replace(const char* orig, char* rep, char* with);
char* trim_whitespace(char* orig);

int str_equal(char* a, char* b);

char* str_append(char* start, char* add);

#endif
