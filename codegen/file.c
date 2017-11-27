#include "proto.h"

void add_blank_line_to_codefile (CodeFile file) {
  add_line_to_codefile(file, "");
}

void add_comment_to_codefile (CodeFile file, const char * comment, int indented) {
  char * p = strchr(comment, '\n');
  if (!p) {
    add_formatted_line_to_codefile(file, "%s; %s", indented ? "\t" : "", comment);
    return;
  }
  char * line = malloc(p - comment + 1);
  memcpy(line, comment, p - comment);
  line[p - comment] = 0;
  if ((p != comment) && (line[p - comment - 1] == '\r')) line[p - comment - 1] = 0;
  add_comment_to_codefile(file, line, indented);
  free(line);
  if (p[1]) add_comment_to_codefile(file, p + 1, indented);
  return;
}

void add_line_to_codefile (CodeFile file, const char * line) {
  // ...
}

void add_formatted_line_to_codefile (CodeFile file, const char * format, ...) {
  va_list ap;
  va_start(ap, format);
  char * string = generate_string_from_varargs(format, ap);
  va_end(ap);
  add_line_to_codefile(file, string);
  free(string);
}
