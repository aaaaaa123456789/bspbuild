#include "proto.h"

char * bsp_main (const char * output, const char * input, ...) {
  bsp_error = NULL;
  int rv = setjmp(bsp_return_point);
  if (rv) return bsp_error;
  // ...
}

void bsp_throw_error (const char * fmtstring, ...) {
  va_list ap;
  va_start(ap, fmtstring);
  char * error = generate_string_from_varargs(fmtstring, ap);
  va_end(ap);
  bsp_error = malloc(strlen(error) + 8);
  if (current_file)
    sprintf(bsp_error, "%s:%u: %s", current_file, current_line, error);
  else
    sprintf(bsp_error, "error: %s", error);
  free(error);
  longjmp(bsp_return_point, 1);
}
