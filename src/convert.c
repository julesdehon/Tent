#include <stdlib.h>
#include "file_utils.h"

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

char** tokenize(char* line, char** saveptr, int* out_count){
    char** ret = calloc(strlen(line) + 1, sizeof(char*));

    int count = 0;
    char delims[] = {' '};
    char* cur = strtok_r(line, delims, saveptr);
    int totalLength = 0;	

    return ret;
}
