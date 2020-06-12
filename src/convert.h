#ifndef CONVERT_H
#define CONVERT_H
#include "typedefs.h"
#include <stdint.h>

char** tokenize(char* line, char** saveptr, int* out_count);
MarkupType determine_markup(char* currWord);

#endif
