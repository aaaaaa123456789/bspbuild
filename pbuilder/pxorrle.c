#include "proto.h"

void define_apply_xor_rle_patch_helper_function (void) {
  // receives: #argument: pointer to patch, #result: bytes to patch; returns actually patched bytes in #result
  add_declared_label_to_codefile(builder_state -> codefile, get_label(apply_xor_rle_patch, "ApplyXORRLEPatch"));
  inst(INST_RETZ, reg(result));
  inst(INST_SET, reg(temp), reg(result));
  inst(INST_SET, reg(result), imm(0));
  builder_declare_local("loop");
  inst(INST_PUSH, reg(temp));
  // ...
  inst(INST_POP, reg(temp));
  inst(INST_IFLT, reg(result), reg(temp), loc("loop"));
  inst(INST_RETURN);
  add_blank_line_to_codefile(builder_state -> codefile);
}
