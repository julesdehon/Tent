#include <stdio.h>
#include <stdlib.h>

#include "file_utils.h"
#include "string_utils.h"
#include "template.h"

int main(int argc, char** argv) {
	FILE* file = fopen(argv[1], "r");
	if (!file) {
		return EXIT_FAILURE;
	}

	long buflen;
	char* buf = read_file_into_buffer(file, &buflen);

	/* char* fp = file_path(file); */
	/* char* fn = file_name(file); */
	/* char* fnne = file_name_without_extension(file); */
	/* char* fe = file_extension(file); */
	/* printf("%s\n", fp); */
	/* printf("%s\n", fn); */
	/* printf("%s\n", fnne); */
	/* printf("%s\n", fe); */


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
