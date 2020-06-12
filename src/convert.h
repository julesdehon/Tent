#ifndef CONVERT_H
#define CONVERT_H
#include "file_utils.h"
#include "typedefs.h"
#include <stdint.h>

char* parse_markdown(FILE *input);
char** tokenize(char* line, char** saveptr, int* out_count);
MarkupType determine_markup(char* currWord);

#endif
