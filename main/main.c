#include "proto.h"

int main (int argc, char ** argv) {
  Options options = parse_options(argv + 1, argc - 1);
  int exit_status;
  if (options -> response) {
    fprintf(stderr, "%s\n", options -> response);
    exit_status = EXIT_STATUS_OK;
  } else if (options -> error_text) {
    fprintf(stderr, "%s: error: %s\n", *argv, options -> error_text);
    exit_status = EXIT_STATUS_INVALID_OPTIONS;
  } else
    exit_status = ((int (* []) (Options, const char *)) {
      [OPERATION_MODE_NORMAL] = &normal_operation_mode,
      [OPERATION_MODE_BSP_INPUT] = &bsp_input_operation_mode,
      [OPERATION_MODE_IPS_OUTPUT] = &ips_output_operation_mode
    })[options -> operation_mode](options, *argv);
  destroy_options_object(options);
  return exit_status;
}

int normal_operation_mode (Options options, const char * executable_name) {
  CodeFile cf;
  if (options -> prefixes.label || options -> prefixes.constant || options -> prefixes.variable)
    cf = new_codefile_with_prefixes(options -> prefixes.label, options -> prefixes.variable, options -> prefixes.constant);
  else if (options -> prefixes.global)
    cf = new_codefile_with_prefix(options -> prefixes.global);
  else
    cf = new_codefile();
  if (!cf) {
    fprintf(stderr, "%s: error: invalid prefix specified\n", executable_name);
    return EXIT_STATUS_INVALID_OPTIONS;
  }
  char * error = generate_patch_code(options, cf);
  if (error) {
    destroy_codefile(cf);
    fprintf(stderr, "%s: error: %s\n", executable_name, error);
    free(error);
    return EXIT_STATUS_ERROR;
  }
  Buffer code = convert_codefile_to_data(cf);
  FILE * tempfile = NULL;
  int rv;
  if (options -> output_files.source) {
    error = write_buffer_to_new_file(options -> output_files.source, code);
    free(code);
    if (error) {
      fprintf(stderr, "%s: error: %s\n", executable_name, error);
      free(error);
      return EXIT_STATUS_ERROR;
    }
  } else {
    tempfile = tmpfile();
    if (!tempfile) {
      free(code);
      fprintf(stderr, "%s: error: could not create temporary file\n", executable_name);
      return EXIT_STATUS_ERROR;
    }
    rv = write_data_to_file(tempfile, code -> data, code -> length);
    free(code);
    if (!rv) {
      fclose(tempfile);
      fprintf(stderr, "%s: error: could not write to temporary file\n", executable_name);
      return EXIT_STATUS_ERROR;
    }
    rewind(tempfile);
  }
  if (!(options -> output_files.compiled)) return EXIT_STATUS_OK;
  if (tempfile) {
    error = bsp_build_temporary_file(options -> output_files.compiled, tempfile);
    fclose(tempfile);
  } else
    error = bsp_build_file(options -> output_files.compiled, options -> output_files.source);
  if (error) {
    fprintf(stderr, "%s: %s\n", executable_name, error);
    free(error);
    return EXIT_STATUS_ERROR;
  }
  return EXIT_STATUS_OK;
}
