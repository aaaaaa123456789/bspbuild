#include <stdio.h>

struct code_file_info {
  FILE * fp;
  char * label_prefix;
  char * register_prefix;
  char * constant_prefix;
  char ** labels;
  char ** registers;
  char ** constants;
  unsigned label_count;
  unsigned register_count;
  unsigned constant_count;
  unsigned next_numeric_local;
  unsigned next_numeric_data;
  // ...
};
