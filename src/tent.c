#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "string_utils.h"
#include "file_utils.h"

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

void build_site_aux(const char *content_path, const char *public_path/* , VariableMap *config_map */)
{
  DIR *dir;
  struct dirent *entry;

  if (!(dir = opendir(content_path))) {
    printf("Could not open directory '%s' - make sure there is a content/ folder in your current directory.\n", content_path);
    return;
  }

  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_DIR) {
      char content_directory_path[1024];
      char public_directory_path[1024];
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
	continue;
      snprintf(content_directory_path, sizeof(content_directory_path), "%s/%s", content_path, entry->d_name);
      snprintf(public_directory_path, sizeof(public_directory_path), "%s/%s", public_path, entry->d_name);
      mkdir(public_directory_path, DIR_PERMS);
      build_site_aux(content_directory_path, public_directory_path/*, VariableMap *config_map */);
    } else {
      char path_to_content_file[1024];
      snprintf(path_to_content_file, sizeof(path_to_content_file), "%s/%s", content_path, entry->d_name);
      FILE *f = fopen(path_to_content_file, "r");
      if (!f) {
	perror("tent.c - error reading files in content directory");
	exit(EXIT_FAILURE);
      }
      if (str_equal(file_extension(f), "md")) {
	//VariableMap *meta_map = init_variable_map();
	//char *content = parse_markdown(f, meta_map);
	char path_to_html_file[1024];
	snprintf(path_to_html_file, sizeof(path_to_html_file), "%s/%s.html", public_path, file_name_without_extension(f));
	FILE *out = fopen(path_to_html_file, "w");
	if (!out) {
	  perror("tent.c - error creating output file for a markdown conversion");
	  exit(EXIT_FAILURE);
	}
	//fill_template(content, config_map, meta_map, out);
	fclose(out);
      } else {
	char path_to_public_file[1024];
	snprintf(path_to_public_file, sizeof(path_to_public_file), "%s/%s", public_path, file_name(f));
	FILE *out = fopen(path_to_public_file, "w");
	if (!out) {
	  perror("tent.c - error creating file for copying to public directory");
	  exit(EXIT_FAILURE);
	}
	//copy_file(f, out); // <-- this should be a new file util to implement
	fclose(out);
      }
      fclose(f);
    }
  }
  closedir(dir);
}

void print_help() {
  printf("\n%s\n", HELP_MESSAGE);
}

void build_site() {
  // VariableMap *config_map = init_variable_map();
  // load_config("config.tent", config_map);
  DIR *dir;
  if ((dir = opendir("public"))) {
    closedir(dir);
    printf("Please delete the public folder before building.\n");
    return;
  }
  mkdir("public", DIR_PERMS);
  build_site_aux("content", "public"/* , config_map */);
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
