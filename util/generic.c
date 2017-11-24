#include "proto.h"

unsigned value_in_list (unsigned value, ...) {
  unsigned count = 0;
  va_list ap;
  va_start(ap, value);
  unsigned current;
  while ((current = va_arg(ap, unsigned)) != -1u) {
    count ++;
    if (current == value) break;
  }
  va_end(ap);
  if (current == -1u) return 0;
  return count;
}
