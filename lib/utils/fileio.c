#include "fileio.h"

#include <stdlib.h>

char* read_text_file(FILE* file) {
	fseek(file, 0, SEEK_END);
	unsigned long buffer_size = ftell(file);
	rewind(file);
	char* buffer = (char *) calloc((buffer_size + 1), sizeof(char)); //allocate memory for buffer
	fread(buffer, buffer_size, 1, file);
	return buffer;
}
