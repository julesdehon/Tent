#ifndef TEMPLATE_H
#define TEMPLATE_H

// Represents all of the possible template types we can support
// Underneath there is no difference but separating them allows
// the syntax for the templating to be nicer and also means the user
// has more naming possibilites
typedef enum TemplateType {
    PAGE,
    SNIPPET
} TemplateType;

typedef struct Template {
    char* name;
    TemplateType type;
    char* content;
} Template;

#endif
