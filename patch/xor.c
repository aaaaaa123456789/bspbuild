#include "proto.h"

/*
   No fragmentation, no padding:
     4 bytes length, pointer to data (for reversibility)
   No fragmentation, padding:
     4 bytes length, 4 bytes data length, pointer to data
   Fragmentation: (assumes padding = 0 if missing)
     4 bytes length, pointer to fragment table
     fragment table: 4 bytes data length, pointer to data
   Fragmentation with detect fragment permutation enabled is preceded by pointer to permutation table
   Files not multiple of the fragment size are rounded up by zero-padding prior to patching
*/

char * write_xor_patch_data (CodeFile codefile, Buffer source, Buffer target, const struct patching_flags * flags) {
  if (!flags -> fragmentation_enabled) return write_xor_patch_data_without_fragments(codefile, source, target, flags);
  struct fragment_permutation_table * fragment_table;
  char * result;
  if (flags -> fragment_reordering)
    result = generate_fragment_permutation_table(source, target, flags, &fragment_table);
  else
    result = generate_identity_fragment_permutation_table(source -> length, target -> length, flags, &fragment_table);
  if (result) return result;
  result = write_xor_patch_data_with_fragments(codefile, source, target, flags, fragment_table);
  destroy_fragment_permutation_table(fragment_table);
  return result;
}

char * write_xor_patch_data_without_fragments (CodeFile codefile, Buffer source, Buffer target, const struct patching_flags * flags) {
  // ...
}

char * write_xor_patch_data_with_fragments (CodeFile codefile, Buffer source, Buffer target, const struct patching_flags * flags,
                                            const struct fragment_permutation_table * fragment_table) {
  // ...
}
