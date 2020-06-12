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

#endif
