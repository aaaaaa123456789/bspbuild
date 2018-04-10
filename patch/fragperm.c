#include "proto.h"

char * generate_fragment_permutation_table (Buffer source, Buffer target, const struct patching_flags * flags, struct fragment_permutation_table ** table) {
  // ...
}

char * generate_identity_fragment_permutation_table (unsigned source_length, unsigned target_length, const struct patching_flags * flags,
                                                     struct fragment_permutation_table ** table) {
  unsigned source_fragments = source_length ? 1 + (source_length - 1) / (flags -> fragment_size) : 0;
  unsigned target_fragments = target_length ? 1 + (target_length - 1) / (flags -> fragment_size) : 0;
  *table = create_fragment_permutation_table(source_fragments, target_fragments, flags -> reversible_patch);
  unsigned fragment;
  for (fragment = 0; fragment < target_fragments; fragment ++)
    fragment[(**table).source_to_target_fragments] = (fragment >= source_fragments) ? -1 : fragment;
  if (flags -> reversible_patch) for (fragment = 0; fragment < source_fragments; fragment ++)
    fragment[(**table).target_to_source_fragments] = (fragment >= target_fragments) ? -1 : fragment;
  return NULL;
}

struct fragment_permutation_table * create_fragment_permutation_table (unsigned source_fragments, unsigned target_fragments, int reversible) {
  struct fragment_permutation_table * result = malloc(sizeof(struct fragment_permutation_table));
  result -> source_fragments = source_fragments;
  result -> target_fragments = target_fragments;
  result -> source_to_target_fragments = malloc(sizeof(int) * target_fragments);
  if (reversible)
    result -> target_to_source_fragments = malloc(sizeof(int) * source_fragments);
  else
    result -> target_to_source_fragments = NULL;
  return result;
}

void destroy_fragment_permutation_table (struct fragment_permutation_table * table) {
  free(table -> target_to_source_fragments);
  free(table -> source_to_target_fragments);
  free(table);
}

char * write_fragment_permutation_table_to_codefile (CodeFile codefile, const struct fragment_permutation_table * table, int reverse) {
  // ...
}
