#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "string_utils.h"

#define HELP_MESSAGE "USAGE: tent <command> [options]\n\n"\
"Commands:\n"\
"- build (build static files to public from the theme and content)\n"\
"- create [site-name] (setup a new tent project in the current directory with default config and site-name)\n"

#define COM_BUILD "build"
#define COM_CREATE "create"
#define COM_NEW "new"

#define DIR_PERMS 0700

#define DIR_CONTENT "/content"
#define DIR_THEME "/theme"
#define FIL_CONFIG "/config.tent"

void print_help() {
	printf("\n%s\n", HELP_MESSAGE);
}

void build_site() {
	printf("I will build the site!\n");
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
