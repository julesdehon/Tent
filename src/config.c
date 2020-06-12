#include "config.h"

#include "file_utils.h"
#include "variable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONFIG_FILE ("config.tent")

void populate_config(Config* conf, char* conf_buf, long buf_len) {
	char** saveptr = &conf_buf;
	char* line = strtok_r(conf_buf, "\n", saveptr);
	while (line) {
		char* key = strtok_r(line, ":", &line);
		char* value = strtok_r(NULL, "\n", &line);
		VariableType vt = determine_vartype(value);
		line = strtok_r(NULL, "\n", saveptr);
	}
}

Config* load_config() {
	Config* conf = malloc(sizeof(conf));
	/* map_init(conf); */
	FILE* f = fopen(CONFIG_FILE, "r");
	if (!f) {
		perror("config.c: load_config() - Error opening config file");
		exit(EXIT_FAILURE);
	}

	long buf_len;
	char* buf = read_file_into_buffer(f, &buf_len);
	populate_config(conf, buf, buf_len);
	return conf;
}
