#include "proto.h"

char * bsp_main (const char * output, const char * input, ...) {
  int rv = setjmp(bsp_return_point);
  if (rv) return bsp_error();
  // ...
}

void bsp_error (void) {
  // ...
}
