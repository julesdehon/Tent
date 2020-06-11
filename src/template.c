#include "template.h"
#include "file_utils.h"

#include <string.h>
#include <stdlib.h>

Template* create_template() {
	Template* t = (Template*) malloc(sizeof(Template));
	return t;
}

Template* load_template(FILE* file, TemplateType type) {
	Template* t = create_template();
	t->name = file_name_without_extension(file);
	t->type = type;
	t->content = read_file_into_buffer(file, &t->content_len);
	return t;
}
