#include "file_utils.h"
#include "string_utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Remember to free the returned buffer if non null!
char* read_file_into_buffer(FILE* file, long* filelen) {
  fseek(file, 0, SEEK_END);
  *filelen = ftell(file);
  fseek(file, 0, SEEK_SET);
  
  char* ret = calloc(*filelen, sizeof(char));
  fread(ret, *filelen, 1, file);
  return ret;
}

char* file_path(FILE* file) {
  char path[1024];
  char* result = calloc(1024, sizeof(char));
  
  int fd = fileno(file);
  sprintf(path, "/proc/self/fd/%d", fd);
  readlink(path, result, 1024 * sizeof(char));
  
  return result;
}

char* file_extension(FILE* file) {
  char* ret;
  char* name = file_name(file);
  char* dot = strrchr(name, '.') + 1;
  ret = calloc(strlen(dot) + 1, sizeof(char));
  memcpy(ret, dot, strlen(dot) + 1);
  free(name);
  return ret;
}

char* file_name(FILE* file) {
  char* ret;
  char* full_path = file_path(file);
  char* name = strrchr(full_path, '/') + 1;
  ret = calloc(strlen(name) + 1, sizeof(char));
  memcpy(ret, name, strlen(name) + 1);
  free(full_path);
  return ret;
}

char* file_name_without_extension(FILE* file) {
  char* ret;
  char* full_path = file_path(file);
  char* name = strrchr(full_path, '/') + 1;
  char* dot = strrchr(name, '.');
  ret = calloc(dot - name, sizeof(char));
  memcpy(ret, name, dot - name);
  free(full_path);
  return ret;
}

void copy_file(FILE *from, FILE *to) {
  char byte = fgetc(from);
  while(!feof(from)) {
    fputc(byte, to);
    byte = fgetc(from);
  }
}
