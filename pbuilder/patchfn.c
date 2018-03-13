#include "proto.h"

/*
   Patch format:
   1 byte: patch engine ID (bits 0-6), reversible patch flag (bit 7)
   If the patch is reversible, 4 bytes: pointer to reverse patch
   Data (interpreted by the engine
*/

void define_apply_patch_function (void) {
  add_declared_label_to_codefile(builder_state -> codefile, get_label(apply_patch, "ApplyPatch"));
  inst(INST_GETBYTEINC, reg(result), reg(argument));
  inst(INST_IFLT, reg(result), imm(0x80), lbl(execute_patch, "ExecutePatch"));
  inst(INST_AND2, reg(result), imm(0x7f));
  inst(INST_ADD2, reg(argument), imm(4));
  inst(INST_JUMP, lbl(execute_patch, "ExecutePatch"));
  add_blank_line_to_codefile(builder_state -> codefile);
}

void define_apply_reverse_patch_function (void) {
  add_declared_label_to_codefile(builder_state -> codefile, get_label(apply_reverse_patch, "ApplyReversePatch"));
  inst(INST_GETBYTEINC, reg(result), reg(argument));
  inst(INST_IFLT, reg(result), imm(0x80), loc("not_reversible"));
  inst(INST_AND2, reg(result), imm(0x7f));
  inst(INST_GETWORD, reg(argument), reg(argument));
  inst(INST_JUMP, lbl(execute_patch, "ExecutePatch"));
  add_blank_line_to_codefile(builder_state -> codefile);
  builder_declare_local("not_reversible");
  inst(INST_SET, reg(argument), err(CODE_ERROR_PATCH_NOT_REVERSIBLE));
  inst(INST_JUMP, lbl(error, "Error"));
  add_blank_line_to_codefile(builder_state -> codefile);
}
