#include "proto.h"

char * prefix_option (Options options, const char * prefix, int param) {
  if (options -> prefixes.global) return multiple_option_response("--prefix");
  options -> prefixes.global = copy_string_for_options(options, prefix);
  return NULL;
}

char * label_prefix_option (Options options, const char * prefix, int param) {
  // ...
}

char * constant_prefix_option (Options options, const char * prefix, int param) {
  // ...
}

char * variable_prefix_option (Options options, const char * prefix, int param) {
  // ...
}
