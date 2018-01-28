#include "proto.h"

void validate_options (Options options) {
  char * (* const validators[])(Options) = {
    &validate_input_file_options,
    &validate_output_file_options,
    &validate_nonsensical_option_combinations,
    &validate_options_in_bsp_mode,
    &validate_options_in_ips_mode,
    // ...
  };
  unsigned validator;
  char * error;
  for (validator = 0; validator < (sizeof validators / sizeof *validators); validator ++)
    if (error = validators[validator](options)) {
      options -> error_text = error;
      return;
    }
}

char * validate_input_file_options (Options options) {
  if (options -> file_count_per_direction[DIRECTION_SOURCE_TARGET]) return NULL;
  if (!(options -> file_count_per_direction[DIRECTION_TARGET])) return copy_string_for_options(options, "no target input files");
  if (!(options -> file_count_per_direction[DIRECTION_SOURCE])) return copy_string_for_options(options, "no source input files");
  return NULL;
}

char * validate_output_file_options (Options options) {
  if (!(options -> output_files.compiled || options -> output_files.source)) return copy_string_for_options(options, "no output files");
  return NULL;
}

char * validate_nonsensical_option_combinations (Options options) {
  if (options -> detect_fragment_permutation && !(options -> fragmentation_parameters_given))
    return copy_string_for_options(options, "fragment swap detection requested without fragment size");
  if (options -> messages.error_replacement && !(options -> messages.error))
    return copy_string_for_options(options, "error text substitution given without error text");
  if (options -> suppress_error_messages && options -> messages.error)
    return copy_string_for_options(options, "error message suppression requested along with an error message");
  if (options -> prefixes.global && (options -> prefixes.label || options -> prefixes.variable || options -> prefixes.constant))
    return copy_string_for_options(options, "--prefix cannot be given with other --*-prefix options");
  return NULL;
}

char * validate_options_in_bsp_mode (Options options) {
  // ...
}

char * validate_options_in_ips_mode (Options options) {
  // ...
}
