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
  validate_options(result);
  return result;
}

int parse_option (Options options, const char * option, const char * argument) {
  unsigned current;
  for (current = 0; option_parsers[current].option; current ++) if (!strcmp(option_parsers[current].option, option)) break;
  if (!option_parsers[current].option) {
    options -> error_text = generate_string_for_options(options, "unknown option: %s", option);
    return 0;
  }
  if (option_parsers[current].argument_required && !argument) {
    options -> error_text = generate_string_for_options(options, "option %s requires an argument", option);
    return 0;
  }
  char * error;
  if (option_parsers[current].argument_required)
    error = option_parsers[current].argument_callback(options, argument, option_parsers[current].parameter);
  else
    error = option_parsers[current].no_argument_callback(options, option_parsers[current].parameter);
  if (error) {
    options -> error_text = mr_malloc(options -> memory_region, strlen(error) + 1);
    strcpy(options -> error_text, error);
    free(error);
  }
  return option_parsers[current].argument_required;
}

void parse_naked_argument (Options options, const char * argument) {
  // ...
}
