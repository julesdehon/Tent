#ifndef HANDLE_MARKUPS_H
#define HANDLE_MARKUPS_H

#include "typedefs.h"

void handle_header(Line* line, char** tokens, int token_count);
void handle_bold(Line* line, char** tokens, int token_count);
void handle_italic(Line* line, char** tokens, int token_count);

#endif
