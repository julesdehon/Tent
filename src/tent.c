#include <stdio.h>
#include <stdlib.h>

#include "file_utils.h"
#include "string_utils.h"

int main(int argc, char** argv) {
	FILE* file = fopen(argv[1], "r");
	if (!file) {
		return EXIT_FAILURE;
	}

	long buflen;
	char* buf = read_file_into_buffer(file, &buflen);


	char* new = str_replace(buf, "Casey", "Jules");

	printf("%s\n", new);

	FILE* out = fopen(argv[2], "w");
	if (!out) {
		return EXIT_FAILURE;
	}

	fputs(new, out);

	free(buf);
	free(new);
	fclose(file);
	fclose(out);

	return 0;
}
