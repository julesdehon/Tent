#ifndef VARIABLE_H
#define VARIABLE_H

#include "map.h"

typedef enum VariableType {
  VT_STRING,
  VT_ARRAY
} VariableType;

typedef struct Variable {
  VariableType type;
  union {
    char* str;
    char** arr;
  } value;
} Variable;

typedef map_t(Variable*) VariableMap;

VariableType determine_vartype(char* value);
void free_variable_map(VariableMap *var_map);
VariableMap *load_variable_map(char *key_val_pairs);
VariableMap *load_config(char *file_path);

#endif
