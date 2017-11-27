#include <stdio.h>

#include "pubtypes.h"

struct code_file_info {
  char * label_prefix;
  char * register_prefix;
  char * constant_prefix;
  char ** labels;
  char ** registers;
  char ** constants;
  char ** lines;
  unsigned line_count;
  unsigned label_count;
  unsigned register_count;
  unsigned constant_count;
  unsigned next_numeric_local;
  unsigned next_numeric_data;
};

struct code_generation_instruction_entry {
  const char * name;
  unsigned char variable_operands;
  unsigned char any_operands;
};
