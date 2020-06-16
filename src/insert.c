#include "insert.h"
#include "string_utils.h"
#include "map.h"
#include "variable.h"
#include "template.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void parse_insert_value(char* insert, char** type, char** name) {
  *type = strtok(insert, ".");
  *name = strtok(NULL, ".");
}	

char** parse_insert(char* insert, int* out_count) {
  char** ret = calloc(strlen(insert) + 1, sizeof(char*));

  int count = 0;
  char delims[] = {' '};
  char* cur = strtok(insert, delims);
  int total_length = 0;
  while (cur != NULL) {
    total_length += strlen(cur) + 1;
    ret[count] = cur;
    count++;
    cur = strtok(NULL, delims);
  }
  if (count == 0) {
    return NULL;
  }
  ret = realloc(ret, count * sizeof(char*));
  *out_count = count;
  return ret;
}

void parse_args(char** args, int arg_count, Variable*** args_pos, VariableMap** args_named) {
  int named = 0;
  int from_md = 0;
  if (strstr(args[0], "=&quot;") || strstr(args[0], "=\"")) {
    named = 1;
    if (!strstr(args[0], "=\"")) {
      from_md = 1;
    }
    *args_named = init_variable_map();
  } else {
    *args_pos = calloc(arg_count, sizeof(Variable*));
  }
  for (int i = 0; i < arg_count; i++) {
    if (named) {
      if (!strstr(args[0], "=&quot;")) {
        printf("Cannot mix named and positional arguments!");
      }
      char* name = strtok(args[i], "=");
      char* value = strtok(NULL, "");
      value = from_md ? str_replace(value, "&quot;", "") : str_replace(value, "\"", "");
      Variable* var = malloc(sizeof(Variable));
      var->type = VT_STRING;
      var->length = strlen(value);
      var->value.str = value;
      map_set(*args_named, name, var);
    } else {
      if (strstr(args[0], "=&quot;")) {
        printf("Cannot mix named and positional arguments!");
      }
      char* value = from_md ? str_replace(args[i], "&quot;", "") : str_replace(args[i], "\"", "");
      Variable* var = malloc(sizeof(Variable));
      var->type = VT_STRING;
      var->length = strlen(value);
      var->value.str = value;
      *args_pos[i] = var;
    }
  }
}

// TODO: Supporting VT_ARRAY variables
// Let's support another keyword other than content, call this range
// The following insert thingy after range MUST BE an array, followed immediately
// by a snippet or template. After that you can pass any number of other arguments.
// parse_insert will recognise range, and load the other optional variables.
// E.g.
// {{ range config.nav-names snippet.nav-link config.nav-paths }}
// In this case, for each value in config.nav-names, we will insert a nav-link snippet
// that has access to all normal config and metadata variables but we also explicity
// state that nav-paths is an argument (see below).
// As this is a range insert, we keep track of the current index of the iteration.
// We pass this index to the function that populates the snippet/template. If it comes
// across an insert of type VT_ARRAY it uses this insert to insert the correct value.

char* get_insert(char* insert, char* content, VariableMap* config,
	       	VariableMap* variables, VariableMap* args_named, Variable** args_pos,
          int var_arg_index, int args_length, TemplateMap* templates) {
  int token_count;
  char** tokens = parse_insert(insert, &token_count);
  
  // Content token
  if (str_equal("content", tokens[0])) {
    return content;
  }

  // Insert snippet/template in tokens[2] for each value in VT_ARRAY tokens[1]
  // TODO:
  // check token types, parse args
  // loop through token[1]
  // create string by repeatedly appending the result from replace_inserts
  if (str_equal("range", tokens[0])) {

  }

  // VARIABLE ONLY
  char* insert_type = NULL;
  char* insert_name = NULL;
  parse_insert_value(tokens[0], &insert_type, &insert_name);

  if (insert_type == NULL || insert_name == NULL) {
    printf("Could not parse insert: %s\n", tokens[0]);
    exit(EXIT_FAILURE);
  }

  if (str_equal("args", insert_type)) {
    if (isdigit(insert_name[0])) {
      int index = atoi(insert_name);
      if (index < 0 || index >= args_length) {
        printf("Index %d out of bounds for positional arguments\n", index);
        exit(EXIT_FAILURE);
      } 
      if (!args_pos) {
        printf("No positional arguments exist!\n");
        exit(EXIT_FAILURE);
      }
      return args_pos[index]->value.str;
    } else {
      if (!args_named) {
        printf("No named arguments exist!\n");
        exit(EXIT_FAILURE);
      }
      Variable* var = (*map_get(args_named, insert_name));
      if (!var) {
        printf("Named argument %s does not exist!\n", insert_name);
        exit(EXIT_FAILURE);
      }
      return var->value.str;
    }
  }

  Variable* variable;
  if(str_equal("config",insert_type)) {
    Variable** var_ptr = map_get(config, insert_name);
    if (!var_ptr) {
      printf("Missing config: %s\n", insert_name);
    } else {
      variable = *var_ptr;
      if(variable->type == VT_ARRAY) {
        perror("Not implemented yet!");
        exit(EXIT_FAILURE);
      }
      return variable->value.str; 
    }
  }

  if(str_equal("variable",insert_type)) {
    Variable** var_ptr = map_get(variables, insert_name);
    if (!var_ptr) {
      printf("Missing variable: %s\n", insert_name);
    } else {
      variable = *var_ptr;
      if(variable->type == VT_ARRAY) {
        perror("Not implemented yet!");
        exit(EXIT_FAILURE);
      }
      return variable->value.str; 
    }
  }

  // TEMPLATE
  // THESE CAN HAVE ARGUMENTS!

  Template** tmp_ptr = map_get(templates, insert_name);
  if (!tmp_ptr) {
    printf("Missing template: %s\n", insert_name);
  } else {
    Template* template = *tmp_ptr;
    Variable** args_pos = NULL;
    VariableMap* args_named = NULL;
    int arg_count = token_count - 1;
    
    // Arguments start from second token
    if (token_count > 1) {
      parse_args(tokens + 1, arg_count, &args_pos, &args_named);
    }
    if(str_equal("template",insert_type)) {
      if(template->type == TT_PAGE) {
        return replace_inserts(template->content, NULL, config, variables,
            args_named, args_pos, 0, arg_count, templates);
      } else {
        printf("%s is not a template insert.\n", insert_name);
        exit(EXIT_FAILURE);
      }
    }

    if(str_equal("snippet",insert_type)) {
      if(template->type == TT_SNIPPET) {
        return replace_inserts(template->content, NULL, config, variables,
            args_named, args_pos, 0, arg_count, templates);
      } else {
        printf("%s is not a snippet insert.\n", insert_name);
        exit(EXIT_FAILURE);
      }
    }  
  }
  printf("Unrecognized insert type %s\n", insert_type);
  exit(EXIT_FAILURE);
}	
