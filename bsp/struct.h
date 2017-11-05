#include <stdio.h>

struct bsp_symbol {
  unsigned value;
  char name[];
};

struct script_data {
  unsigned length;
  unsigned symbol_count;
  unsigned definition_count;
  unsigned reference_count;
  struct bsp_symbol ** symbols;
  struct bsp_symbol ** definitions;
  struct bsp_symbol ** references;
  char data[];
};

struct file_stack_entry {
  FILE * fp;
  char * name;
  unsigned line;
};

struct bsp_command {
  const char * name;
  int argument;
  void (* parser) (int, char **);
};

struct bsp_argument {
  unsigned char kind; // 0: constant, 1: argument, 2: reference
  unsigned value;
  char reference[];
};
