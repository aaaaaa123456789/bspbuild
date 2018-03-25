#include "proto.h"

char * generate_fragment_permutation_table (Buffer source, Buffer target, const struct patching_flags * flags, struct fragment_permutation_table ** table) {
  // ...
}

char * generate_identity_fragment_permutation_table (unsigned source_length, unsigned target_length, const struct patching_flags * flags,
                                                     struct fragment_permutation_table ** table) {
  // ...
}

char * write_fragment_permutation_table_to_codefile (CodeFile codefile, const struct fragment_permutation_table * table, int reverse) {
  // ...
}

void destroy_fragment_permutation_table (struct fragment_permutation_table * table) {
  // ...
}
