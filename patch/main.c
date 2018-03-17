#include "proto.h"

char * write_patch_to_codefile (CodeFile codefile, Buffer source, Buffer target, const struct patching_flags * flags) {
  if (flags -> patching_method >= NUM_PATCHING_METHODS) return generate_string("unknown patching method %hhu", flags -> patching_method);
  return patch_generator_functions[flags -> patching_method](codefile, source, target, flags);
}

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
