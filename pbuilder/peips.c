#include "proto.h"

void define_apply_ips_patch_function (void) {
	inst(INST_JUMP, loc("handle_loop"));
	builder_declare_local("loop");
	inst(INST_SEEK, reg(temp));
	inst(INST_GETWORDINC, reg(temp), reg(argument));
	inst(INST_IPSPATCH, reg(temp), reg(temp));
	builder_declare_local("handle_loop");
	inst(INST_GETWORDINC, reg(temp), reg(argument));
	inst(INST_IFNE, reg(temp), imm(-1), loc("loop"));
	inst(INST_GETWORD, reg(temp), reg(argument));
  inst(INST_TRUNCATE, reg(temp));
}
