#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "file_utils.h"
#include "convert.h"
#include "handle_markups.h"

//returns a large string containing all of the HTML
char* parse_markdown(FILE *input){

  long bufflen;
  char* source = read_file_into_buffer(input, &bufflen);

  char *line;
  //go through line by line
  line = strtok_r(source, "\n", &source);
  while(line != NULL){
    Line markup_line;
    int token_count;
    char** tokens = tokenize(line, &line, &token_count);
    //for look that will go through every token in the line
    //will now need to implement a new markup type such that it is just text.
    for(int i = 0; i < token_count; ++i){
	markup_line.markup = determine_markup(tokens[i]); 
	switch(markup_line.markup){
	  case Header:
	    handle_header(&markup_line, tokens, token_count);
	    break;
	  case Bold:
	    handle_bold(&markup_line, tokens, token_count);
	    break;
	  case Italic:
	    handle_italic(&markup_line, tokens, token_count);
	    break;	
	  case Link:
	  case Paragraph:
	    perror("can't handle this markup yet");
	    exit(EXIT_FAILURE);	
	}    
    }
  }
  char* ret = NULL;
  return ret;
}

MarkupType determine_markup(char* currWord){
  if (currWord[0] == '#'){
    return Header;
  } else if (currWord[0] == '*'){
    return Bold;
  } else if (currWord[0] == '_'){
    return Italic;
  } else {
    perror("haven't managed to program this far yet");
    exit(EXIT_FAILURE);
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
