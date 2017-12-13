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
    } else if (*(arguments[current]) == '-')
      parse_option(result, arguments[current], ((current + 1) == argument_count) ? NULL : arguments[current + 1]);
    else
      parse_naked_argument(result, arguments[current]);
    if (result -> error_text) return result;
  }
  validate_options(result);
  return result;
}

void parse_option (Options options, const char * option, const char * argument) {
  // ...
}

void parse_naked_argument (Options options, const char * argument) {
  // ...
}
