#include "proto.h"

void add_data_to_codefile (CodeFile file, const void * data, unsigned length) {
  const char * current = data;
  unsigned char p;
  while (length > 32) {
    fputs("\thexdata ", file -> fp);
    for (p = 0; p < 32; p ++) fprintf(file -> fp, "%02hhx", *(current ++));
    putc('\n', file -> fp);
    length -= 32;
  }
  if (!length) return;
  fputs("\thexdata ", file -> fp);
  for (p = 0; p < length; p ++) fprintf(file -> fp, "%02hhx", *(current ++));
  putc('\n', file -> fp);
}

void add_string_to_codefile (CodeFile file, const char * string) {
  const char * p;
  char * buf;
  while (p = find_next_invalid_string_character(string)) {
    buf = malloc(p - string + 1);
    memcpy(buf, string, p - string);
    buf[p - string] = 0;
    fprintf(file -> fp, "\tdb \"%s\"", buf);
    free(buf);
    string = p + count_invalid_string_characters(p);
    while (p != string) fprintf(file -> fp, ", 0x%02hhx", *(p ++));
    putc('\n', file -> fp);
  }
  fprintf(file -> fp, "\tstring \"%s\"\n", string);
}

const char * find_next_invalid_string_character (const char * string) {
  for (; *string; string ++) if ((*string < 0x20) || (*string == '"')) return string;
  return NULL;
}

unsigned count_invalid_string_characters (const char * string) {
  const char * current;
  for (current = string; *current; current ++) if ((*current >= 0x20) && (*current != '"')) break;
  return current - string;
}
