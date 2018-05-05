#include "proto.h"

void define_apply_xor_rle_patch_helper_function (void) {
  // receives: #argument: pointer to patch, #result: bytes to patch; returns actually patched bytes in #result
  add_declared_label_to_codefile(builder_state -> codefile, get_label(apply_xor_rle_patch, "ApplyXORRLEPatch"));
  inst(INST_RETZ, reg(result));
  inst(INST_SET, reg(temp), reg(result));
  inst(INST_XOR2, reg(result), reg(result));
  builder_declare_local("loop");
  inst(INST_PUSH, reg(temp));
  inst(INST_PUSH, reg(result));
  inst(INST_CALL, loc("do_command"));
  inst(INST_POP, reg(temp));
  inst(INST_ADD2, reg(result), reg(temp));
  inst(INST_POP, reg(temp));
  inst(INST_IFLT, reg(result), reg(temp), loc("loop"));
  inst(INST_RETURN);
  add_blank_line_to_codefile(builder_state -> codefile);
  builder_declare_local("do_command");
  inst(INST_GETBYTEINC, reg(result), reg(argument));
  inst(INST_IFGE, reg(result), imm(0x20), loc("repeat_command"));
  inst(INST_JUMPNZ, reg(result), loc("got_data_length"));
  inst(INST_GETBYTEINC, reg(result), reg(argument));
  inst(INST_ADD2, reg(result), imm(0x20));
  builder_declare_local("got_data_length");
  inst(INST_XORDATA, reg(argument), reg(result));
  inst(INST_ADD2, reg(argument), reg(result));
  inst(INST_RETURN);
  add_blank_line_to_codefile(builder_state -> codefile);
  builder_declare_local("repeat_command");
  inst(INST_SHIFTRIGHT, reg(temp), reg(result), imm(5));
  inst(INST_AND2, reg(result), imm(0x1f));
  inst(INST_IFGE, reg(result), imm(3), loc("got_repeat_length"));
  inst(INST_PUSH, reg(temp));
  inst(INST_GETBYTEINC, reg(temp), reg(argument));
  inst(INST_SHIFTLEFT2, reg(result), imm(8));
  inst(INST_ADD2, reg(result), imm(0x20));
  inst(INST_ADD2, reg(result), reg(temp));
  inst(INST_POP, reg(temp));
  builder_declare_local("got_repeat_length");
  inst(INST_DECREMENT, reg(temp));
  inst(INST_PUSH, reg(temp));
  inst(INST_PUSH, reg(result));
  inst(INST_CALL, loc("do_repeat"));
  inst(INST_POP, reg(result));
  inst(INST_POP, reg(temp));
  inst(INST_DECREMENT, reg(temp));
  inst(INST_SHIFTRIGHT2, reg(temp), imm(1));
  inst(INST_AND2, reg(temp), imm(3));
  inst(INST_SHIFTLEFT2, reg(result), reg(temp));
  inst(INST_RETURN);
  add_blank_line_to_codefile(builder_state -> codefile);
  builder_declare_local("do_repeat");
  inst(INST_JUMPTABLE, reg(temp));
  inst(INST_DW, loc("repeat_doubleword"));
  inst(INST_DW, loc("repeat_byte"));
  inst(INST_DW, loc("repeat_byte_with_offset"));
  inst(INST_DW, loc("repeat_halfword"));
  inst(INST_DW, loc("repeat_halfword_with_offset"));
  inst(INST_DW, loc("repeat_word"));
  inst(INST_DW, loc("repeat_word_with_offset"));
  add_blank_line_to_codefile(builder_state -> codefile);
  builder_declare_local("repeat_byte_with_offset");
  inst(INST_GETBYTEINC, reg(temp), reg(argument));
  inst(INST_PUSH, reg(argument));
  inst(INST_GETBYTE, reg(argument), reg(argument));
  inst(INST_JUMP, loc("repeat_byte_loop"));
  add_blank_line_to_codefile(builder_state -> codefile);
  builder_declare_local("repeat_byte");
  inst(INST_PUSH, reg(argument));
  inst(INST_GETBYTE, reg(temp), reg(argument));
  inst(INST_XOR2, reg(argument), reg(argument));
  builder_declare_local("repeat_byte_loop");
  inst(INST_PUSH, reg(result));
  inst(INST_GETFILEBYTE, reg(result));
  inst(INST_XOR2, reg(result), reg(temp));
  inst(INST_ADD2, reg(temp), reg(argument));
  inst(INST_WRITEBYTE, reg(result));
  inst(INST_POP, reg(result));
  inst(INST_DECREMENT, reg(result));
  inst(INST_JUMPNZ, reg(result), loc("repeat_byte_loop"));
  inst(INST_POP, reg(argument));
  inst(INST_INCREMENT, reg(argument));
  inst(INST_RETURN);
  add_blank_line_to_codefile(builder_state -> codefile);
  builder_declare_local("repeat_halfword_with_offset");
  inst(INST_GETHALFWORDINC, reg(temp), reg(argument));
  inst(INST_PUSH, reg(argument));
  inst(INST_GETHALFWORD, reg(argument), reg(argument));
  inst(INST_JUMP, loc("repeat_halfword_loop"));
  add_blank_line_to_codefile(builder_state -> codefile);
  builder_declare_local("repeat_halfword");
  inst(INST_PUSH, reg(argument));
  inst(INST_GETHALFWORD, reg(temp), reg(argument));
  inst(INST_XOR2, reg(argument), reg(argument));
  builder_declare_local("repeat_halfword_loop");
  inst(INST_PUSH, reg(result));
  inst(INST_GETFILEHALFWORD, reg(result));
  inst(INST_XOR2, reg(result), reg(temp));
  inst(INST_ADD2, reg(temp), reg(argument));
  inst(INST_WRITEHALFWORD, reg(result));
  inst(INST_POP, reg(result));
  inst(INST_DECREMENT, reg(result));
  inst(INST_JUMPNZ, reg(result), loc("repeat_halfword_loop"));
  inst(INST_POP, reg(argument));
  inst(INST_ADD2, reg(argument), imm(2));
  inst(INST_RETURN);
  add_blank_line_to_codefile(builder_state -> codefile);
  builder_declare_local("repeat_word_with_offset");
  inst(INST_GETWORDINC, reg(temp), reg(argument));
  inst(INST_PUSH, reg(argument));
  inst(INST_GETWORD, reg(argument), reg(argument));
  inst(INST_JUMP, loc("repeat_word_loop"));
  add_blank_line_to_codefile(builder_state -> codefile);
  builder_declare_local("repeat_word");
  inst(INST_PUSH, reg(argument));
  inst(INST_GETWORD, reg(temp), reg(argument));
  inst(INST_XOR2, reg(argument), reg(argument));
  builder_declare_local("repeat_word_loop");
  inst(INST_PUSH, reg(result));
  inst(INST_GETFILEWORD, reg(result));
  inst(INST_XOR2, reg(result), reg(temp));
  inst(INST_ADD2, reg(temp), reg(argument));
  inst(INST_WRITEWORD, reg(result));
  inst(INST_POP, reg(result));
  inst(INST_DECREMENT, reg(result));
  inst(INST_JUMPNZ, reg(result), loc("repeat_word_loop"));
  inst(INST_POP, reg(argument));
  inst(INST_ADD2, reg(argument), imm(4));
  inst(INST_RETURN);
  add_blank_line_to_codefile(builder_state -> codefile);
  builder_declare_local("repeat_doubleword");
  inst(INST_PUSH, reg(argument));
  inst(INST_GETWORDINC, reg(temp), reg(argument));
  inst(INST_GETWORD, reg(argument), reg(argument));
  builder_declare_local("repeat_doubleword_loop");
  inst(INST_PUSH, reg(result));
  inst(INST_GETFILEWORD, reg(result));
  inst(INST_XOR2, reg(result), reg(temp));
  inst(INST_WRITEWORD, reg(result));
  inst(INST_GETFILEWORD, reg(result));
  inst(INST_XOR2, reg(result), reg(argument));
  inst(INST_WRITEWORD, reg(result));
  inst(INST_POP, reg(result));
  inst(INST_DECREMENT, reg(result));
  inst(INST_JUMPNZ, reg(result), loc("repeat_doubleword_loop"));
  inst(INST_POP, reg(argument));
  inst(INST_ADD2, reg(argument), imm(8));
  inst(INST_RETURN);
  add_blank_line_to_codefile(builder_state -> codefile);
}