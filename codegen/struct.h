#include <stdio.h>

struct code_file_info {
  FILE * fp;
  char * label_prefix;
  char * register_prefix;
  char * constant_prefix;
  char ** labels;
  unsigned label_count;
  unsigned next_numeric_local;
  unsigned next_numeric_data;
  // ...
};
