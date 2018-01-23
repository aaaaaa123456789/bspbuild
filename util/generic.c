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

void destroy_string_array (char ** array, unsigned count) {
  unsigned p;
  for (p = 0; p < count; p ++) free(array[p]);
  free(array);
}

unsigned check_nulls (const void * buffer, unsigned length) {
  const char * p = buffer;
  unsigned pos;
  for (pos = 0; pos < length; pos ++) if (!p[pos]) return pos + 1;
  return 0;
}

void sort_number_array (unsigned * array, unsigned size) {
  qsort(array, size, sizeof(unsigned), &compare_numbers);
}

int compare_numbers (const void * first, const void * second) {
  const unsigned * n1 = first;
  const unsigned * n2 = second;
  return (n1 > n2) - (n1 < n2);
}
