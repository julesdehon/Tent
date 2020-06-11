#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "map.h"

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

Template* load_template(FILE* file, TemplateType type);

#endif
