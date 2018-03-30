#include "proto.h"

/*
   No fragmentation, no padding:
     4 bytes length, pointer to data (for reversibility)
   No fragmentation, padding:
     4 bytes length, 4 bytes data length, pointer to data
   Fragmentation: (assumes padding = 0 if missing)
     4 bytes length, pointer to fragment table
     fragment table: 4 bytes data length, pointer to data (if length != 0)
   Fragmentation with detect fragment permutation:
     4 bytes length, pointer to fragment table, fragment permutation table
   Files not multiple of the fragment size are rounded up by zero-padding prior to patching
*/

char * write_xor_patch_data (CodeFile codefile, Buffer source, Buffer target, const struct patching_flags * flags) {
  return write_xor_like_patch(codefile, source, target, flags, &write_xor_patch_buffer_data);
}

char * write_xor_patch_buffer_data (CodeFile codefile, const unsigned char * data, unsigned length) {
  // this wrapper is needed because of the differing return types
  add_data_to_codefile(codefile, data, length);
  return NULL;
}
