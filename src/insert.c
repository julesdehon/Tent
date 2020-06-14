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

  char* insert_name = parse_insert(insert);
  char* insert_type = insert;
  if(str_equal("content",insert_type)) {
    return content;
  }
  Variable* variable;
  if(str_equal("config",insert_type)) {
    variable = (*map_get(config,insert_name));
    if(variable->type == VT_ARRAY) {
      perror("Not implemented yet!");
      exit(EXIT_FAILURE);
    }
    return variable->value.str; 
  }

  if(str_equal("variable",insert_type)) {
    variable = (*map_get(variables,insert_name));
    if(variable->type == VT_ARRAY) {
      perror("Not implemented yet!");
      exit(EXIT_FAILURE);
    }
    return variable->value.str; 
  }

  Template* template = (*map_get(templates,insert_name));
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
  return  "111";
}	
