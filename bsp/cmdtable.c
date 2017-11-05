#include "struct.h"
#include "commands.h"

struct bsp_command script_commands[] = {
  {"add",               0x20, &calculation_command},
  {"addcarry",          0xb0, &two_variables_two_arguments_command},
  {"align",             0,    &align_command},
  {"and",               0x34, &calculation_command},
  {"bsppatch",          0x94, &one_variable_two_arguments_command},
  {"bufchar",           0xa2, &one_argument_command},
  {"bufnumber",         0xa4, &one_argument_command},
  {"bufstring",         0xa0, &one_argument_command},
  {"call",              0x04, &one_argument_command},
  {"callnz",            0x5e, &one_variable_one_argument_command},
  {"callz",             0x5c, &one_variable_one_argument_command},
  {"checksha1",         0x16, &one_variable_one_argument_command},
  {"clearbuf",          0xa7, &no_arguments_command},
  {"db",                1,    &data_command},
  {"decrement",         0x9f, &one_variable_command},
  {"define",            0,    &define_command},
  {"dh",                2,    &data_command},
  {"divide",            0x2c, &calculation_command},
  {"dw",                4,    &data_command},
  {"exit",              0x06, &one_argument_command},
  {"fillbyte",          0x70, &one_argument_one_byte_argument},
  {"fillhalfword",      0x74, &one_argument_one_halfword_argument},
  {"fillword",          0x78, &two_arguments_command},
  {"getbyte",           0x10, &one_variable_one_argument_command},
  {"getbytedec",        0x9c, &two_variables_command},
  {"getbyteinc",        0x98, &two_variables_command},
  {"getfilebyte",       0xac, &one_variable_command},
  {"getfilehalfword",   0xad, &one_variable_command},
  {"getfileword",       0xae, &one_variable_command},
  {"gethalfword",       0x12, &one_variable_one_argument_command},
  {"gethalfworddec",    0x9d, &two_variables_command},
  {"gethalfwordinc",    0x99, &two_variables_command},
  {"getstacksize",      0xaa, &one_variable_command},
  {"getvariable",       0xaf, &two_variables_command},
  {"getword",           0x14, &one_variable_one_argument_command},
  {"getworddec",        0x9e, &two_variables_command},
  {"getwordinc",        0x9a, &two_variables_command},
  {"hexdata",           0,    &hexdata_command},
  {"ifeq",              0x50, &one_variable_two_arguments_command},
  {"ifge",              0x4c, &one_variable_two_arguments_command},
  {"ifgt",              0x48, &one_variable_two_arguments_command},
  {"ifle",              0x44, &one_variable_two_arguments_command},
  {"iflt",              0x40, &one_variable_two_arguments_command},
  {"ifne",              0x54, &one_variable_two_arguments_command},
  {"incbin",            1,    &include_command},
  {"include",           0,    &include_command},
  {"increment",         0x9b, &one_variable_command},
  {"ipspatch",          0x86, &one_variable_one_argument_command},
  {"jump",              0x02, &one_argument_command},
  {"jumpnz",            0x5a, &one_variable_one_argument_command},
  {"jumptable",         0x83, &one_variable_command},
  {"jumpz",             0x58, &one_variable_one_argument_command},
  {"length",            0x0b, &one_variable_command},
  {"lockpos",           0x80, &no_arguments_command},
  {"longmul",           0xb8, &two_variables_two_arguments_command},
  {"longmulacum",       0xbc, &two_variables_two_arguments_command},
  {"menu",              0x6a, &one_variable_one_argument_command},
  {"mulacum",           0xbc, &mulacum_command},
  {"multiply",          0x28, &calculation_command},
  {"nop",               0x00, &no_arguments_command},
  {"or",                0x38, &calculation_command},
  {"pop",               0x0a, &one_variable_command},
  {"poppos",            0x93, &no_arguments_command},
  {"pos",               0x0f, &one_variable_command},
  {"print",             0x68, &one_argument_command},
  {"printbuf",          0xa6, &no_arguments_command},
  {"push",              0x08, &one_argument_command},
  {"pushpos",           0x92, &no_arguments_command},
  {"readbyte",          0x0c, &one_variable_command},
  {"readhalfword",      0x0d, &one_variable_command},
  {"readword",          0x0e, &one_variable_command},
  {"remainder",         0x30, &calculation_command},
  {"retnz",             0x91, &one_variable_command},
  {"return",            0x01, &no_arguments_command},
  {"retz",              0x90, &one_variable_command},
  {"rotateleft",        2,    &bit_shift_command},
  {"seek",              0x60, &one_argument_command},
  {"seekback",          0x64, &one_argument_command},
  {"seekend",           0x66, &one_argument_command},
  {"seekfwd",           0x62, &one_argument_command},
  {"set",               0x84, &one_variable_one_argument_command},
  {"setstacksize",      0xa8, &one_argument_command},
  {"shiftleft",         0,    &bit_shift_command},
  {"shiftright",        1,    &bit_shift_command},
  {"shiftrightarith",   3,    &bit_shift_command},
  {"stackread",         0x8c, &one_variable_one_argument_command},
  {"stackshift",        0x8e, &one_argument_command},
  {"stackwrite",        0x88, &two_arguments_command},
  {"string",            0,    &string_command},
  {"subborrow",         0xb4, &two_variables_two_arguments_command},
  {"subtract",          0x24, &calculation_command},
  {"truncate",          0x1e, &one_argument_command},
  {"truncatepos",       0x82, &no_arguments_command},
  {"unlockpos",         0x81, &no_arguments_command},
  {"writebyte",         0x18, &one_byte_argument_command},
  {"writedata",         0x7c, &two_arguments_command},
  {"writehalfword",     0x1a, &one_halfword_argument_command},
  {"writeword",         0x1c, &one_argument_command},
  {"xor",               0x3c, &calculation_command},
  {"xordata",           0x6c, &two_arguments_command},
  {NULL,                0,    NULL}
};
