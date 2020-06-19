#ifdef __linux__
#define _XOPEN_SOURCE 500 // used by ftw.h
#endif

#include "file_utils.h"
#include "string_utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <ftw.h>

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
  char path[PATH_MAX];
  char* result = calloc(PATH_MAX, sizeof(char));
  
  int fd = fileno(file);
  sprintf(path, "/proc/self/fd/%d", fd);
  readlink(path, result, PATH_MAX * sizeof(char));
  
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

char *file_extension_from_string(char *file_name) {
  char *extension = strrchr(file_name, '.');
  if (!extension)
    return file_name + strlen(file_name);
  return extension + 1;
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

char *file_name_without_extension_from_string(char *file_name) {
  char *extension = strrchr(file_name, '.');
  char *ret;
  if (!extension) {
    ret = calloc(strlen(file_name) + 1, sizeof(char));
    strcpy(ret, file_name);
    return ret;
  }
  ret = calloc(extension - file_name, sizeof(char));
  memcpy(ret, file_name, extension - file_name);
  return ret;
}

void copy_file(FILE *from, FILE *to) {
  char byte = fgetc(from);
  while(!feof(from)) {
    fputc(byte, to);
    byte = fgetc(from);
  }
}

int copy_directory(char *from, char *to, char *exclude) {
  DIR *dir;
  struct dirent *entry;

  if (!(dir = opendir(from))) {
    printf("Could not open directory '%s'\n", from);
    return -1;
  }

  while((entry = readdir(dir)) != NULL) {
    char from_path[PATH_MAX];
    char to_path[PATH_MAX];
    snprintf(from_path, sizeof(from_path), "%s/%s", from, entry->d_name);
    snprintf(to_path, sizeof(to_path), "%s/%s",to, entry->d_name);
    if (entry->d_type == DT_DIR) {
      if (str_equal(entry->d_name, ".") || str_equal(entry->d_name, "..") || str_equal(entry->d_name, exclude))
	continue;
      mkdir(to_path, DIR_PERMS);
      copy_directory(from_path, to_path, "");
    } else {
      FILE *in = fopen(from_path, "r");
      if (!in) {
	closedir(dir);
	return -1;
      }
      FILE *out = fopen(to_path, "w");
      if (!out) {
	closedir(dir);
	return -1;
      }
      copy_file(in, out);
      fclose(in);
      fclose(out);
    }
  }
  closedir(dir);
  return 0;
}

int remove_file_callback(const char *path, const struct stat *stat_buffer,
			 int typeflag, struct FTW *ftwbuf) {
  int rv = remove(path);
  if (rv)
    perror(path);
  return rv;
}

int del_directory(char *path) {
  return nftw(path, remove_file_callback, 64, FTW_DEPTH | FTW_PHYS);
}

