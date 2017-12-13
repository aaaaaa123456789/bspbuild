#include "proto.h"

Options special_option_response (const char * response) {
  Options result = new_options_object();
  unsigned length = strlen(response) + 1;
  result -> error_text = mr_malloc(result -> memory_region, length);
  memcpy(result -> error_text, response, length);
  return result;
}

Options no_arguments_response (void) {
  return special_option_response("bspbuild - version " VERSION_STRING "\nerror: no arguments given. Use \"bspbuild --help\" for help.\n");
}

Options version_response (void) {
  // ...
}

Options help_response (void) {
  // ...
}
