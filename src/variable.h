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
VariableMap *init_variable_map(void);
void load_variable_map(char *key_val_pairs, VariableMap *var_map);
void load_config(char *file_path, VariableMap *var_map);

#endif
