#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>
#include <limits.h>

#define DIR_PERMS 0700

char* read_file_into_buffer(FILE* file, long* filelen);

char* file_path(FILE* file);

char* file_extension(FILE* file);
char *file_extension_from_string(char *file_name);

char* file_name(FILE* file);

char* file_name_without_extension(FILE* file);
char *file_name_without_extension_from_string(char *file_name);

void copy_file(FILE *from, FILE *to);

int copy_directory(char *from, char *to, char *exclude);

int del_directory(char *path);
#endif
