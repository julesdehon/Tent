#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "file_utils.h"
#include "string_utils.h"
#include "template.h"
#include "map.h"
#include "cmark_parser.h"
int main(int argc, char** argv) {
  /* FILE* file = fopen(argv[1], "r"); */
  /* if (!file) { */
  /*   return EXIT_FAILURE; */
  /* } */

  TemplateMap* tm = load_template_map();
  Template* home = (*map_get(tm, "home"));
  printf("%s", replace_inserts(home->content, NULL, NULL, NULL, NULL));
  map_deinit(tm);

  free(tm);

  return 0;
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
