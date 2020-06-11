#ifndef METADATA_H
#define METADATA_H

#include "map.h"

typedef enum VariableType {
  STRING,
  ARRAY
} VariableType;

typedef struct Variable {
  VariableType type;
  union {
    char* value;
    char** arr;
  };
} Variable;

typedef map_t(Variable*) VariableMap;

#endif
