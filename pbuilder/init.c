#include "proto.h"

void initialize_code_generator (void) {
  builder_state -> registers.stackframe = declare_register("stackframe", 0);
  builder_state -> registers.argument = declare_register("argument", 1);
  builder_state -> registers.temp = declare_register("temp", 2);
  builder_state -> registers.result = declare_register("result", 3);
  builder_state -> registers.error = declare_register("error", 3);
  // ...
}

int declare_register (const char * register_name, unsigned char register_offset) {
  unsigned char num = builder_state -> options -> initial_register_number + register_offset;
  int result = add_register_definition_to_codefile(builder_state -> codefile, register_name, num);
  if (result < 0) builder_throw("could not declare register '%s'", register_name);
  return result;
}
