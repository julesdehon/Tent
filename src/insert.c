#include "insert.h"
#include "string_utils.h"
#include "map.h"
#include "variable.h"
#include "template.h"

#include <stdlib.h>
#include <string.h>

char* parse_insert(char* insert) {
  strtok(insert,".");
  if(!str_equal("content",insert)) {
    return strtok(NULL,".");  
  }
  return "content";    	  
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
	       	VariableMap* variables, TemplateMap* templates) {
  char* insert_name = parse_insert(insert);
  char* insert_type = insert;
  if(str_equal("content",insert_type)) {
    return content;
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

  Template** tmp_ptr = map_get(templates, insert_name);
  if (!tmp_ptr) {
    printf("Missing template: %s\n", insert_name);
  } else {
  Template* template = *tmp_ptr;
    if(str_equal("template",insert_type)) {
      if(template->type == TT_PAGE) {
        return template->content;
      }  
    }

    if(str_equal("snippet",insert_type)) {
      if(template->type == TT_SNIPPET) {
        return template->content;
      }  
    }  
  }
  return  "111";
}	
