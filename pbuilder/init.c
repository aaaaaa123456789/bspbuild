#include "proto.h"

void initialize_builder_state (Options options, CodeFile codefile) {
  builder_state = mr_calloc(builder_memory_region, sizeof(struct code_generation_state));
  builder_state -> options = options;
  builder_state -> codefile = codefile;
  memset(&(builder_state -> labels), -1, sizeof (builder_state -> labels)); // this should be enough to set them all to some negative value
  memset(&(builder_state -> constants), -1, sizeof (builder_state -> constants));
}

void initialize_code_generator (void) {
  builder_state -> registers.file = declare_register("file", 0);
  builder_state -> registers.argument = declare_register("argument", 1);
  builder_state -> registers.temp = declare_register("temp", 2);
  builder_state -> registers.result = declare_register("result", 3);
  add_blank_line_to_codefile(builder_state -> codefile);
  *(builder_state -> constants.errors) = declare_constant("NO_ERRORS", 0);
  builder_state -> constants.errors[CODE_ERROR_INVALID_SOURCE] = declare_constant("ERROR_INVALID_SOURCE", 1);
  builder_state -> constants.errors[CODE_ERROR_SOURCE_EQUALS_TARGET] = declare_constant("ERROR_SOURCE_EQUALS_TARGET", 2);
  add_blank_line_to_codefile(builder_state -> codefile);
  // ...
}

int declare_register (const char * register_name, unsigned char register_offset) {
  unsigned char num = builder_state -> options -> initial_register_number + register_offset;
  int result = add_register_definition_to_codefile(builder_state -> codefile, register_name, num);
  if (result < 0) builder_throw("could not declare register '%s'", register_name);
  return result;
}

int declare_constant (const char * constant_name, unsigned constant_value) {
  int result = add_constant_to_codefile(builder_state -> codefile, constant_name, constant_value);
  if (result < 0) builder_throw("could not declare constant '%s'", constant_name);
  return result;
}
