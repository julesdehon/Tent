#include <stdio.h>
#include <stdlib.h>
#include <cmark.h>

#include "variable.h"
#include "string_utils.h"
#include "file_utils.h"
#include "cmark_parser.h"

#define MAX_METADATA_LENGTH (3000)
#define MAX_LINE_LENGTH (1024)
#define METADATA_DELIMITER ("---\n")

char *md_to_html(char *text_to_convert, int length) {
  char *res = malloc(100);
  strcpy(res, "Our markdown converter has not yet been implemented");
  return res;
}

char *parse_markdown(FILE *f, VariableMap *meta_map) {
  // Get file length for later use
  fseek(f, 0, SEEK_END);
  long int length = ftell(f);
  fseek(f, 0, SEEK_SET);
  
  // Ensure first line of md file is ---
  char line[MAX_LINE_LENGTH];
  fgets(line, sizeof(line), f);
  if (!str_equal(line, METADATA_DELIMITER)) {
    printf("Incorrect format in markdown file.  Missing metadata section after ---\n");
    exit(EXIT_FAILURE);
  }
  
  // get all text before next ---
  char meta_text[MAX_METADATA_LENGTH] = "\0";
  fgets(line, sizeof(line), f);
  while (!str_equal(line, METADATA_DELIMITER)) {
    strcat(meta_text, line);
    fgets(line, sizeof(line), f);
  }
  load_variable_map(meta_text, meta_map);

  // get all remaining text
  long int remaining_length = length - ftell(f);
  char remaining_text[remaining_length];
  fread(remaining_text, 1, remaining_length, f);

  // Parse remaining text
  char *content = convert_md_to_html(remaining_text, remaining_length);
  return content;
}
/* int main(void) { */
/*   VariableMap *meta_map = init_variable_map(); */
/*   FILE *f = fopen("markdown-eg.md", "r"); */
/*   char *content = parse_markdown(f, meta_map); */
/*   const char *key; */
/*   map_iter_t iter = map_iter(meta_map); */
/*   while ((key = map_next(meta_map, &iter))) { */
/*     printf("%s -> %s\n", key, (*map_get(meta_map, key))->value.str); */
/*   } */
/*   free_variable_map(meta_map); */
/*   printf("%s\n", content); */
/*   free(content); */
/*   return 0; */
/* } */
