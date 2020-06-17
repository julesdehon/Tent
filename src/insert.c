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

// TODO: Allow existing variables to be passed as positional and named arguments
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
      if (!strstr(args[i], "=&quot;")) {
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
      if (strstr(args[i], "=&quot;")) {
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

char* get_insert(char* insert, char* content, VariableMap* config,
	       	VariableMap* variables, VariableMap* args_named, Variable** args_pos,
          int var_arg_index, int args_length, TemplateMap* templates) {
  int token_count;
  char** tokens = parse_insert(insert, &token_count);
  
  Variable** args_p = NULL;
  VariableMap* args_n = NULL;

  // Content token
  if (str_equal("content", tokens[0])) {
    return content;
  }

  if (str_equal("range", tokens[0])) {
    Variable* arr = NULL;

    if (token_count < 3) {
      printf("Formatting of range command incorrect.\n");
      exit(EXIT_FAILURE);
    }
    char* arr_type = NULL;
    char* arr_name = NULL;
    parse_insert_value(tokens[1], &arr_type, &arr_name);
    int arg_c = token_count - 3;
    if (arr_type == NULL || arr_name == NULL) {
      printf("Formatting of range command incorrect.\n");
      exit(EXIT_FAILURE);
    }

    if (token_count > 3) {
      parse_args(tokens+3, arg_c, &args_p, &args_n);
    }

    if (str_equal(arr_type, "config")) {
      arr = *(map_get(config, arr_name));
    } else if (str_equal(arr_type, "variable")) {
      arr = *(map_get(variables, arr_name));
    } else {
      printf("First argument to range command must be a variable.\n");
      exit(EXIT_FAILURE);
    }

    if (arr == NULL) {
      printf("Variable %s does not exist.\n", arr_name);
      exit(EXIT_FAILURE);
    }

    if (arr->type != VT_ARRAY) {
      printf("First argument to range command must be an array.\n");
      exit(EXIT_FAILURE);
    }

    char* snip_type;
    char* snip_name;
    Template* arr_temp; 
    parse_insert_value(tokens[2], &snip_type, &snip_name);
    if (!str_equal(snip_type, "snippet")) {
      printf("Range only supports snippets.\n");
      exit(EXIT_FAILURE);
    }
    arr_temp = *(map_get(templates, snip_name));
    if (arr_temp == NULL) {
      printf("Snippet named %s doesn't exist.\n", snip_name);
      exit(EXIT_FAILURE);
    }
    if (arr_temp->type != TT_SNIPPET) {
      printf("Range only supports snippets.\n");
      exit(EXIT_FAILURE);
    }

    char* ret = replace_inserts(arr_temp->content, NULL, config, variables,
        args_n, args_p, 0, arg_c, templates);
    printf("i = %d\nret = %s\n", 0, ret);
    for (int i = 1; i < arr->length; i++) { 
      char* replaced = replace_inserts(arr_temp->content, NULL, config, variables,
        args_n, args_p, i, arg_c, templates);
      char* tmp = calloc(strlen(ret) + strlen(replaced) + 1, sizeof(char));
      strcpy(tmp, ret);
      strcat(tmp, replaced);
      free(ret);
      ret = tmp;
    }

    return ret;

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
      if (var->type == VT_ARRAY) {
        return var->value.arr[var_arg_index];
      } else {
        return var->value.str;
      }
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
        return variable->value.arr[var_arg_index];
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
        return variable->value.arr[var_arg_index];
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
    int arg_c = token_count - 1;
    
    // Arguments start from second token
    if (token_count > 1) {
      parse_args(tokens + 1, arg_c, &args_p, &args_n);
    }
    if(str_equal("template",insert_type)) {
      if(template->type == TT_PAGE) {
        return replace_inserts(template->content, NULL, config, variables,
            args_n, args_p, 0, arg_c, templates);
      } else {
        printf("%s is not a template insert.\n", insert_name);
        exit(EXIT_FAILURE);
      }
    }

    if(str_equal("snippet",insert_type)) {
      if(template->type == TT_SNIPPET) {
        return replace_inserts(template->content, NULL, config, variables,
            args_n, args_p, 0, arg_c, templates);
      } else {
        printf("%s is not a snippet insert.\n", insert_name);
        exit(EXIT_FAILURE);
      }
    }  
  }
  printf("Unrecognized insert type %s\n", insert_type);
  exit(EXIT_FAILURE);
}	
