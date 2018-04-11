#include "proto.h"

char * generate_fragment_permutation_table (Buffer source, Buffer target, const struct patching_flags * flags, struct fragment_permutation_table ** table) {
  unsigned source_fragments = source -> length ? 1 + (source -> length - 1) / (flags -> fragment_size) : 0;
  unsigned target_fragments = target -> length ? 1 + (target -> length - 1) / (flags -> fragment_size) : 0;
  unsigned ** cost_matrix = generate_fragment_cost_matrix(source, target, flags -> fragment_size);
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

unsigned ** generate_fragment_cost_matrix (Buffer source, Buffer target, unsigned fragment_length) {
  unsigned source_fragments = source -> length ? 1 + (source -> length - 1) / fragment_length : 0;
  unsigned target_fragments = target -> length ? 1 + (target -> length - 1) / fragment_length : 0;
  unsigned ** result = malloc((sizeof(unsigned) * (target_fragments + 1) + sizeof(unsigned *)) * (source_fragments + 1));
  unsigned * row_pointer = (unsigned *) (result + source_fragments + 1);
  unsigned source_fragment, target_fragment;
  const unsigned char * source_pointer = source -> data;
  const unsigned char * target_pointer;
  for (source_fragment = 0; source_fragment < source_fragments; source_fragment ++) {
    result[source_fragment] = row_pointer;
    target_pointer = target -> data;
    for (target_fragment = 0; target_fragment < target_fragments; target_fragment ++) {
      result[source_fragment][target_fragment] = calculate_estimated_fragment_cost(source_pointer, target_pointer, fragment_length);
      target_pointer += fragment_length;
    }
    result[source_fragment][target_fragments] = calculate_estimated_fragment_cost(source_pointer, NULL, fragment_length);
    row_pointer += target_fragments + 1;
    source_pointer += fragment_length;
  }
  result[source_fragments] = row_pointer;
  target_pointer = target -> data;
  for (target_fragment = 0; target_fragment < target_fragments; target_fragment ++) {
    result[source_fragments][target_fragment] = calculate_estimated_fragment_cost(NULL, target_pointer, fragment_length);
    target_pointer += fragment_length;
  }
  result[source_fragments][target_fragments] = calculate_estimated_fragment_cost(NULL, NULL, fragment_length);
  return result;
}

unsigned calculate_estimated_fragment_cost (const unsigned char * source, const unsigned char * target, unsigned length) {
  if (!source) {
    if (!target) return 0;
    return calculate_estimated_fragment_cost(target, source, length);
  }
  unsigned pos, result = 0;
  if (target)
    for (pos = 0; pos < length; pos ++) if (source[pos] == target[pos]) result ++;
  else
    for (pos = 0; pos < length; pos ++) if (!source[pos]) result ++;
  return result;
}
