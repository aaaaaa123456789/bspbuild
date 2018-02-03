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
    return EXIT_STATUS_ERROR;
  }
  return EXIT_STATUS_OK;
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
    return EXIT_STATUS_ERROR;
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
    return EXIT_STATUS_ERROR;
  }
  Buffer source_data = create_buffer_from_size(source_length), target_data = create_buffer_from_size(target_length);
  source_length = fread(source_data -> data, 1, source_length, source);
  target_length = fread(target_data -> data, 1, target_length, target);
  fclose(source);
  fclose(target);
  if ((source_length != source_data -> length) || (target_length != target_data -> length)) {
    fputs("error: could not read from input files\n", stderr);
    free(source_data);
    free(target_data);
    return EXIT_STATUS_ERROR;
  }
  Buffer result = generate_ips_patch_from_buffers(source_data, target_data);
  free(source_data);
  free(target_data);
  if (!result) {
    fputs("error: could not generate IPS patch\n", stderr);
    return EXIT_STATUS_ERROR;
  }
  target = open_binary_file_for_writing(options -> output_files.compiled, &error);
  if (error) {
    fprintf(stderr, "error: %s\n", error);
    free(result);
    return EXIT_STATUS_ERROR;
  }
  int rv = write_data_to_file(target, result -> data, result -> length);
  free(result);
  fclose(target);
  if (!rv) fprintf(stderr, "error: could not write to %s\n", options -> output_files.compiled);
  return rv ? EXIT_STATUS_OK : EXIT_STATUS_ERROR;
}
