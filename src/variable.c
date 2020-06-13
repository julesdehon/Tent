#include "variable.h"
#include "string_utils.h"
#include "file_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

VariableType determine_vartype(char* value) {
  return VT_STRING;
}

// Takes a string of key: value pairs, for example
// "title: first blog post\nauthor: Jules Dehon\ndescription: a sample post"
// and returns a VariableMap containing each key and
// its corresponding value
VariableMap *load_variable_map(char *key_val_pairs) {
  VariableMap *var_map = malloc(sizeof(VariableMap));
  if (!var_map) {
    perror("Malloc error in load_variable_map");
    exit(EXIT_FAILURE);
  }
  map_init(var_map);
  
  char **saveptr = &key_val_pairs;
  char *line = strtok_r(key_val_pairs, "\n", saveptr);
  while (line) {
    char* key = strtok_r(line, ":", &line);
    char* value = strtok_r(NULL, "\n", &line);
    Variable *var = malloc(sizeof(Variable));
    if (!var) {
      perror("Malloc error in load_variable_map");
      exit(EXIT_FAILURE);
    }
    var->type = determine_vartype(value);
    if (var->type == VT_STRING) {
      var->value.str = trim_whitespace(value);
    } else {
      printf("Array variable types not yet supported\n");
      var->value.str = trim_whitespace(value);
    }
    map_set(var_map, key, var);
    line = strtok_r(NULL, "\n", saveptr);
  }
  return var_map;
}

// Necessary because when loading variable maps, Variable is allocated on the heap
void free_variable_map(VariableMap *var_map) {
  map_iter_t iter = map_iter(var_map);
  const char *key;
  while ((key = map_next(var_map, &iter))) {
    free(*map_get(var_map, key));
  }
  map_deinit(var_map);
  free(var_map);
}

// Takes a string path to the config file,
// opens it, and returns a VariableMap
// containing all the key-value pairs in the config file
// If any of the required keys - listed below - are
// missing, throws an error
const int num_required_keys = 2;
const char* required_keys[2] = {"site_title", "author"};
VariableMap *load_config(char *file_path) {
  FILE *f = fopen(file_path, "r");
  if (!f) {
    perror("variable.c: error reading config file");
    exit(EXIT_FAILURE);
  }
  long filelen;
  char *buffer = read_file_into_buffer(f, &filelen);
  VariableMap *config_map = load_variable_map(buffer);
  for (int i = 0; i < num_required_keys; i++) {
    if (!map_get(config_map, required_keys[i])) {
      fprintf(stderr, "Config file missing required key, %s\n", required_keys[i]);
      exit(EXIT_FAILURE);
    }
  }
  return config_map;
}

//Uncomment to test functionality.
/* int main(void) { */
/*   char key_val_pairs[] = "title: first blog post\nauthor: Jules Dehon\ndescription: a sample post"; */
/*   VariableMap *var_map = load_variable_map(key_val_pairs); */
/*   const char *key; */
/*   map_iter_t iter = map_iter(var_map); */
/*   while ((key = map_next(var_map, &iter))) { */
/*     printf("%s -> %s\n", key, (*map_get(var_map, key))->value.str); */
/*   } */
/*   free_variable_map(var_map); */
/*   return 0; */
/* } */
/* int main(void) { */
/*   VariableMap *config_map = load_config("config.tent"); */
/*   const char *key; */
/*   map_iter_t iter = map_iter(config_map); */
/*   while ((key = map_next(config_map, &iter))) { */
/*     printf("%s -> %s\n", key, (*map_get(config_map, key))->value.str); */
/*   } */
/*   free_variable_map(config_map); */
/*   return 0; */
/* } */
