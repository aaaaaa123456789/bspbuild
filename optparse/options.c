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

char * set_hide_errors_option (Options options, int param) {
  if (options -> suppress_error_messages) return multiple_option_response("--hide-errors");
  options -> suppress_error_messages = 1;
  return NULL;
}

char * set_force_output_menu_option (Options options, int param) {
  if (options -> output_selection_on_single_option) return multiple_option_response("--force-output-menu");
  options -> output_selection_on_single_option = 1;
  return NULL;
}

char * disable_output_validation_option (Options options, int disable_value) {
  if (options -> disable_output_validations) {
    if (options -> disable_output_validations != disable_value) return duplicate_string("conflicting output validation disabling options given");
    return duplicate_string("output validation disabling option given more than once");
  }
  options -> disable_output_validations = disable_value;
  return NULL;
}

char * no_source_to_source_option (Options options, int param) {
  if (options -> no_source_patches) return multiple_option_response("--no-source-to-source");
  options -> no_source_patches = 1;
  return NULL;
}

char * sort_target_list_option (Options options, int param) {
  if (options -> sort_targets_alphabetically) return multiple_option_response("--sort-output-menu");
  options -> sort_targets_alphabetically = 1;
  return NULL;
}

char * set_target_and_reference_option (Options options, int reference_mode) {
  // reference mode: -1 = previous file, 0..0xffffff = set to value
  if (reference_mode < 0) {
    if ((!(options -> input_file_count)) || (options -> input_files[options -> input_file_count - 1].direction != DIRECTION_TARGET))
      return duplicate_string("reference for -tl/--target-from-last must be a target file");
    options -> current_conversion_reference = options -> input_file_count - 1;
  } else
    options -> current_conversion_reference = reference_mode;
  options -> current_conversion_direction = DIRECTION_TARGET;
}
