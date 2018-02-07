#include "proto.h"

char * generate_patch_code (Options options, CodeFile codefile) {
  initialize_builder_state(options, codefile);
  builder_error = NULL;
  int rv = setjmp(builder_return_point);
  if (!rv) {
    initialize_code_generator();
    generate_runtime();
    // ...
  }
  // ...
}

void builder_throw (const char * error, ...) {
  va_list ap;
  va_start(ap, error);
  builder_error = generate_string_from_varargs(error, ap);
  va_end(ap);
  longjmp(builder_return_point, 1);
}
