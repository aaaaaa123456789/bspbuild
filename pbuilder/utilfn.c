#include "proto.h"

void define_detect_input_function (void) {
  add_declared_label_to_codefile(builder_state -> codefile, get_label(detect_input, "DetectInput"));
  inst(INST_SET, reg(file), imm(0));
  inst(INST_LENGTH, reg(result));
  inst(INST_SET, reg(argument), lbl(file_sizes, "FileSizes"));
  if (add_local_label_to_codefile(builder_state -> codefile, "loop") < 0) builder_throw("could not declare local label '.loop'");
  inst(INST_GETWORDINC, reg(temp), reg(argument));
  inst(INST_IFNE, reg(temp), reg(result), loc("next_file"));
  inst(INST_MULTIPLY, reg(temp), reg(file), cnst(hash_size));
  inst(INST_ADD2, reg(temp), lbl(file_hashes, "FileHashes"));
  inst(INST_CHECKSHA1, reg(temp), reg(temp));
  inst(INST_RETZ, reg(temp));
  if (add_local_label_to_codefile(builder_state -> codefile, "next_file") < 0) builder_throw("could not declare local label '.next_file'");
  inst(INST_INCREMENT, reg(file));
  inst(INST_IFLT, reg(file), cnst(num_input_files), loc("loop"));
  inst(INST_SET, reg(argument), err(CODE_ERROR_INVALID_SOURCE));
  inst(INST_JUMP, lbl(error, "Error"));
  add_blank_line_to_codefile(builder_state -> codefile);
}

void define_get_nth_string_function (void) {
  add_declared_label_to_codefile(builder_state -> codefile, get_label(get_nth_string, "GetNthString"));
  inst(INST_RETZ, reg(argument));
  if (add_local_label_to_codefile(builder_state -> codefile, "loop") < 0) builder_throw("could not declare local label '.loop'");
  inst(INST_GETBYTEINC, reg(temp), reg(result));
  inst(INST_JUMPNZ, reg(temp), loc("loop"));
  inst(INST_DECREMENT, reg(argument));
  inst(INST_JUMPNZ, reg(argument), loc("loop"));
  inst(INST_RETURN);
  add_blank_line_to_codefile(builder_state -> codefile);
}
