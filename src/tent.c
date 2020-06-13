#include <stdio.h>
#include <stdlib.h>

#include "file_utils.h"
#include "string_utils.h"
#include "template.h"

int main(int argc, char** argv) {
  FILE* file = fopen(argv[1], "r");
  if (!file) {
    return EXIT_FAILURE;
  }

  /* long buflen; */
  /* char* buf = read_file_into_buffer(file, &buflen); */

  replace_strings(file);

  fclose(file);

  return 0;
}
