#include "proto.h"

void define_detect_input_function (void) {
  add_declared_label_to_codefile(builder_state -> codefile, get_label(detect_input, "DetectInput"));
  inst(INST_SET, ARGTYPE_NAMED_REGISTER, builder_state -> registers.file, ARGTYPE_IMMEDIATE, 0);
  inst(INST_LENGTH, ARGTYPE_NAMED_REGISTER, builder_state -> registers.result);
  inst(INST_SET, ARGTYPE_NAMED_REGISTER, builder_state -> registers.argument, ARGTYPE_NAMED_LABEL, get_label(file_sizes, "FileSizes"));
  if (add_local_label_to_codefile(builder_state -> codefile, "loop") < 0) builder_throw("could not declare local label '.loop'");
  inst(INST_GETWORDINC, ARGTYPE_NAMED_REGISTER, builder_state -> registers.temp, ARGTYPE_NAMED_REGISTER, builder_state -> registers.argument);
  inst(INST_IFNE, ARGTYPE_NAMED_REGISTER, builder_state -> registers.temp, ARGTYPE_NAMED_REGISTER, builder_state -> registers.result,
                  ARGTYPE_LOCAL_LABEL, "next_file");
  inst(INST_MULTIPLY, ARGTYPE_NAMED_REGISTER, builder_state -> registers.temp, ARGTYPE_NAMED_REGISTER, builder_state -> registers.file,
                      ARGTYPE_NAMED_CONSTANT, builder_state -> constants.hash_size);
  inst(INST_ADD2, ARGTYPE_NAMED_REGISTER, builder_state -> registers.temp, ARGTYPE_NAMED_LABEL, get_label(file_hashes, "FileHashes"));
  inst(INST_CHECKSHA1, ARGTYPE_NAMED_REGISTER, builder_state -> registers.temp, ARGTYPE_NAMED_REGISTER, builder_state -> registers.temp);
  inst(INST_RETZ, ARGTYPE_NAMED_REGISTER, builder_state -> registers.temp);
  if (add_local_label_to_codefile(builder_state -> codefile, "next_file") < 0) builder_throw("could not declare local label '.next_file'");
  inst(INST_INCREMENT, ARGTYPE_NAMED_REGISTER, builder_state -> registers.file);
  inst(INST_IFLT, ARGTYPE_NAMED_REGISTER, builder_state -> registers.file, ARGTYPE_NAMED_CONSTANT, builder_state -> constants.num_input_files,
                  ARGTYPE_LOCAL_LABEL, "loop");
  inst(INST_SET, ARGTYPE_NAMED_REGISTER, builder_state -> registers.argument, ARGTYPE_NAMED_CONSTANT, builder_state -> constants.errors[CODE_ERROR_INVALID_SOURCE]);
  inst(INST_JUMP, ARGTYPE_NAMED_LABEL, get_label(error, "Error"));
  add_blank_line_to_codefile(builder_state -> codefile);
}

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
