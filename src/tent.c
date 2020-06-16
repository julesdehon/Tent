#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "string_utils.h"
#include "file_utils.h"
#include "variable.h"
#include "cmark_parser.h"
#include "template.h"

#define HELP_MESSAGE ("USAGE: tent <command> [options]\n\n"		\
		      "Commands:\n"					\
		      "- build (build static files to public from the theme and content)\n" \
		      "- create [site-name] (setup a new tent project in the current directory with default config and site-name)\n")

#define COM_BUILD ("build")
#define COM_CREATE ("create")
#define COM_NEW ("new")

#define DIR_CONTENT ("content")
#define DIR_THEME ("theme")
#define DIR_PUBLIC ("public")
#define FIL_CONFIG ("config.tent")

#define REPLACE_EXTENSION ("md")

void build_site_aux(const char *content_path, const char *public_path, VariableMap *config_map, TemplateMap *template_map) {
  DIR *dir;
  struct dirent *entry;

  if (!(dir = opendir(content_path))) {
    printf("Could not open directory '%s' - make sure there is a content/ folder in your current directory.\n", content_path);
    return;
  }

  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_DIR) {
      char content_directory_path[PATH_MAX];
      char public_directory_path[PATH_MAX];
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
	continue;
      snprintf(content_directory_path, sizeof(content_directory_path), "%s/%s", content_path, entry->d_name);
      snprintf(public_directory_path, sizeof(public_directory_path), "%s/%s", public_path, entry->d_name);
      mkdir(public_directory_path, DIR_PERMS);
      build_site_aux(content_directory_path, public_directory_path, config_map, template_map);
    } else {
      char path_to_content_file[PATH_MAX];
      snprintf(path_to_content_file, sizeof(path_to_content_file), "%s/%s", content_path, entry->d_name);
      FILE *f = fopen(path_to_content_file, "r");
      if (!f) {
	perror("tent.c - error reading files in content directory");
	exit(EXIT_FAILURE);
      }
      if (str_equal(file_extension_from_string(entry->d_name), REPLACE_EXTENSION)) {
	VariableMap *meta_map = init_variable_map();
	char *content = parse_markdown(f, meta_map);
	char path_to_html_file[PATH_MAX];
	char *without_extension = file_name_without_extension_from_string(entry->d_name);
	snprintf(path_to_html_file, sizeof(path_to_html_file), "%s/%s.html", public_path, without_extension);
	free(without_extension);
	FILE *out = fopen(path_to_html_file, "w");
	if (!out) {
	  perror("tent.c - error creating output file for a markdown conversion");
	  exit(EXIT_FAILURE);
	}
	fill_template(content, config_map, meta_map, template_map, out);
	fclose(out);
      } else {
	char path_to_public_file[PATH_MAX];
	snprintf(path_to_public_file, sizeof(path_to_public_file), "%s/%s", public_path, entry->d_name);
	FILE *out = fopen(path_to_public_file, "w");
	if (!out) {
	  perror("tent.c - error creating file for copying to public directory");
	  exit(EXIT_FAILURE);
	}
	copy_file(f, out);
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
  VariableMap *config_map = init_variable_map();
  load_config(FIL_CONFIG, config_map);
  TemplateMap *template_map = load_template_map();
  del_directory(DIR_PUBLIC);
  mkdir(DIR_PUBLIC, DIR_PERMS);
  copy_directory(DIR_THEME, DIR_PUBLIC, "templates");
  build_site_aux(DIR_CONTENT, DIR_PUBLIC, config_map, template_map);
}

void write_default_config(FILE* cf, char* site_name) {
  fprintf(cf, "site-name: %s", site_name);
}

void create_new_project(char* site_name) {
  struct stat st = {0};
  if (stat(site_name, &st) == -1) {
    if (mkdir(site_name, DIR_PERMS)) {
      perror("tent.c - couldn't make new site directory ");
      return;
    }
  } else {
    printf("Directory called %s already exists!\n", site_name);
    return;
  }

  char content_dir[strlen(site_name) + strlen(DIR_CONTENT) + 2]; // +1 for '/', +1 for '\0' = +2
  snprintf(content_dir, sizeof(content_dir), "%s/%s", site_name, DIR_CONTENT);
  mkdir(content_dir, DIR_PERMS);
  char theme_dir[strlen(site_name) + strlen (DIR_THEME) + 2];
  snprintf(theme_dir, sizeof(theme_dir), "%s/%s", site_name, DIR_THEME);
  mkdir(theme_dir, DIR_PERMS);
  char config_file[strlen(site_name) + strlen(FIL_CONFIG) + 2];
  snprintf(config_file, sizeof(config_file), "%s/%s", site_name, FIL_CONFIG);
  FILE* cf = fopen(config_file, "w");

  write_default_config(cf, site_name);

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
/* int main(void) { */
/*   VariableMap *meta_map = init_variable_map(); */
/*   FILE *f = fopen("markdown-eg.md", "r"); */
/*   char *content = parse_markdown(f, meta_map); */
/*   const char *key; */
/*   map_iter_t iter = map_iter(meta_map); */
/*   while ((key = map_next(meta_map, &iter))) { */
/*     printf("%s -> %s\n", key, (*map_get(meta_map, key))->value.str); */
/*   } */
/*   free_variable_map(meta_map); */
/*   printf("%s\n", content); */
/*   free(content); */
/*   return 0; */
/* } */
   
