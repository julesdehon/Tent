#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "map.h"

// Represents all of the possible template types we can support
// Underneath there is no difference but separating them allows
// the syntax for the templating to be nicer and also means the user
// has more naming possibilites
typedef enum TemplateType {
  PAGE,
  PARTIAL,
  CSS,
  JS,
  SNIPPET
} TemplateType;

// The template struct, which will be used by the templater to replace
// {{ type.name }} with the relevant text content
typedef struct Template {
  char* name;
  TemplateType type;
  char* content;
} Template;

typedef map_t(Template*) TemplateMap; 

#endif
