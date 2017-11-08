#include "proto.h"

unsigned get_segment_length (const char * first_buffer, const char * second_buffer, unsigned length, int kind) {
  // 0: differing, 1: equal
  unsigned pos;
  for (pos = 0; (pos < length) && ((first_buffer[pos] == second_buffer[pos]) == kind); pos ++);
  return pos;
}

int check_runs (const char * data, unsigned length) {
  unsigned pos, cmp;
  if (length < MINIMUM_RUN) return -1;
  for (pos = 0; pos <= (length - MINIMUM_RUN); pos ++) {
    for (cmp = 0; (cmp < MINIMUM_RUN) && (data[pos] == data[pos + cmp]); cmp ++);
    if (cmp >= MINIMUM_RUN) return pos;
  }
  return -1;
}
