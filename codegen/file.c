#include "proto.h"

void add_blank_line_to_codefile (CodeFile file) {
  putc('\n', file -> fp);
}

void add_comment_to_codefile (CodeFile file, const char * comment, int indented) {
  char * p = strchr(comment, '\n');
  if (!p) {
    fprintf(file -> fp, "%s; %s\n", indented ? "\t" : "", comment);
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
