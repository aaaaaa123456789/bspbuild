#include "proto.h"

void define_handle_fragment_permutation_function (void) {
  add_declared_label_to_codefile(builder_state -> codefile, get_label(handle_fragment_permutation, "HandleFragmentPermutation"));
  inst(INST_JUMP, loc("handle_loop"));
  builder_declare_local("loop");
  inst(INST_CALL, loc("handle_fragment"));
  builder_declare_local("handle_loop");
  inst(INST_GETWORDINC, reg(temp), reg(argument));
  inst(INST_IFNE, reg(temp), imm(-1), loc("loop"));
  inst(INST_RETURN);
  add_blank_line_to_codefile(builder_state -> codefile);
  builder_declare_local("handle_fragment");
  inst(INST_IFLT, reg(temp), imm(0x80000000), loc("handle_copy_swap"));
  inst(INST_AND2, reg(temp), imm(0x7fffffff));
  inst(INST_MULTIPLY2, reg(temp), cnst(fragment_size));
  inst(INST_SEEK, reg(temp));
  inst(INST_FILLBYTE, cnst(fragment_size), imm(0));
  inst(INST_RETURN);
  builder_declare_local("handle_copy_swap");
  inst(INST_GETWORDINC, reg(result), reg(argument));
  inst(INST_PUSH, reg(argument));
  inst(INST_IFGE, reg(result), imm(0x80000000), loc("handle_swap"));
  inst(INST_MULTIPLY, reg(argument), reg(temp), cnst(fragment_size));
  inst(INST_SEEK, reg(argument));
  inst(INST_IFLT, reg(result), reg(temp), loc("copy_backwards"));
  inst(INST_SUBTRACT, reg(temp), reg(result), reg(temp));
  write_fragment_copy_code(INST_SEEKFWD, INST_SEEKBACK, "forwards_copy_loop");
  builder_declare_local("copy_backwards");
  inst(INST_SUBTRACT2, reg(temp), reg(result));
  write_fragment_copy_code(INST_SEEKBACK, INST_SEEKFWD, "backwards_copy_loop");
  builder_declare_local("handle_swap");
  inst(INST_AND2, reg(result), imm(0x7fffffff));
  inst(INST_IFLT, reg(temp), reg(result), loc("swap_order_OK"));
  inst(INST_SET, reg(argument), reg(temp));
  inst(INST_SET, reg(temp), reg(result));
  inst(INST_SET, reg(result), reg(argument));
  builder_declare_local("swap_order_OK");
  inst(INST_MULTIPLY, reg(argument), reg(temp), cnst(fragment_size));
  inst(INST_SEEK, reg(argument));
  inst(INST_SUBTRACT, reg(temp), reg(result), reg(temp));
  inst(INST_MULTIPLY2, reg(temp), cnst(fragment_size));
  unsigned char byte_repetition;
  for (byte_repetition = 0; byte_repetition < (builder_state -> options -> fragment_size & 3); byte_repetition ++) {
    inst(INST_GETFILEBYTE, reg(result));
    inst(INST_SEEKFWD, reg(temp));
    inst(INST_GETFILEBYTE, reg(argument));
    inst(INST_LOCKPOS);
    inst(INST_WRITEBYTE, reg(result));
    inst(INST_UNLOCKPOS);
    inst(INST_SEEKBACK, reg(temp));
    inst(INST_WRITEBYTE, reg(argument));
  }
  inst(INST_SET, reg(argument), imm(builder_state -> options -> fragment_size >> 2));
  builder_declare_local("swap_loop");
  inst(INST_PUSH, reg(argument));
  inst(INST_GETFILEWORD, reg(result));
  inst(INST_SEEKFWD, reg(temp));
  inst(INST_GETFILEWORD, reg(argument));
  inst(INST_LOCKPOS);
  inst(INST_WRITEWORD, reg(result));
  inst(INST_UNLOCKPOS);
  inst(INST_SEEKBACK, reg(temp));
  inst(INST_WRITEWORD, reg(argument));
  inst(INST_POP, reg(argument));
  inst(INST_DECREMENT, reg(argument));
  inst(INST_JUMPNZ, reg(argument), loc("swap_loop"));
  inst(INST_RETURN);
  add_blank_line_to_codefile(builder_state -> codefile);
}

void write_fragment_copy_code (int move_to_instruction, int move_back_instruction, const char * loop_label) {
  unsigned char byte_repetition;
  inst(INST_MULTIPLY2, reg(temp), cnst(fragment_size));
  for (byte_repetition = 0; byte_repetition < (builder_state -> options -> fragment_size & 3); byte_repetition ++) {
    inst(INST_GETFILEBYTE, reg(result));
    inst(move_to_instruction, reg(temp));
    inst(INST_WRITEBYTE, reg(result));
    inst(move_back_instruction, reg(temp));
  }
  inst(INST_SET, reg(argument), imm(builder_state -> options -> fragment_size >> 2));
  builder_declare_local(loop_label);
  inst(INST_GETFILEWORD, reg(result));
  inst(move_to_instruction, reg(temp));
  inst(INST_WRITEWORD, reg(result));
  inst(move_back_instruction, reg(temp));
  inst(INST_DECREMENT, reg(argument));
  inst(INST_JUMPNZ, loc(loop_label));
  inst(INST_POP, reg(argument));
  inst(INST_RETURN);
}
