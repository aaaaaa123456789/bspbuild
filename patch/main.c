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

const char * get_patch_method_name (unsigned char method) {
  if (method >= NUM_PATCHING_METHODS) return NULL;
  return patching_method_names[method];
}

char * write_patch_data_to_codefile (CodeFile codefile, unsigned count, const unsigned * values, const char * label) {
  struct instruction_argument * arguments = calloc(count + 1, sizeof(struct instruction_argument));
  unsigned current;
  for (current = 0; current < count; current ++) arguments[current] = (struct instruction_argument) {.type = ARGTYPE_IMMEDIATE, .value = values[current]};
  if (label) arguments[count ++] = (struct instruction_argument) {.type = ARGTYPE_LOCAL_LABEL, .string = label};
  char * result = add_instruction_with_arguments_to_codefile(codefile, INST_DW, count, arguments);
  free(arguments);
  return result;
}
