#include "struct.h"

const struct code_generation_instruction_entry codegen_instruction_table[] = {
  {"add",             1, 2},
  {"add",             1, 1},
  {"addcarry",        2, 2},
  {"and",             1, 2},
  {"and",             1, 1},
  {"bsppatch",        1, 2},
  {"bufchar",         0, 1},
  {"bufnumber",       0, 1},
  {"bufstring",       0, 1},
  {"call",            0, 1},
  {"callnz",          1, 1},
  {"callz",           1, 1},
  {"checksha1",       1, 1},
  {"clearbuf",        0, 0},
  {"decrement",       1, 0},
  {"divide",          1, 2},
  {"divide",          1, 1},
  {"exit",            0, 1},
  {"fillbyte",        0, 2},
  {"fillhalfword",    0, 2},
  {"fillword",        0, 2},
  {"getbyte",         1, 1},
  {"getbytedec",      2, 0},
  {"getbyteinc",      2, 0},
  {"getfilebyte",     1, 0},
  {"getfilehalfword", 1, 0},
  {"getfileword",     1, 0},
  {"gethalfword",     1, 1},
  {"gethalfworddec",  2, 0},
  {"gethalfwordinc",  2, 0},
  {"getstacksize",    1, 0},
  {"getvariable",     2, 0},
  {"getword",         1, 1},
  {"getworddec",      2, 0},
  {"getwordinc",      2, 0},
  {"ifeq",            1, 2},
  {"ifge",            1, 2},
  {"ifgt",            1, 2},
  {"ifle",            1, 2},
  {"iflt",            1, 2},
  {"ifne",            1, 2},
  {"increment",       1, 0},
  {"ipspatch",        1, 1},
  {"jump",            0, 1},
  {"jumpnz",          1, 1},
  {"jumptable",       1, 0},
  {"jumpz",           1, 1},
  {"length",          1, 0},
  {"lockpos",         0, 0},
  {"longmul",         2, 2},
  {"longmulacum",     2, 2},
  {"mulacum",         1, 2},
  {"menu",            1, 1},
  {"multiply",        1, 2},
  {"multiply",        1, 1},
  {"nop",             0, 0},
  {"or",              1, 2},
  {"or",              1, 1},
  {"pop",             1, 0},
  {"poppos",          0, 0},
  {"pos",             1, 0},
  {"print",           0, 1},
  {"printbuf",        0, 0},
  {"push",            0, 1},
  {"pushpos",         0, 0},
  {"readbyte",        1, 0},
  {"readhalfword",    1, 0},
  {"readword",        1, 0},
  {"remainder",       1, 2},
  {"remainder",       1, 1},
  {"retnz",           1, 0},
  {"return",          0, 0},
  {"retz",            1, 0},
  {"rotateleft",      1, 2},
  {"rotateleft",      1, 1},
  {"seek",            0, 1},
  {"seekback",        0, 1},
  {"seekend",         0, 1},
  {"seekfwd",         0, 1},
  {"set",             1, 1},
  {"setstacksize",    0, 1},
  {"shiftleft",       1, 2},
  {"shiftleft",       1, 1},
  {"shiftright",      1, 2},
  {"shiftright",      1, 1},
  {"shiftrightarith", 1, 2},
  {"shiftrightarith", 1, 1},
  {"stackread",       1, 1},
  {"stackshift",      0, 1},
  {"stackwrite",      0, 2},
  {"subborrow",       2, 2},
  {"subtract",        1, 2},
  {"subtract",        1, 1},
  {"truncate",        0, 1},
  {"truncatepos",     0, 0},
  {"unlockpos",       0, 0},
  {"writebyte",       0, 1},
  {"writedata",       0, 2},
  {"writehalfword",   0, 1},
  {"writeword",       0, 1},
  {"xor",             1, 2},
  {"xor",             1, 1},
  {"xordata",         0, 2}
};

const char * const codegen_directive_names[] = {
  "db",
  "dh",
  "dw"
};

const unsigned codegen_instruction_count = sizeof codegen_instruction_table / sizeof(struct code_generation_instruction_entry);
const unsigned codegen_directive_count = sizeof codegen_directive_names / sizeof(const char *);
