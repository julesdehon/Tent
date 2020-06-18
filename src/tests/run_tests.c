#include <stdlib.h>
#include <string.h>
#include <test.h>
#include <string.h>
#include <stdio.h>
#include <log.h>

#include "../cmark_parser.h"
#include "../variable.h"
#include "../string_utils.h"
#include "../file_utils.h"

#define MAX_STRING 1024

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
  char key_vals[MAX_STRING] = "foo:to the bar\n"\
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
    char buf[MAX_STRING] = "";
    for (int j = 0; j < var->length; j++)
      strncat(buf, var->value.arr[j], 100 * sizeof(char));
    if (!str_equal(buf, arr_values[i]))
      return 0;
  }
  free_variable_map(var_map);
  return 1;
}

#define NUM_REPLACEABLE 3
int test_str_replace(char *testname) {
  strcpy(testname, "string_utils.c - Test String Replace");
  char *originals[NUM_REPLACEABLE] = {
    "Java is my favourite programming language. I love to program in Java!",
    "www.youtube.com",
    "ThisMightBeAnEdgeCase"
  };
  char *rep[NUM_REPLACEABLE] = {
    "Java",
    "youtube",
    "might"
  };
  char *with[NUM_REPLACEABLE] = {
    "C... and python too,",
    "facebook",
    "IsDefinitely"
  };
  char *expected[NUM_REPLACEABLE] = {
    "C... and python too, is my favourite programming language. I love to program in C... and python too,!",
    "www.facebook.com",
    "ThisMightBeAnEdgeCase"
  };
  for (int i = 0; i < NUM_REPLACEABLE; i++) {
    char *replaced = str_replace(originals[i], rep[i], with[i]);
    if (!replaced)
      return 0;
    if (!str_equal(replaced, expected[i]))
      return 0;
    free(replaced);
  }
  return 1;
}

#define NUM_TRIMMABLE 3
int test_trim_whitespace(char *testname) {
  strcpy(testname, "string_utils.c - Test Trim Whitespace");
  char originals[NUM_TRIMMABLE][MAX_STRING] = {
    "\t \nThis test has a tab and newline at the start      ",
    " \r  and this one has some form-feed and carriage return   \v  \f",
    "Finally this one is just normal!"
  };
  char *expected[NUM_TRIMMABLE] = {
    "This test has a tab and newline at the start",
    "and this one has some form-feed and carriage return",
    "Finally this one is just normal!"
  };
  for (int i = 0; i < NUM_TRIMMABLE; i++) {
    char *trimmed = trim_whitespace(originals[i]);
    if (!trimmed)
      return 0;
    if (!str_equal(trimmed, expected[i])) {
      log_debug("Got: '%s', expected: '%s'", trimmed, expected[i]);
      return 0;
    }
  }
  return 1;
}

#define NUM_APPENDABLE 2
int test_str_append(char *testname) {
  strcpy(testname, "string_utils.c - Test Str Append");
  char *originals[NUM_APPENDABLE] = {
    "I really like",
    "This is one sentence. "
  };
  char *to_append[NUM_APPENDABLE] = {
    " cheese!",
    "and this is another sentence!"
  };
  char *expected[NUM_APPENDABLE] = {
    "I really like cheese!",
    "This is one sentence. and this is another sentence!"
  };
  for (int i = 0; i < NUM_APPENDABLE; i++) {
    char *appended = str_append(originals[i], to_append[i]);
    if (!appended)
      return 0;
    if (!str_equal(appended, expected[i])) {
      log_debug("Got: '%s', expected: '%s'", appended, expected[i]);
      return 0;
    }
  }
  return 1;  
}

#define EG_MD_FILE_TEXT "---\n"\
"foo:\tbar\n"\
"ping:pong\n"\
"hello:  world\n"\
"pop: corn\n"\
"percy:pig\n"\
"---\n"\
"# This is some example text\n"\
"\n"\
"I am **trying** to see if the _parse\\_markdown_ functions are working!\n"
int test_read_file_into_buffer(char *testname) {
  strcpy(testname, "file_utils.c - Test Read File Into Buffer");
  FILE *f = fopen("eg_markdown_file.md", "r");
  if (!f)
    return 0;
  long filelen;
  char *buffer = read_file_into_buffer(f, &filelen);
  if (!str_equal(EG_MD_FILE_TEXT, buffer)) {
    log_debug("Got: '%s', expected: '%s'", buffer, EG_MD_FILE_TEXT);
    return 0;
  }
  return 1;
}

#define NUM_EG_FILENAMES 5
int test_file_name_processing(char *testname) {
  strcpy(testname, "file_utils.c - Test Filename Processing Functions");
  char *eg_filenames[NUM_EG_FILENAMES] = {
    "tent.c",
    "homepage.md",
    "filename with spaces.txt",
    "filename.with.multiple.dots",
    "no-filename"
  };
  char *eg_names[NUM_EG_FILENAMES] = {
    "tent",
    "homepage",
    "filename with spaces",
    "filename.with.multiple",
    "no-filename"
  };
  char *eg_extensions[NUM_EG_FILENAMES] = {
    "c",
    "md",
    "txt",
    "dots",
    ""
  };
  for (int i = 0; i < NUM_EG_FILENAMES; i++) {
    char *name = file_name_without_extension_from_string(eg_filenames[i]);
    char *extension = file_extension_from_string(eg_filenames[i]);
    if (!str_equal(name, eg_names[i]))
      return 0;
    if (!str_equal(extension, eg_extensions[i]))
      return 0;
  }
  return 1;
}

int main(void) {
  int (*tests[])(char*) = {
    test_markdown_parser,
    test_determine_vartype,
    test_init_variable_map,
    test_load_variable_map,
    test_str_replace,
    test_trim_whitespace,
    test_str_append,
    test_read_file_into_buffer,
    test_file_name_processing
  };

  run_test_suite(tests, 9, __FILE__);

  return 0;
}
