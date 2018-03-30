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
  if (flags -> fragmentation_enabled) return write_xor_like_fragmented_patch_data(codefile, source, target, flags, &write_xor_patch_fragment_data);
  char * result = NULL;
  if (flags -> reversible_patch) {
    if (result = write_patch_data_to_codefile(codefile, 0, NULL, "reverse")) return result;
    add_blank_line_to_codefile(codefile);
  }
  unsigned data_length, p;
  if (flags -> padding_length) {
    data_length = calculate_unpadded_data_length(target -> data, target -> length, flags);
    if (flags -> reversible_patch) {
      p = calculate_unpadded_data_length(source -> data, source -> length, flags);
      if (p > data_length) data_length = p;
    }
  } else if (flags -> reversible_patch)
    data_length = (source -> length > target -> length) ? source -> length : target -> length;
  else
    data_length = target -> length;
  // ...
}

char * write_xor_patch_fragment_data (CodeFile codefile, const unsigned char * data, unsigned length) {
  // this wrapper is needed because of the differing return types
  add_data_to_codefile(codefile, data, length);
  return NULL;
}
