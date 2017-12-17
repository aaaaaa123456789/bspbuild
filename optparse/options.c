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

char * set_input_bsp_source_option (Options options, int param) {
  if (options -> input_bsp_source) return multiple_option_response("--bsp");
  options -> input_bsp_source = 1;
  return NULL;
}

char * set_compiled_output_file (Options options, const char * filename, int param) {
  if (options -> output_files.compiled) return multiple_option_response("-o");
  options -> output_files.compiled = copy_string_for_options(options, filename);
  return NULL;
}
