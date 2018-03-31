#include "proto.h"

/*
   Format is the same as for plain XOR patches
   RLE data format:
   1 byte command:
   bits 7-6: command (0: data, 1: repeat byte, 2: repeat halfword, 3: repeat word)
   bit 5: offset/long toggle (for data: 0: data, 1: repeat doubleword; for others: 0: repeat, 1: repeat and increment)
   bits 4-0: length (0: additional byte follows indicating length + 32)
   length for repeat commands is in number of values, not bytes; for repeat commands, length = 1, 2 indicates a byte follows for length + 0x120, 0x220
   for repeat and increment commands, the initial value follows, and then then increment; increment = 0 means the value is negated (NOT) at every step
   valid length ranges: data: 1 .. 287, repeat: 3 .. 799
*/

char * write_xor_rle_patch_data (CodeFile codefile, Buffer source, Buffer target, const struct patching_flags * flags) {
  return write_xor_like_patch(codefile, source, target, flags, &write_xor_rle_patch_buffer_data);
}

char * write_xor_rle_patch_buffer_data (CodeFile codefile, const unsigned char * data, unsigned length) {
  // ...
}
