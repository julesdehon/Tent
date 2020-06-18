#include <stdlib.h>
#include <string.h>
#include <test.h>
#include <string.h>
#include <stdio.h>

#include "../cmark_parser.h"
#include "../variable.h"
#include "../string_utils.h"

#define NUM_VARIABLES 5
int test_markdown_parser(char *testname) {
  char* keys[NUM_VARIABLES] = {"foo", "ping", "hello", "pop", "percy"};
  char* values[NUM_VARIABLES] = {"bar", "pong", "world", "corn", "pig"};  
  strcpy(testname, "cmark_parser.c - Test Markdown File Parser");
  VariableMap *meta_map = init_variable_map();
  FILE *eg_markdown_file = fopen("eg_markdown_file.md", "r");
  if (!eg_markdown_file) {
    perror("Could not open example markdown file");
    exit(EXIT_FAILURE);
  }
  char *parsed_content = parse_markdown(eg_markdown_file, meta_map);
  for (int i = 0; i < NUM_VARIABLES; i++) {
    Variable **var_ptr = map_get(meta_map, keys[i]);
    if (!var_ptr)
      return 0;
    if (!str_equal((*var_ptr)->value.str, values[i]))
      return 0;
  }
  if (!parsed_content)
    return 0;
  if (!strstr(parsed_content, "<h1>"))
    return 0;
  if (!strstr(parsed_content, "<strong>"))
    return 0;
  if (!strstr(parsed_content, "<em>"))
    return 0;
  free_variable_map(meta_map);
  return 1;
}

int test_determine_vartype(char *testname) {
  strcpy(testname, "variable.c - Test Determine Vartype");
  if (determine_vartype("clearly a string") != VT_STRING)
    return 0;
  if (determine_vartype("[Is this a string or an array?") != VT_STRING)
    return 0;
  if (determine_vartype("String with [array] inside") != VT_STRING)
    return 0;
  if (determine_vartype("[clearly, an, array]") != VT_ARRAY)
    return 0;
  if (determine_vartype("[Singleton]") != VT_ARRAY)
    return 0;
  return 1;
}

int test_init_variable_map(char *testname) {
  strcpy(testname, "variable.c - Test Init Variable Map");
  VariableMap *var_map = init_variable_map();
  if (!var_map)
    return 0;
  map_iter_t iter = map_iter(var_map);
  const char *key;
  if ((key = map_next(var_map, &iter)))
    return 0;
  return 1;
}

#define NUM_STRINGS 5
#define NUM_FAKES 5
#define NUM_ARRAYS 2
#define KEY_VALS 
int test_load_variable_map(char *testname) {
  char key_vals[1024] = "foo:to the bar\n"\
                        "ping:   pong   \n"\
                        "hello: darkness my old friend   \n"\
                        "pop:corn\n"\
                        "percy: pig\n"\
                        "favourite colours: [red  ,   green, blue,white]\n"\
                        "condiments: [  mustard, mayo, barbecue sauce  ]\n";
  char* str_keys[NUM_STRINGS] = {"foo", "ping", "hello", "pop", "percy"};
  char* str_values[NUM_STRINGS] = {"to the bar", "pong", "darkness my old friend", "corn", "pig"};
  char* not_keys[NUM_FAKES] = {"instrument", "f", "bar:", "corn", "perc"};
  char* arr_keys[NUM_ARRAYS] = {"favourite colours", "condiments"};
  char* arr_values[NUM_ARRAYS] = {"redgreenbluewhite", "mustardmayobarbecue sauce"};
  
  strcpy(testname, "variable.c - Test Load Variable Map");
  VariableMap *var_map = init_variable_map();
  load_variable_map(key_vals, var_map);
  for (int i = 0; i < NUM_STRINGS; i++) {
    Variable **var_ptr = map_get(var_map, str_keys[i]);
    if (!var_ptr)
      return 0;
    Variable *var = *var_ptr;
    if (var->type != VT_STRING)
      return 0;
    if (!str_equal(var->value.str, str_values[i]))
      return 0;
  }
  for (int i = 0; i < NUM_FAKES; i++) {
    Variable **var_ptr = map_get(var_map, not_keys[i]);
    if (var_ptr)
      return 0;
  }
  for (int i = 0; i < NUM_ARRAYS; i++) {
    Variable **var_ptr = map_get(var_map, arr_keys[i]);
    if (!var_ptr)
      return 0;
    Variable *var = *var_ptr;
    if (var->type != VT_ARRAY)
      return 0;
    char buf[100] = "";
    for (int j = 0; j < var->length; j++)
      strncat(buf, var->value.arr[j], 100 * sizeof(char));
    if (!str_equal(buf, arr_values[i]))
      return 0;
  }
  free_variable_map(var_map);
  return 1;
}

int main(void) {
  int (*tests[])(char*) = {
    test_markdown_parser,
    test_determine_vartype,
    test_init_variable_map,
    test_load_variable_map
  };

  run_test_suite(tests, 4, __FILE__);

  return 0;
}
