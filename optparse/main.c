#include "proto.h"

Options parse_options (char ** arguments, unsigned argument_count) {
  if (!(arguments && argument_count)) return no_arguments_response();
  int no_more_options = 0;
  Options result = new_options_object();
  unsigned current;
  for (current = 0; current < argument_count; current ++) {
    if (no_more_options)
      parse_naked_argument(result, arguments[current]);
    else if (!strcmp(arguments[current], "--"))
      no_more_options = 1;
    else if (!strcmp(arguments[current], "--help")) {
      destroy_options_object(result);
      return help_response();
    } else if (!strcmp(arguments[current], "--version")) {
      destroy_options_object(result);
      return version_response();
    } else if (*(arguments[current]) == '-') {
      if (parse_option(result, arguments[current], ((current + 1) == argument_count) ? NULL : arguments[current + 1])) current ++;
    } else
      parse_naked_argument(result, arguments[current]);
    if (result -> error_text) return result;
  }
  process_input_file_options(result);
  if (!(result -> error_text)) validate_options(result);
  return result;
}

Options new_options_object (void) {
  void * mr = create_memory_region();
  Options result = mr_calloc(mr, sizeof(struct options));
  result -> memory_region = mr;
  result -> current_conversion_direction = DIRECTION_SOURCE_TARGET;
  result -> current_conversion_method = METHOD_XOR_RLE;
  result -> current_conversion_reference = TARGET_TYPE_PREVIOUS;
  result -> initial_register_number = 252;
  return result;
}

void destroy_options_object (Options options) {
  destroy_memory_region(options -> memory_region);
}

int parse_option (Options options, const char * option, const char * argument) {
  unsigned current;
  for (current = 0; option_parsers[current].option; current ++) if (!strcmp(option_parsers[current].option, option)) break;
  if (!option_parsers[current].option) {
    options -> error_text = generate_string_for_options(options, "unknown option: %s", option);
    return 0;
  }
  if (option_parsers[current].has_argument && !argument) {
    options -> error_text = generate_string_for_options(options, "option %s requires an argument", option);
    return 0;
  }
  char * error;
  if (option_parsers[current].has_argument)
    error = option_parsers[current].argument_callback(options, argument, option_parsers[current].parameter);
  else
    error = option_parsers[current].no_argument_callback(options, option_parsers[current].parameter);
  if (error) {
    options -> error_text = copy_string_for_options(options, error);
    free(error);
  }
  return option_parsers[current].has_argument;
}
