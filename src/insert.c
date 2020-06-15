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

char* get_insert(char* insert, char* content, VariableMap* config,
	       	VariableMap* variables, TemplateMap* templates) {

  const char *key;
  map_iter_t iter = map_iter(variables);

  printf("\nINSERT - %s\n", insert);
  while ((key = map_next(variables, &iter))) {
    printf("%s -> %s\n", key, (*map_get(variables, key))->value.str);
  }

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
    Variable** var_ptr = map_get(config, insert_name);
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
