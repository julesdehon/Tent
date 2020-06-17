#include "variable.h"
#include "string_utils.h"
#include "file_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LIST_ITEMS (500)

VariableType determine_vartype(char* value) {
  if (value[0] == '[' && value[strlen(value) - 1] == ']') {
    return VT_ARRAY;
  }
  return VT_STRING;
}

VariableMap *init_variable_map(void) {
  VariableMap *var_map = malloc(sizeof(VariableMap));
  if (!var_map) {
    perror("Malloc error in load_variable_map");
    exit(EXIT_FAILURE);
  }
  map_init(var_map);
  return var_map;
}

// Takes a string of key: value pairs, for example
// "title: first blog post\nauthor: Jules Dehon\ndescription: a sample post"
// and returns a VariableMap containing each key and
// its corresponding value
void load_variable_map(char *key_val_pairs, VariableMap *var_map) {
  char **saveptr = &key_val_pairs;
  char *line = strtok_r(key_val_pairs, "\n", saveptr);
  while (line) {
    char* key = strtok_r(line, ":", &line);
    char* value = strtok_r(NULL, "\n", &line);
    if (!key || !value) {
      printf("Incorrectly formatted line.\n");
      break;
    }
    Variable *var = malloc(sizeof(Variable));
    if (!var) {
      perror("Malloc error in load_variable_map");
      exit(EXIT_FAILURE);
    }
    value = trim_whitespace(value);
    var->type = determine_vartype(value);
    if (var->type == VT_STRING) {
      var->value.str = malloc(strlen(value) + 1);
      strcpy(var->value.str, value);
      var->length = strlen(value);
    } else {
      value++;
      value[strlen(value) - 1] = '\0';
      char *temp_array[MAX_LIST_ITEMS];
      char *current = strtok(value, ",");
      int counter = 0;
      while (current) {
	if (counter >= MAX_LIST_ITEMS) {
	  printf("Cannot have more than %d list items in a variable, taking first %d.\n", MAX_LIST_ITEMS, MAX_LIST_ITEMS);
	  break;
	}
	temp_array[counter] = malloc((strlen(current) + 1) * sizeof(char));
	strcpy(temp_array[counter], trim_whitespace(current));
	counter++;
	current = strtok(NULL, ",");
      }
      var->length = counter;
      var->value.arr = malloc((var->length) * sizeof(char *));
      memcpy(var->value.arr, temp_array, (var->length) * sizeof(char *));
    }
    map_set(var_map, key, var);
    line = strtok_r(NULL, "\n", saveptr);
  }
}

// Necessary because when loading variable maps, Variable is allocated on the heap
void free_variable_map(VariableMap *var_map) {
  map_iter_t iter = map_iter(var_map);
  const char *key;
  while ((key = map_next(var_map, &iter))) {
    Variable *var = *map_get(var_map, key);
    if (var->type == VT_STRING) {
      free(var->value.str);
    } else if (var->type == VT_ARRAY) {
      for (int i = 0; i < var->length; i++) {
	free(var->value.arr[i]);
      }
      free(var->value.arr);
    }
    free(var);
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
const char* required_keys[2] = {"site-title", "site-url"};
void load_config(char *file_path, VariableMap *config_map) {
  FILE *f = fopen(file_path, "r");
  if (!f) {
    perror("variable.c: error reading config file");
    exit(EXIT_FAILURE);
  }
  long filelen;
  char *buffer = read_file_into_buffer(f, &filelen);
  load_variable_map(buffer, config_map);
  for (int i = 0; i < num_required_keys; i++) {
    if (!map_get(config_map, required_keys[i])) {
      fprintf(stderr, "Config file missing required key, %s\n", required_keys[i]);
      exit(EXIT_FAILURE);
    }
  }
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
/*   VariableMap *config_map = init_variable_map(); */
/*   load_config("config.tent", config_map); */
/*   const char *key; */
/*   map_iter_t iter = map_iter(config_map); */
/*   while ((key = map_next(config_map, &iter))) { */
/*     Variable *var = *map_get(config_map, key); */
/*     if (var->type == VT_STRING) */
/*       printf("%s -> %s\n", key, var->value.str); */
/*     if (var->type == VT_ARRAY) { */
/*       for (int i = 0; i < var->length; i++) */
/* 	printf("%s[%d] -> %s\n", key, i, var->value.arr[i]); */
/*     } */
/*   } */
/*   free_variable_map(config_map); */
/*   return 0; */
/* } */
