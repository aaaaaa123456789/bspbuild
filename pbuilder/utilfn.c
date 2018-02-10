#include "proto.h"

void define_get_nth_string_function (void) {
  add_declared_label_to_codefile(builder_state -> codefile, get_label(get_nth_string, "GetNthString"));
  inst(INST_RETZ, ARGTYPE_NAMED_REGISTER, builder_state -> registers.argument);
  if (add_local_label_to_codefile(builder_state -> codefile, "loop") < 0) builder_throw("could not declare local label '.loop'");
  inst(INST_GETBYTEINC, ARGTYPE_NAMED_REGISTER, builder_state -> registers.temp, ARGTYPE_NAMED_REGISTER, builder_state -> registers.result);
  inst(INST_JUMPNZ, ARGTYPE_NAMED_REGISTER, builder_state -> registers.temp, ARGTYPE_LOCAL_LABEL, "loop");
  inst(INST_DECREMENT, ARGTYPE_NAMED_REGISTER, builder_state -> registers.argument);
  inst(INST_JUMPNZ, ARGTYPE_NAMED_REGISTER, builder_state -> registers.argument, ARGTYPE_LOCAL_LABEL, "loop");
  inst(INST_RETURN);
  add_blank_line_to_codefile(builder_state -> codefile);
}
