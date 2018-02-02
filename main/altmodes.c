#include "proto.h"

int bsp_input_operation_mode (Options options) {
  char ** input_files = malloc(sizeof(char *) * (options -> input_file_count + 1));
  unsigned pos;
  for (pos = 0; pos < options -> input_file_count; pos ++) input_files[pos] = options -> input_files[pos].name;
  input_files[options -> input_file_count] = NULL;
  char * result = bsp_build(options -> output_files.compiled, input_files);
  free(input_files);
  if (result) {
    fprintf(stderr, "%s\n", result);
    free(result);
    return 2;
  }
  return 0;
}

int ips_output_operation_mode (Options options) {
  // ...
}
