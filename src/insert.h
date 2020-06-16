#ifndef INSERT_H
#define INSERT_H

#include "variable.h"
#include "template.h"

char* get_insert(char* insert, char* content,
	       	VariableMap* config, VariableMap* variables, 
          VariableMap* args_named, Variable** args_pos,
          int var_arg_index, int args_length, TemplateMap* templates);

#endif
