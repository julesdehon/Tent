#include "insert.h"
#include "string_utils.h"
#include "map.h"
#include "variable.h"
#include "template.h"

#include <stdlib.h>
#include <string.h>

void parse_insert_value(char* insert, char* type, char* name) {
  type = strtok(insert,".");
  name = strtok(NULL, ".");
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

void parse_args(char** args, Variable** args_pos, VariableMap* args_named) {

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

// TODO: Supporting positional and named arguments for inserts!
// You can either provide ALL positional arguments or ALL names arguments
// Positional:
// {{ snippet.fancy-message "Hello there" }}
// Named:
// {{ snippet.youtube url="https://youtube..." }}
// Both a Variable[] (positional) and a VariableMap (named) will be passed to the 
// function that populates the snippet, however at least one will always be NULL.
// (Both are null if no arguments are passed)
// Snippets / Templates can then access arguments using the syntax
// {{ args.0 }} for positional args where the number is the array index OR
// {{ args.url }} for named arguments

char* get_insert(char* insert, char* content, VariableMap* config,
	       	VariableMap* variables, VariableMap* args_named, Variable** args_pos,
          int var_arg_index, TemplateMap* templates) {
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
  parse_insert_value(tokens[0], insert_type, insert_name);

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
    
    // Arguments start from second token
    if (token_count > 1) {
      parse_args(tokens + 1, args_pos, args_named);
    }
    if(str_equal("template",insert_type)) {
      if(template->type == TT_PAGE) {
        // replace_inserts but with args
        return template->content;
      } else {
        printf("%s is not a template insert.\n", insert_name);
        exit(EXIT_FAILURE);
      }
    }

    if(str_equal("snippet",insert_type)) {
      if(template->type == TT_SNIPPET) {
        // replace_inserts but with args
        return template->content;
      } else {
        printf("%s is not a snippet insert.\n", insert_name);
        exit(EXIT_FAILURE);
      }
    }  
  }
  return  "111";
}	
