#include "proto.h"

FILE * open_text_file (const char * file, char ** error) {
  return open_file(file, "r", "reading", error);
}

FILE * open_binary_file (const char * file, char ** error) {
  return open_file(file, "rb", "reading", error);
}

FILE * open_binary_file_for_writing (const char * file, char ** error) {
  return open_file(file, "wb", "writing", error);
}

FILE * open_file (const char * file, const char * mode, const char * mode_description, char ** error) {
  FILE * fp = fopen(file, mode);
  if (!fp) {
    *error = malloc(strlen(file) + strlen(mode_description) + 26);
    sprintf(*error, "could not open file %s for %s", file, mode_description);
  } else
    *error = NULL;
  return fp;
}

char * read_line (FILE * fp) {
  char * line = NULL;
  unsigned length = 0;
  int c;
  while (1) {
    c = getc(fp);
    if ((c == EOF) || (c == '\n')) break;
    if (c == '\r') continue;
    line = realloc(line, length + 1);
    line[length ++] = c;
  }
  line = realloc(line, length + 1);
  line[length] = 0;
  return line;
}

int write_data_to_file (FILE * fp, const void * data, unsigned length) {
  unsigned rv;
  const char * current = data;
  while (length) {
    rv = fwrite(current, 1, (length > 65536) ? 65536 : length, fp);
    if (!rv) return 0;
    current += rv;
    length -= rv;
  }
  return 1;
}
