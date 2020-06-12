#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "file_utils.h"
#include "convert.h"


void parse_markdown(FILE *input, FILE *output){

  long bufflen;
  char* source = read_file_into_buffer(input, &bufflen);

  char *line;
  //go through line by line
  line = strtok_r(source, "\n", &source);
  while(line != NULL){
    int token_count;
    char** tokens = tokenize(line, &line, &token_count);
  }    
}

//same tokenize function as the one in assembler
//breaks down the line given to tokens that are separated
//only by ' ' this time.
char** tokenize(char* line, char** saveptr, int* out_count){
  char** ret = calloc(strlen(line) + 1, sizeof(char*));

  int count = 0;
  char delims[] = {' '};
  char* curr = strtok_r(line, delims, saveptr);
  int totalLength = 0;

  while(curr){
    totalLength += strlen(curr) + 1;
    ret[count] = curr;
    count++;
    curr = strtok_r(NULL, delims, saveptr);
  }
  if(count == 0){
    return NULL;  
  }
  ret = realloc(ret, count * sizeof(char*));
  *out_count = count;

  return ret;
}
