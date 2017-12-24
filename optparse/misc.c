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
  if (!*argument) return; // just in case an empty argument manages to make it through
  if (options -> input_file_count >= 0xffff00) {
    options -> error_text = copy_string_for_options(options, "too many input files");
    return;
  }
  options -> input_files = mr_realloc(options -> memory_region, options -> input_files, (options -> input_file_count + 1) * sizeof(struct option_file));
  options -> input_files[options -> input_file_count ++] = (struct option_file) {
    .name = copy_string_for_options(options, argument),
    .label = NULL,
    .reference = options -> current_conversion_reference,
    .method = options -> current_conversion_method,
    .direction = options -> current_conversion_direction
  };
}
