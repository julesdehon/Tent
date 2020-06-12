#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>

char* read_file_into_buffer(FILE* file, long* filelen);
void replace_strings(FILE* file, char* rep, char* with);

#endif
