#include "template.h"
#include "file_utils.h"

#include <string.h>
#include <stdlib.h>

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
  int matching_brackets = 0;
  for(int i = 0;i < file_length;) { //iterating through every character in the file
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
					break;    
				} else {
					final_text[length++] = c;
					final_text[length++] = d;
				}	
			} else {
				final_text[length++] = c;
			}      
    }
    matching_brackets = 0;
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
					matching_brackets = 1;
					break;    
				} else {
					replaced[length_replace++] = c;
					replaced[length_replace++] = d;
				}	
      } else {
				replaced[length_replace++] = c;
      }      
    }
    if(matching_brackets) {//if we encountered the closing brackets we replace the string inside with ..
      with = malloc(length_replace+1);
      //with = replace_with(replaced);
      with = "aaaaa";

      for(int j = 0;j < 5/*length_replace*/;j++) {
				final_text[length++] = with[j];    
      }
      //free(with);    
    } else { // otherwise we just write the text that was there before
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
