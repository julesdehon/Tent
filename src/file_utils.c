#include "file_utils.h"
#include "string_utils.h"
#include <stdlib.h>

// Remember to free the returned buffer if non null!
char* read_file_into_buffer(FILE* file, long* filelen) {
	fseek(file, 0, SEEK_END);
	*filelen = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* ret = calloc(*filelen, sizeof(char));
	fread(ret, *filelen, 1, file);
	return ret;
}

void replace_strings(FILE* fp, char* rep, char* with) {
  fseek(fp, 0, SEEK_END);
  long long file_length = ftell(fp);
  fseek(fp,0,SEEK_SET);
  char* file_text = malloc(file_length + 1);
  fread(file_text,file_length,1,fp);
  char* final_text = str_replace(file_text,rep,with);
  if(fputs(final_text, fp) <= 0) {
    perror("Couldn't print to file!");
    exit(EXIT_FAILURE); 
  }
  fclose(fp);
  return;  
}	
