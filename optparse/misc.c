#include "proto.h"

char * multiple_option_response (const char * option_name) {
  return generate_string("option %s given more than once", option_name);
}

char * generate_string_for_options (Options options, const char * fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  char * temp = generate_string_from_varargs(fmt, ap);
  va_end(ap);
  char * result = copy_string_for_options(options, temp);
  free(temp);
  return result;
}

char * copy_string_for_options (Options options, const char * string) {
  unsigned size = strlen(string) + 1;
  char * result = mr_malloc(options -> memory_region, size);
  memcpy(result, string, size);
  return result;
}

void parse_naked_argument (Options options, const char * argument) {
  // ...
}
