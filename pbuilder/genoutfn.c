#include "proto.h"

void define_generate_output_function (void) {
  add_declared_label_to_codefile(builder_state -> codefile, get_label(main, "GenerateOutput"));
  if (builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE_TARGET] > 1) {
    builder_state -> needed_functions.generate_reverse_output = 1;
    inst(INST_IFLT, reg(argument), reg(file), lbl(generate_reverse_output, "GenerateReverseOutput"));
  }
  inst(INST_XOR2, reg(temp), reg(temp));
  if (add_local_label_to_codefile(builder_state -> codefile, "loop") < 0) builder_throw("could not declare local label '.loop'");
  inst(INST_PUSH, reg(temp));
  unsigned first_patch_entry;
  if (!(builder_state -> options -> no_source_patches)) {
    builder_state -> needed_functions.full_patch_list = 1;
    first_patch_entry = 1;
  } else
    first_patch_entry = builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE];
  inst(INST_SUBTRACT, reg(temp), reg(argument), imm(first_patch_entry));
  inst(INST_SHIFTLEFT2, reg(temp), imm(3));
  inst(INST_ADD2, reg(temp), lbl(patch_list, "PatchList"));
  inst(INST_GETWORDINC, reg(argument), reg(temp));
  inst(INST_GETWORD, reg(temp), reg(temp));
  inst(INST_IFEQ, reg(argument), reg(file), loc("run_patch"));
  inst(INST_IFNE, reg(argument), imm(-1), loc("loop"));
  inst(INST_MULACUM, reg(temp), reg(file), imm(4));
  inst(INST_GETWORD, reg(temp), reg(temp));
  if (add_local_label_to_codefile(builder_state -> codefile, "run_patch") < 0) builder_throw("could not declare local label '.run_patch'");
  inst(INST_SET, reg(argument), reg(temp));
  if (add_local_label_to_codefile(builder_state -> codefile, "next_patch") < 0) builder_throw("could not declare local label '.next_patch'");
  inst(INST_CALL, lbl(apply_patch, "ApplyPatch"));
  inst(INST_POP, reg(argument));
  inst(INST_JUMPNZ, reg(argument), loc("next_patch"));
  inst(INST_RETURN);
  add_blank_line_to_codefile(builder_state -> codefile);
}

void define_generate_reverse_output_function (void) {
  add_declared_label_to_codefile(builder_state -> codefile, get_label(generate_reverse_output, "GenerateReverseOutput"));
  unsigned first_patch_entry;
  if (!(builder_state -> options -> no_source_patches)) {
    builder_state -> needed_functions.full_patch_list = 1;
    first_patch_entry = 1;
  } else
    first_patch_entry = builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE];
  inst(INST_SUBTRACT, reg(temp), reg(file), imm(first_patch_entry));
  inst(INST_SHIFTLEFT2, reg(temp), imm(3));
  inst(INST_ADD2, reg(temp), lbl(patch_list, "PatchList"));
  inst(INST_GETWORDINC, reg(file), reg(temp));
  inst(INST_GETWORD, reg(temp), reg(temp));
  inst(INST_IFEQ, reg(file), imm(-1), loc("last_patch"));
  inst(INST_PUSH, reg(argument));
  inst(INST_SET, reg(argument), reg(temp));
  builder_state -> needed_functions.apply_reverse_patch = 1;
  inst(INST_CALL, lbl(apply_reverse_patch, "ApplyReversePatch"));
  inst(INST_POP, reg(argument));
  inst(INST_IFNE, reg(file), reg(argument), lbl(generate_reverse_output, "GenerateReverseOutput"));
  inst(INST_RETURN);
  add_blank_line_to_codefile(builder_state -> codefile);
  if (add_local_label_to_codefile(builder_state -> codefile, "last_patch") < 0) builder_throw("could not declare local label '.last_patch'");
  inst(INST_MULACUM, reg(temp), reg(argument), imm(4));
  inst(INST_GETWORD, reg(argument), reg(temp));
  inst(INST_JUMP, lbl(apply_reverse_patch, "ApplyReversePatch"));
  add_blank_line_to_codefile(builder_state -> codefile);
}
