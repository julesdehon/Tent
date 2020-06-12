#include "string_utils.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

// Adapted from:
// https://stackoverflow.com/questions/779875/what-is-the-function-to-replace-string-in-c
// You must free the result if result is non-NULL.
char* str_replace(char* orig, char* rep, char* with) {
	char* result; // the return string
	char* ins;    // the next insert point
	char* tmp;    // varies
	int len_rep;  // length of rep (the string to remove)
	int len_with; // length of with (the string to replace rep with)
	int len_front; // distance between rep and end of last rep
	int count;    // number of replacements

	// sanity checks and initialization
	if (!orig || !rep) {
		return NULL;
	}
	len_rep = strlen(rep);
	if (len_rep == 0) {
		return NULL; // empty rep causes infinite loop during count
	}
	if (!with) {
		with = "";
	}
	len_with = strlen(with);

	// count the number of replacements needed
	ins = orig;
	for (count = 0; (tmp = strstr(ins, rep)); count++) {
		ins = tmp + len_rep;
	}

	tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

	if (!result) {
		return NULL;
	}

	// first time through the loop, all the variable are set correctly
	// from here on,
	//    tmp points to the end of the result string
	//    ins points to the next occurrence of rep in orig
	//    orig points to the remainder of orig after "end of rep"
	while (count--) {
		ins = strstr(orig, rep);
		len_front = ins - orig;
		tmp = strncpy(tmp, orig, len_front) + len_front;
		tmp = strcpy(tmp, with) + len_with;
		orig += len_front + len_rep; // move to next "end of rep"
	}
	strcpy(tmp, orig);
	return result;
}

// Returns a pointer to the substring where that starts after the leading
// whitespace and terminates before the trailing whitespace.
// Keep track of the original char* if you need to free it as you can't
// use the return value to free the string
char* trim_whitespace(char* orig) {
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*orig)) orig++;

  if(*orig == 0)  // All spaces?
    return orig;

  // Trim trailing space
  end = orig + strlen(orig) - 1;
  while(end > orig && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return orig;
}

int str_equal(char* a, char* b) {
	return strcmp(a, b) == 0;
}

char* str_append(char* start, char* add) {
	char* ret = (char*) calloc(strlen(start) + strlen(add) + 1, sizeof(char));
	strcpy(ret, start);
	strcpy(ret+strlen(start), add);
	return ret;
}
