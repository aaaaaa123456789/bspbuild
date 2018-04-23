#include "proto.h"

void define_apply_xor_patch_function (void) {
  define_apply_xor_like_patch_function(0);
}

void define_apply_xor_rle_patch_function (void) {
  builder_state -> needed_functions.apply_xor_rle_patch = 1;
  define_apply_xor_like_patch_function(1);
}

void define_apply_xor_like_patch_function (int is_RLE) {
  if (builder_state -> options -> fragment_size) {
    define_apply_xor_like_fragmented_patch_function(is_RLE);
    return;
  }
  if (builder_state -> options -> padding_size) {
    inst(INST_GETWORDINC, reg(temp), reg(argument));
    if (is_RLE) inst(INST_PUSH, reg(temp));
  }
  inst(INST_GETWORDINC, reg(result), reg(argument));
  inst(INST_GETWORD, reg(argument), reg(argument));
  inst(INST_TRUNCATE, reg(result));
  inst(INST_SEEK, imm(0));
  if (is_RLE)
    inst(builder_state -> options -> padding_size ? INST_CALL : INST_JUMP, lbl(apply_xor_rle_patch, "ApplyXORRLEPatch"));
  else {
    inst(INST_XORDATA, reg(argument), reg(result));
    if (!(builder_state -> options -> padding_size)) inst(INST_RETURN);
  }
  if (!(builder_state -> options -> padding_size)) return;
  if (is_RLE)
    inst(INST_POP, reg(argument));
  else
    inst(INST_SET, reg(argument), reg(temp));
  builder_state -> needed_functions.pad_to_length = 1;
  inst(INST_JUMP, lbl(pad_to_length, "PadToLength"));
}

void define_apply_xor_like_fragmented_patch_function (int is_RLE) {
  if (is_RLE)
    inst(INST_SET, reg(result), lbl(apply_xor_rle_patch, "ApplyXORRLEPatch"));
  else
    inst(INST_SET, reg(result), loc("apply_xor_patch"));
  builder_state -> needed_functions.apply_fragmented_patch = 1;
  inst(INST_JUMP, lbl(apply_fragmented_patch, "ApplyFragmentedPatch"));
  if (is_RLE) return;
  add_blank_line_to_codefile(builder_state -> codefile);
  builder_declare_local("apply_xor_patch");
  inst(INST_XORDATA, reg(argument), reg(result));
  inst(INST_RETURN);
}
