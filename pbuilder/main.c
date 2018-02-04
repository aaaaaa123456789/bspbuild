#include "proto.h"

char * generate_patch_code (Options options, CodeFile codefile) {
  builder_state = calloc(1, sizeof(struct code_generation_state));
  builder_state -> options = options;
  builder_state -> codefile = codefile;
  builder_error = NULL;
  int rv = setjmp(builder_return_point);
  if (!rv) {
    initialize_code_generator();
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
