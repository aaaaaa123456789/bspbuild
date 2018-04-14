#include "proto.h"

/*
   Fragment permutation table format:
   data stored in 32-bit words, top bit: command, 31 bits: fragment affected
   top bit 1: clear fragment (1 word, payload indicates fragment ID)
   top bit 0: copy/swap (2 words, first word payload indicates source, second word payload indicates target, second word command is 0 for copy, 1 for swap)
   -1 ends list
*/

char * write_fragment_permutation_table_to_codefile (CodeFile codefile, const struct fragment_permutation_table * table, int reverse) {
  // ...
}
