#include "proto.h"

signed char find_patch_method_by_name (const char * method_name) {
  signed char current;
  char * lowercase_name = duplicate_string(method_name);
  string_to_lowercase(lowercase_name);
  for (current = 0; current < NUM_PATCHING_METHODS; current ++) if (!strcmp(lowercase_name, patching_method_names[current])) {
    free(lowercase_name);
    return current;
  }
  free(lowercase_name);
  return -1;
}
