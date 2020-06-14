# include <stdint.h>

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

typedef enum MarkupType{
  Header,
  Bold,
  Italic,
  Link,
  Paragraph
} MarkupType;

typedef struct Tags{
  char* tag;
  int number;
} Tags;

typedef struct Line{
  MarkupType markup;
  char* content;
  Tags tag;
} Line;


#endif
