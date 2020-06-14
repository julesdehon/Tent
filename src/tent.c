#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "file_utils.h"
#include "string_utils.h"
#include "template.h"
#include "map.h"

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
