#include "proto.h"

void validate_options (Options options) {
  char * (* const validators[])(Options) = {
    &validate_input_file_options,
    &validate_output_file_options,
    &validate_nonsensical_option_combinations,
    &validate_options_in_bsp_mode,
    &validate_options_in_ips_mode,
    &validate_invalid_values
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
  if (options -> suppress_source_detection_message && options -> messages.source_detection)
    return copy_string_for_options(options, "source detection message suppression requested along with a source detection message");
  if (options -> prefixes.global && (options -> prefixes.label || options -> prefixes.variable || options -> prefixes.constant))
    return copy_string_for_options(options, "--prefix cannot be given with other --*-prefix options");
  return NULL;
}

char * validate_options_in_bsp_mode (Options options) {
  if (options -> operation_mode != OPERATION_MODE_BSP_INPUT) return NULL;
  return validate_options_in_nonstandard_modes(options, "BSP compilation");
}

char * validate_options_in_ips_mode (Options options) {
  if (options -> operation_mode != OPERATION_MODE_IPS_OUTPUT) return NULL;
  char * error = validate_options_in_nonstandard_modes(options, "IPS generation");
  if (error) return error;
  if (options -> input_file_count != 2)
    return copy_string_for_options(options, "exactly two input files must be given in IPS generation mode");
  return NULL;
}

static inline char * invalid_nonstandard_mode_option_error_text (Options options, const char * mode_name, const char * option_name) {
  return generate_string_for_options(options, "%s option%s not valid in %s mode", option_name, (*option_name == '-') ? "" : "s", mode_name);
}

char * validate_options_in_nonstandard_modes (Options options, const char * mode_name) {
  if (options -> messages.opening || options -> messages.success || options -> messages.error || options -> messages.error_replacement)
    return invalid_nonstandard_mode_option_error_text(options, mode_name, "banner");
  if (options -> output_files.source)
    return invalid_nonstandard_mode_option_error_text(options, mode_name, "-ob");
  if (options -> prefixes.global || options -> prefixes.label || options -> prefixes.variable || options -> prefixes.constant)
    return invalid_nonstandard_mode_option_error_text(options, mode_name, "prefix");
  if (options -> label_file)
    return invalid_nonstandard_mode_option_error_text(options, mode_name, "--titles");
  if (options -> fragment_size || options -> fragmentation_parameters_given || options -> detect_fragment_permutation)
    return invalid_nonstandard_mode_option_error_text(options, mode_name, "fragmentation");
  if (options -> padding_size)
    return invalid_nonstandard_mode_option_error_text(options, mode_name, "padding");
  if (options -> disable_output_validations)
    return invalid_nonstandard_mode_option_error_text(options, mode_name, "validation");
  if (options -> sort_targets_alphabetically || options -> output_selection_on_single_option || options -> targets_per_page)
    return invalid_nonstandard_mode_option_error_text(options, mode_name, "output menu");
  if (options -> direction_options_given)
    return invalid_nonstandard_mode_option_error_text(options, mode_name, "input file type");
  if (options -> patch_method_options_given)
    return invalid_nonstandard_mode_option_error_text(options, mode_name, "patching method");
  if (options -> initial_register_number_given)
    return invalid_nonstandard_mode_option_error_text(options, mode_name, "--initial-register");
  return NULL;
}

char * validate_invalid_values (Options options) {
  if (
    (options -> operation_mode == 3) ||
    (options -> disable_output_validations == 3) ||
    (options -> targets_per_page == 1)
  ) return copy_string_for_options(options, "[internal] invalid option values detected");
  unsigned file;
  for (file = 0; file < options -> input_file_count; file ++)
    if (
      (options -> input_files[file].direction == 3) ||
      ((options -> input_files[file].reference < MAX_INPUT_FILES) && (options -> input_files[file].reference >= options -> input_file_count)) ||
      (options -> input_files[file].method >= NUM_PATCHING_METHODS)
    ) return generate_string_for_options(options, "[internal] invalid parameters for file %s detected", options -> input_files[file].name);
  return NULL;
}
