#include "proto.h"

void initialize_builder_state (Options options, CodeFile codefile) {
  builder_state = calloc(1, sizeof(struct code_generation_state));
  builder_state -> options = options;
  builder_state -> codefile = codefile;
  memset(&(builder_state -> functions), -1, sizeof (builder_state -> functions)); // this should be enough to set them all to some negative value
}

void initialize_code_generator (void) {
  builder_state -> registers.file = declare_register("file", 0);
  builder_state -> registers.argument = declare_register("argument", 1);
  builder_state -> registers.temp = declare_register("temp", 2);
  builder_state -> registers.result = declare_register("result", 3);
  // ...
}

int declare_register (const char * register_name, unsigned char register_offset) {
  unsigned char num = builder_state -> options -> initial_register_number + register_offset;
  int result = add_register_definition_to_codefile(builder_state -> codefile, register_name, num);
  if (result < 0) builder_throw("could not declare register '%s'", register_name);
  return result;
}
