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
  if (flags -> fragmentation_enabled) return write_xor_like_fragmented_patch_data(codefile, source, target, flags, &write_xor_patch_buffer_data);
  char * result = NULL;
  if (flags -> reversible_patch) {
    if (result = write_patch_data_to_codefile(codefile, 0, NULL, "reverse")) return result;
    add_blank_line_to_codefile(codefile);
  }
  unsigned p, data_length = calculate_xor_like_unfragmented_data_length(source, target, flags);
  if (flags -> padding_length) {
    p = (data_length > target -> length) ? target -> length : data_length;
    result = write_patch_data_to_codefile(codefile, 2, (unsigned []) {target -> length, p}, "data");
  } else
    result = write_patch_data_to_codefile(codefile, 1, &(target -> length), "data");
  if (result) return result;
  add_blank_line_to_codefile(codefile);
  if (flags -> reversible_patch) {
    if (add_local_label_to_codefile(codefile, "reverse") < 0) return duplicate_string("could not declare local label '.reverse'");
    if (flags -> padding_length) {
      p = (data_length > source -> length) ? source -> length : data_length;
      result = write_patch_data_to_codefile(codefile, 2, (unsigned []) {source -> length, p}, "data");
    } else
      result = write_patch_data_to_codefile(codefile, 1, &(source -> length), "data");
    if (result) return result;
    add_blank_line_to_codefile(codefile);
  }
  return write_xor_like_unfragmented_patch_data(codefile, source, target, data_length, "data", &write_xor_patch_buffer_data);
}

char * write_xor_patch_buffer_data (CodeFile codefile, const unsigned char * data, unsigned length) {
  // this wrapper is needed because of the differing return types
  add_data_to_codefile(codefile, data, length);
  return NULL;
}
