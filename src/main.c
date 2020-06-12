#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_utils.h"
int main(void) {
  FILE* fp;
  if((fp = fopen("/homes/ms3319/arm11_11/tent/src/text.txt","r+")) == NULL) {
    printf("No\n");
    return -1;
  }
  replace_strings(fp);


  return 0;
}	
