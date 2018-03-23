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
*/

char * write_xor_patch_data (CodeFile codefile, Buffer source, Buffer target, const struct patching_flags * flags) {
  if (!flags -> fragmentation_enabled) return write_xor_patch_data_without_fragments(codefile, source, target, flags);
  // ...
}

char * write_xor_patch_data_without_fragments (CodeFile codefile, Buffer source, Buffer target, const struct patching_flags * flags) {
  // ...
}
