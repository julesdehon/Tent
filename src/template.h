#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "map.h"
#include "variable.h"

#include <stdio.h>

// Represents all of the possible template types we can support
// Underneath there is no difference but separating them allows
// the syntax for the templating to be nicer and also means the user
// has more naming possibilites
typedef enum TemplateType {
  TT_PAGE,
  TT_PARTIAL,
  TT_CSS,
  TT_JS,
  TT_SNIPPET
} TemplateType;

// The template struct, which will be used by the templater to replace
// {{ type.name }} with the relevant text content
typedef struct Template {
  char* name;
  TemplateType type;
  char* content;
  long content_len;
} Template;

typedef map_t(Template*) TemplateMap; 

Template* load_template(FILE* file, TemplateType type, char *file_name);

void destroy_template(Template* t);

void destroy_template_map(TemplateMap *t_map);

void fill_template(char* content, VariableMap* config, VariableMap* variables,
    TemplateMap* templates, FILE* out);

TemplateMap* load_template_map();

char* replace_inserts(char* template, char* content, VariableMap* config,
    VariableMap* variables, VariableMap* args_named, Variable** args_pos, 
    int var_arg_index, int args_length, TemplateMap* templates);

#endif
