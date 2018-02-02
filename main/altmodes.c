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
  char * error;
  FILE * source = open_binary_file(options -> input_files -> name, &error);
  FILE * target = NULL;
  if (!error) target = open_binary_file(options -> input_files[1].name, &error);
  if (error) {
    if (source) fclose(source);
    fprintf(stderr, "error: %s\n", error);
    free(error);
    return 2;
  }
  unsigned long long source_length = get_file_length(source), target_length = get_file_length(target);
  error = NULL;
  if (source_length > MAX_IPS_PATCH_SIZE)
    error = "source file is too large";
  else if (target_length > MAX_IPS_PATCH_SIZE)
    error = "target file is too large";
  else if (source_length > target_length)
    error = "source file is larger than target file";
  if (error) {
    fprintf(stderr, "error: %s\n", error);
    fclose(source);
    fclose(target);
    return 2;
  }
  // ...
}
