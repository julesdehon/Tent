#include "template.h"
#include "file_utils.h"
#include "string_utils.h"
#include "map.h"

#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

#define PATH_LENGTH 128 //Shouldn't be longer than this
#define TEMPLATES_PATH "/theme/templates"

Template* create_template() {
  Template* t = (Template*) malloc(sizeof(Template));
  return t;
}

void destroy_template(Template* t) {
  free(t->name);
  free(t->content);
  free(t);
}

Template* load_template(FILE* file, TemplateType type) {
  Template* t = create_template();
  t->name = file_name_without_extension(file);
  t->type = type;
  t->content = read_file_into_buffer(file, &t->content_len);
  return t;
}

void replace_strings(FILE* fp) {
  fseek(fp, 0, SEEK_END);
  long long file_length = ftell(fp);
  fseek(fp,0,SEEK_SET);
  char* final_text = malloc(file_length + 1);
  char* replaced;
  //fread(final_text,file_length,1,fp);
  char* with,c,d;
  int length = 0,length_replace = 0;
  int closing_brackets = 0, opening_brackets = 0;
  for(int i = 0;i < file_length;) {
    opening_brackets = 0;	  //iterating through every character in the file
    while(i < file_length) { //we read from the file  and update our final_text until we have 2 consecutive {{ or until the file ends
      c = fgetc(fp);
      i++;
      if(c == '{') {
        if(i >= file_length) {
          final_text[length++] = c;
          break;
        }	      
        d = fgetc(fp);
        i++;
        if(d == '{') {
	  opening_brackets = 1;	
          break;  
        } else {
          final_text[length++] = c;
          final_text[length++] = d;
        }
      } else {
        final_text[length++] = c;
      }      
    }
    closing_brackets = 0;
    length_replace = 0;
    replaced = malloc(file_length + 1);
    while(i < file_length) { // we read until we hit the closing brackets or the file ends 
      c = fgetc(fp);
      i++;
      if(c == '}') {
        if(i >= file_length) {
          replaced[length_replace++] = c; // we store the string inside the brackets in replaced
          break;
        }	      
        d = fgetc(fp);
        i++;
        if(d == '}') {
          closing_brackets = 1;
          break;
        } else {
          replaced[length_replace++] = c;
          replaced[length_replace++] = d;
        }	
      } else {
        replaced[length_replace++] = c;
      }      
    }
    if(opening_brackets && closing_brackets) {//if we encountered the closing brackets we replace the string inside with ..
      with = malloc(length_replace+1);
      //with = replace_with(replaced);
      with = "aaa\nbbb";

      for(int j = 0;j < 7/*length_replace*/;j++) {
        final_text[length++] = with[j];    
      }
      //free(with);    
    } else if(opening_brackets && !closing_brackets) { // otherwise we just write the text that was there before
      final_text[length++] = '{';
      final_text[length++] = '{';
      for(int j = 0;j < length_replace;j++) {
        final_text[length++] = replaced[j];
      }
    }
    free(replaced);  
  }
    
  char* fpath = file_path(fp);
  fclose(fp);
  if((fp = fopen(fpath, "w+")) == NULL) {
    perror("Couldn't print to file!");
    exit(EXIT_FAILURE); 
  }
  if(fputs(final_text, fp) <= 0) {
    perror("Couldn't print to file!");
    exit(EXIT_FAILURE); 
  }
  free(final_text);
  return;  
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
      Template* template = load_template(fp,get_type(ent->d_name));
      map_set(map,file_name_without_extension(fp),template);
      strcpy(path_name + strlen(path_name) - strlen(sub_ent->d_name), path_name + strlen(path_name));
      fclose(fp);
    }
    closedir(subdir); 
    strcpy(path_name + strlen(path_name) - strlen(ent->d_name) - 1, path_name + strlen(path_name));
  }
  closedir(dir);
  return map;
}  
