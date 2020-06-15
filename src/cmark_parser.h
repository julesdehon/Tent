#ifndef CMARK_PARSER_H
#define CMARK_PARSER_H

#ifdef OUR_MARKDOWN_PARSER
#define convert_md_to_html(text, length) (md_to_html(text, length))
#else
#define convert_md_to_html(text, length) (cmark_markdown_to_html(text, length, 0))
#endif

char *parse_markdown(FILE *f, VariableMap *meta_map);

#endif
