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

  const char* key;
  TemplateMap* tm = load_template_map();
  map_iter_t iter = map_iter(config_map);
  while ((key = map_next(tm, &iter))) {
    printf("%s -> %s\n", key, (*map_get(tm, key))->content);
  }
  map_deinit(tm);

  free(tm);


  /* fclose(file); */

  return 0;
}
