#include "proto.h"

char * prefix_option (Options options, const char * prefix, int param) {
  if (options -> prefixes.global) return multiple_option_response("--prefix");
  options -> prefixes.global = copy_string_for_options(options, prefix);
  return NULL;
}

char * label_prefix_option (Options options, const char * prefix, int param) {
  if (options -> prefixes.label) return multiple_option_response("--label-prefix");
  options -> prefixes.label = copy_string_for_options(options, prefix);
  return NULL;
}

char * constant_prefix_option (Options options, const char * prefix, int param) {
  if (options -> prefixes.constant) return multiple_option_response("--constant-prefix");
  options -> prefixes.constant = copy_string_for_options(options, prefix);
  return NULL;
}

char * variable_prefix_option (Options options, const char * prefix, int param) {
  if (options -> prefixes.variable) return multiple_option_response("--register-prefix");
  options -> prefixes.variable = copy_string_for_options(options, prefix);
  return NULL;
}

char * set_next_input_file_mode (Options options, int new_mode) {
  options -> current_conversion_direction = new_mode;
  return NULL;
}
