#include "proto.h"

void define_apply_fragmented_patch_function (void) {
  // inputs: #argument: pointer to patch data, #result: patching callback (takes #argument: data, #result: length, returns effective length)
  builder_state -> needed_functions.pad_to_length = 1;
  add_declared_label_to_codefile(builder_state -> codefile, get_label(apply_fragmented_patch, "ApplyFragmentedPatch"));
  inst(INST_GETWORDINC, reg(temp), reg(argument));
  inst(INST_JUMPNZ, reg(temp), loc("nonzero_length"));
  inst(INST_TRUNCATE, imm(0));
  inst(INST_RETURN);
  add_blank_line_to_codefile(builder_state -> codefile);
  builder_declare_local("nonzero_length");
  inst(INST_PUSH, reg(temp));
  inst(INST_DECREMENT, reg(temp));
  inst(INST_DIVIDE2, reg(temp), cnst(fragment_size));
  inst(INST_INCREMENT, reg(temp));
  inst(INST_PUSH, reg(argument));
  inst(INST_LENGTH, reg(argument));
  inst(INST_JUMPZ, reg(argument), loc("initial_truncation_done"));
  inst(INST_DECREMENT, reg(argument));
  inst(INST_DIVIDE2, reg(argument), cnst(fragment_size));
  inst(INST_INCREMENT, reg(argument));
  inst(INST_IFGE, reg(argument), reg(temp), loc("got_initial_truncation_size"));
  inst(INST_SET, reg(argument), reg(temp));
  builder_declare_local("got_initial_truncation_size");
  inst(INST_MULTIPLY2, reg(argument), cnst(fragment_size));
  inst(INST_TRUNCATE, reg(argument));
  builder_declare_local("initial_truncation_done");
  inst(INST_POP, reg(argument));
  inst(INST_PUSH, reg(temp));
  inst(INST_PUSH, reg(result));
  inst(INST_GETWORDINC, reg(result), reg(argument));
  inst(INST_PUSH, reg(result));
  if (builder_state -> options -> detect_fragment_permutation) {
    builder_state -> needed_functions.handle_fragment_permutation = 1;
    inst(INST_CALL, lbl(handle_fragment_permutation, "HandleFragmentPermutation"));
  }
  add_blank_line_to_codefile(builder_state -> codefile);
  add_comment_to_codefile(builder_state -> codefile, "stack contents:", 1);
  add_comment_to_codefile(builder_state -> codefile, "0: current fragment (not pushed at loop start)", 1);
  add_comment_to_codefile(builder_state -> codefile, "1: fragment table pointer", 1);
  add_comment_to_codefile(builder_state -> codefile, "2: fragment callback", 1);
  add_comment_to_codefile(builder_state -> codefile, "3: number of fragments", 1);
  add_comment_to_codefile(builder_state -> codefile, "4: target file size", 1);
  inst(INST_SET, reg(temp), imm(0));
  builder_declare_local("fragment_loop");
  inst(INST_PUSH, reg(temp));
  inst(INST_MULTIPLY2, reg(temp), cnst(fragment_size));
  inst(INST_SEEK, reg(temp));
  inst(INST_STACKREAD, reg(temp), imm(1));
  inst(INST_GETWORDINC, reg(result), reg(temp));
  inst(INST_JUMPZ, reg(result), loc("no_data_pointer"));
  inst(INST_GETWORDINC, reg(argument), reg(temp));
  builder_declare_local("no_data_pointer");
  inst(INST_STACKWRITE, imm(1), reg(temp));
  inst(INST_STACKREAD, reg(temp), imm(2));
  inst(INST_CALLNZ, reg(result), reg(temp));
  builder_state -> needed_functions.pad_to_length = 1;
  inst(INST_SET, reg(argument), cnst(fragment_size));
  inst(INST_CALL, lbl(pad_to_length, "PadToLength"));
  inst(INST_STACKREAD, reg(argument), imm(3));
  inst(INST_POP, reg(temp));
  inst(INST_INCREMENT, reg(temp));
  inst(INST_IFLT, reg(temp), reg(argument), loc("fragment_loop"));
  inst(INST_STACKSHIFT, imm(-3));
  inst(INST_POP, reg(result));
  inst(INST_TRUNCATE, reg(result));
  inst(INST_RETURN);
  add_blank_line_to_codefile(builder_state -> codefile);
}
