#include "proto.h"

void add_data_to_codefile (CodeFile file, const void * data, unsigned length) {
  char buffer[80] = "\thexdata ";
  const char * current = data;
  unsigned char p, buffer_size = 9 /* strlen("\thexdata ") */;
  while (length > 32) {
    for (p = 0; p < 32; p ++) buffer_size += sprintf(buffer + buffer_size, "%02hhx", *(current ++));
    add_line_to_codefile(file, buffer);
    length -= 32;
    buffer_size = 9;
  }
  if (!length) return;
  for (p = 0; p < length; p ++) buffer_size += sprintf(buffer + buffer_size, "%02hhx", *(current ++));
  add_line_to_codefile(file, buffer);
}

int add_string_to_codefile (CodeFile file, const char * string) {
  if (!validate_UTF8(string)) return 0;
  const char * p;
  char * buf;
  char * line;
  unsigned line_length;
  while (p = find_next_invalid_string_character(string)) {
    buf = malloc(p - string + 1);
    memcpy(buf, string, p - string);
    buf[p - string] = 0;
    line = generate_string("\tdb \"%s\"", buf);
    line_length = strlen(line);
    free(buf);
    string = p + count_invalid_string_characters(p);
    while (p != string) {
      line = realloc(line, line_length + 7);
      line_length += sprintf(line + line_length, ", 0x%02hhx", *(p ++));
    }
    add_line_to_codefile(file, line);
    free(line);
  }
  add_formatted_line_to_codefile(file, "\tstring \"%s\"", string);
  return 1;
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
