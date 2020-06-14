#ifndef INSERT_H
#define INSERT_H

#include "variable.h"
#include "template.h"

char* get_insert(char* insert, char* content,
	       	VariableMap* config, VariableMap* variables, TemplateMap* templates);

#endif
