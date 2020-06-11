#include "file_utils.h"

#include <stdlib.h>

// Remember to free the returned buffer if non null!
char* read_file_into_buffer(FILE* file, long* filelen) {
	fseek(file, 0, SEEK_END);
	*filelen = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* ret = calloc(*filelen, sizeof(char));
	fread(ret, *filelen, 1, file);
	return ret;
}
