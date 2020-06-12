#include "file_utils.h"
#include "string_utils.h"
#include <stdlib.h>
#include <stdbool.h>
// Remember to free the returned buffer if non null!
char* read_file_into_buffer(FILE* file, long* filelen) {
	fseek(file, 0, SEEK_END);
	*filelen = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* ret = calloc(*filelen, sizeof(char));
	fread(ret, *filelen, 1, file);
	return ret;
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
  bool matching_brackets = false;
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
    matching_brackets = false;
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
	  matching_brackets = true;
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
  if(fputs(final_text, fp) <= 0) {
    perror("Couldn't print to file!");
    exit(EXIT_FAILURE); 
  }
  fclose(fp);
  free(final_text);
  return;  
}	
