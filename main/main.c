#include "proto.h"

int main (int argc, char ** argv) {
  Options options = parse_options(argv + 1, argc - 1);
  if (options -> response) {
    fprintf(stderr, "%s\n", options -> response);
    destroy_options_object(options);
    return 1;
  } else if (options -> error_text) {
    fprintf(stderr, "%s: error: %s\n", *argv, options -> error_text);
    destroy_options_object(options);
    return 1;
  }
  int exit_status = ((int (* [])(Options)) {
    &normal_operation_mode,
    &bsp_input_operation_mode,
    &ips_output_operation_mode
  })[options -> operation_mode](options);
  destroy_options_object(options);
  return exit_status;
}

int normal_operation_mode (Options options) {
  // ...
}
