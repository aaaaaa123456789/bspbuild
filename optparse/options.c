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

char * set_bsp_output_file (Options options, const char * filename, int param) {
  if (options -> output_files.source) return multiple_option_response("-ob");
  options -> output_files.source = copy_string_for_options(options, filename);
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
  return NULL;
}

char * title_file_option (Options options, const char * filename, int param) {
  if (options -> label_file) return multiple_option_response("--titles");
  options -> label_file = copy_string_for_options(options, filename);
  return NULL;
}

char * suppress_errors_option (Options options, int param) {
  if (options -> suppress_error_messages) return multiple_option_response("--suppress-errors");
  options -> suppress_error_messages = 1;
  return NULL;
}

char * targets_per_page_option (Options options, const char * new_value, int param) {
  if (options -> targets_per_page) return multiple_option_response("--targets-per-page");
  char * error;
  unsigned value = convert_digit_string_to_number(new_value, &error);
  free(error);
  if (error) return generate_string("invalid argument to --targets-per-page: %s", new_value);
  if ((value < 2) || (value > 15)) return duplicate_string("number of targets per page must be between 2 and 15");
  options -> targets_per_page = value;
  return NULL;
}

char * check_fragment_permutation_option (Options options, int param) {
  if (options -> detect_fragment_permutation) return multiple_option_response("--check-fragment-swap");
  options -> detect_fragment_permutation = 1;
  return NULL;
}

char * set_fragment_size (Options options, const char * new_value, int param) {
  if (options -> fragment_size) return multiple_option_response("-f");
  char * error;
  unsigned size = convert_string_to_number(new_value, &error);
  free(error);
  if (error) return generate_string("invalid argument to -f: %s", new_value);
  options -> fragment_size = size;
  options -> fragmentation_parameters_given = 1;
  return NULL;
}

char * set_padding_value (Options options, const char * new_value, int size) {
  if (options -> padding_size) return duplicate_string("padding value already given");
  char * error;
  unsigned value = convert_string_to_number(new_value, &error);
  free(error);
  if (error) return generate_string("invalid padding value: %s", new_value);
  options -> padding_value = value & ((unsigned []) {0xff, 0xffff, 0xffffffff})[size - 1];
  options -> padding_size = size;
  return NULL;
}
