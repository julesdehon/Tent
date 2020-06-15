#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>

char* read_file_into_buffer(FILE* file, long* filelen);

char* file_path(FILE* file);

char* file_extension(FILE* file);

char* file_name(FILE* file);

char* file_name_without_extension(FILE* file);

void copy_file(FILE *from, FILE *to);
#endif
