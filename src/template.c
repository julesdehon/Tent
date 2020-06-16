#include "template.h"
#include "file_utils.h"
#include "string_utils.h"
#include "insert.h"
#include "map.h"

#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

#define PATH_LENGTH 128 //Shouldn't be longer than this
#define TEMPLATES_PATH "/theme/templates"
#define TEMPLATE_STR "template"
#define INSERT_OPEN "{{"
#define INSERT_CLOSE "}}"

Template* create_template() {
  Template* t = (Template*) malloc(sizeof(Template));
  return t;
}

void destroy_template(Template* t) {
  free(t->name);
  free(t->content);
  free(t);
}

char* replace_inserts(char* template, char* content, VariableMap* config,
    VariableMap* variables, VariableMap* args_named, Variable** args_pos,
    int var_arg_index, TemplateMap* templates) {

  // 1. Find {{
  // 2. Find }}
  // 3. Pass trimmed string inside to get_insert
  // 4. Use str_replace to replace {{ text }} with replaced string

  char* final_text = calloc(strlen(template) + 1, sizeof(char));
  strcpy(final_text, template);

  char* c_start;
  char* c_end;  
  int pos_start = 0;
  int pos_end = 0;

  while((c_start = strstr(final_text+pos_start, INSERT_OPEN))) {
    pos_start = c_start - final_text;
    if ((c_end = strstr(final_text+pos_start, INSERT_CLOSE))) {
      pos_end = c_end - final_text + strlen(INSERT_CLOSE); 
      char* orig = calloc(pos_end - pos_start + 1, sizeof(char));
      strncpy(orig, c_start, pos_end - pos_start);
      char* inside = calloc(strlen(orig) - strlen(INSERT_OPEN) - strlen(INSERT_CLOSE) + 1, 
          sizeof(char));
      strncpy(inside, orig + strlen(INSERT_OPEN), 
          strlen(orig) - strlen(INSERT_OPEN) - strlen(INSERT_CLOSE));
      char* replacement = get_insert(trim_whitespace(inside), content, config,
          variables, args_named, args_pos, var_arg_index, templates);
      char* new_text = str_replace(final_text, orig, replacement);
      free(final_text);
      final_text = new_text;
      free(orig);
      free(inside);
      free(replacement);
    }
  }

  return final_text;
}

Template* load_template(FILE* file, TemplateType type, char *filename) {
  Template* t = create_template();
  t->name = file_name_without_extension_from_string(filename);
  t->type = type;
  t->content = read_file_into_buffer(file, &t->content_len);
  return t;
}

void fill_template(char* content, VariableMap* config, VariableMap* variables,
    TemplateMap* templates, FILE* out) {
  Variable* tmp_var = (*map_get(variables, TEMPLATE_STR));
  // For now we only supprt VT_STRING
  Template* template = (*map_get(templates, tmp_var->value.str));
  if (template == NULL) {
    printf("There does not exist a template with name %s\n", 
        tmp_var->value.str);
    return;
  }
  char* complete = replace_inserts(template->content, content, config,
      variables, NULL, NULL, 0, templates);
  fputs(complete, out);
  free(complete);
}

TemplateType get_type(char* name) {//We will add them later
  if(str_equal(name,"pages")) {
    return TT_PAGE;  
  }	     
  if(str_equal(name,"snippets")) { //Can't use a switch for this
    return TT_SNIPPET;  
  }
  return TT_SNIPPET;  
}

TemplateMap* load_template_map() {
  TemplateMap* map = malloc(sizeof(TemplateMap));
  map_init(map); 

  DIR *dir,*subdir;
  struct dirent *ent,*sub_ent;
  char path_name[PATH_LENGTH];
  FILE* fp;

  char cwd[PATH_LENGTH];
  if (getcwd(cwd, sizeof(cwd)) == NULL) {
     perror("getcwd() error");
   }

  strcpy(path_name, cwd);
  strcat(path_name,TEMPLATES_PATH);
  strcat(path_name,"/");

  if(!(dir = opendir(path_name))) {
    perror("Could not open directory!");
    exit(EXIT_FAILURE);  
  }

  while ((ent = readdir(dir)) != NULL) {
    strcat(path_name,ent->d_name);
    if (ent->d_type != DT_DIR) {
      continue;
    }
    if(!(subdir = opendir(path_name))) {
      perror("Could not open directory!");
      exit(EXIT_FAILURE);  
    }
    strcat(path_name, "/");
    while ((sub_ent = readdir(subdir)) != NULL) {
      if (sub_ent->d_type != DT_REG) {
        continue;
      }
      strcat(path_name,sub_ent->d_name);
      if((fp = fopen(path_name,"r")) == NULL) {
        perror("Could not open directory!");
        exit(EXIT_FAILURE);  
      }

      Template* template = load_template(fp,get_type(ent->d_name), sub_ent->d_name);
      char *without_extension = file_name_without_extension_from_string(sub_ent->d_name);
      map_set(map,without_extension,template);
      strcpy(path_name + strlen(path_name) - strlen(sub_ent->d_name), path_name + strlen(path_name));
      fclose(fp);
    }
    closedir(subdir); 
    strcpy(path_name + strlen(path_name) - strlen(ent->d_name) - 1, path_name + strlen(path_name));
  }
  closedir(dir);
  return map;
}  
