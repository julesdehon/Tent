#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "string_utils.h"

#define HELP_MESSAGE "USAGE: tent <command> [options]\n\n"		\
  "Commands:\n"								\
  "- build (build static files to public from the theme and content)\n"	\
  "- create [site-name] (setup a new tent project in the current directory with default config and site-name)\n"

#define COM_BUILD "build"
#define COM_CREATE "create"
#define COM_NEW "new"

#define DIR_PERMS 0700

#define DIR_CONTENT "/content"
#define DIR_THEME "/theme"
#define FIL_CONFIG "/config.tent"

// UNFINISHED!
void build_site_aux(const char *name)
{
  DIR *dir;
  struct dirent *entry;

  if (!(dir = opendir(name)))
    return;

  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_DIR) {
      char path[1024];
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
	continue;
      snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
      build_site_aux(path);
    } else {
      printf("Found file: %s\n", entry->d_name);
      char path_to_file[1024];
      snprintf(path_to_file, sizeof(path_to_file), "%s/%s", name, entry->d_name);
      FILE *f = fopen(path_to_file, "r");
      if (!f) {
	perror("tent.c - error reading files in content directory");
	exit(EXIT_FAILURE);
      }
      if (str_equal(file_extension(f), "md")) {
	printf("Parsing markdown file: %s\n", file_name(f));
	
	// char *content = parse_markdown
      }
    }
  }
  closedir(dir);
}

void print_help() {
  printf("\n%s\n", HELP_MESSAGE);
}

//UNFINISHED!!
void build_site() {
  build_site_aux(".");
}

void write_default_config(FILE* cf, char* site_name) {
  fprintf(cf, "site-name: %s", site_name);
}

void create_new_project(char* site_name) {
  struct stat st = {0};
  if (stat(site_name, &st) == -1) {
    mkdir(site_name, DIR_PERMS);
  } else {
    printf("Directory called %s already exists!\n", site_name);
    return;
  }

  char* content_dir = str_append(site_name, DIR_CONTENT);
  mkdir(content_dir, DIR_PERMS);
  char* theme_dir = str_append(site_name, DIR_THEME);
  mkdir(theme_dir, DIR_PERMS);
  char* config_file = str_append(site_name, FIL_CONFIG);
  FILE* cf = fopen(config_file, "w");

  write_default_config(cf, site_name);

  free(content_dir);
  free(theme_dir);
  free(config_file);
  fclose(cf);
}

int main(int argc, char** argv) {
  if (argc == 1) {
    print_help();
    return EXIT_SUCCESS;
  }

  char* command = argv[1];
  if (str_equal(COM_BUILD, command)) {
    build_site();
  } else if (str_equal(COM_CREATE, command)) {
    if (argc != 3) {
      print_help();
      return EXIT_SUCCESS;
    }
    create_new_project(argv[2]);
  } else {
    print_help();
  }

  return EXIT_SUCCESS;
}
