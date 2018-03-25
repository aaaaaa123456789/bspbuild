#include "proto.h"

char * write_xor_like_fragmented_patch_data (CodeFile codefile, Buffer source, Buffer target, const struct patching_flags * flags,
                                             char * (* data_writer) (CodeFile, const unsigned char *, unsigned)) {
  struct fragment_permutation_table * fragment_table;
  char * result;
  if (flags -> fragment_reordering)
    result = generate_fragment_permutation_table(source, target, flags, &fragment_table);
  else
    result = generate_identity_fragment_permutation_table(source -> length, target -> length, flags, &fragment_table);
  if (result) return result;
  result = write_xor_like_patch_data_with_fragments(codefile, source, target, flags, fragment_table, data_writer);
  destroy_fragment_permutation_table(fragment_table);
  return result;
}

char * write_xor_like_patch_data_with_fragments (CodeFile codefile, Buffer source, Buffer target, const struct patching_flags * flags,
                                                 const struct fragment_permutation_table * fragment_table,
                                                 char * (* data_writer) (CodeFile, const unsigned char *, unsigned)) {
  char * result = NULL;
  if (flags -> reversible_patch)
    if (result = write_patch_data_to_codefile(codefile, 0, NULL, "reverse")) return result;
  unsigned source_fragment_count = source -> length ? 1 + (source -> length - 1) / flags -> fragment_size : 0;
  unsigned target_fragment_count = target -> length ? 1 + (target -> length - 1) / flags -> fragment_size : 0;
  unsigned fragment, p;
  int * forward_fragment_labels = malloc(sizeof(int) * target_fragment_count);
  for (fragment = 0; fragment < target_fragment_count; fragment ++) {
    if ((forward_fragment_labels[fragment] = declare_numeric_local_for_codefile(codefile)) >= 0) continue;
    free(forward_fragment_labels);
    return duplicate_string("could not declare local labels for fragments");
  }
  int * reverse_fragment_labels = NULL;
  if (flags -> reversible_patch) {
    reverse_fragment_labels = malloc(sizeof(int) * source_fragment_count);
    for (fragment = 0; fragment < source_fragment_count; fragment ++) reverse_fragment_labels[fragment] = -1;
  }
  unsigned * fragment_lengths = malloc(sizeof(unsigned) * target_fragment_count);
  char * source_fragment_data = NULL;
  if (source -> length % flags -> fragment_size) {
    source_fragment_data = malloc(flags -> fragment_size);
    p = source -> length - (source_fragment_count - 1) * flags -> fragment_size;
    memcpy(source_fragment_data, source -> data + (source_fragment_count - 1) * flags -> fragment_size, p);
    memset(source_fragment_data + p, 0, flags -> fragment_size - p);
  }
  char * target_fragment_data = NULL;
  if (target -> length % flags -> fragment_size) {
    target_fragment_data = malloc(flags -> fragment_size);
    p = target -> length - (target_fragment_count - 1) * flags -> fragment_size;
    memcpy(target_fragment_data, target -> data + (target_fragment_count - 1) * flags -> fragment_size, p);
    memset(target_fragment_data + p, 0, flags -> fragment_size - p);
  }
  char * null_fragment_data = calloc(1, flags -> fragment_size);
  char * source_data;
  char * target_data;
  for (fragment = 0; fragment < target_fragment_count; fragment ++) {
    if (flags -> reversible_patch && (fragment_table -> target_to_source_fragments[fragment_table -> source_to_target_fragments[fragment]] == fragment)) {
      reverse_fragment_labels[fragment_table -> source_to_target_fragments[fragment]] = forward_fragment_labels[fragment];
      p = 1;
    } else
      p = 0;
    if (fragment_table -> source_to_target_fragments[fragment] < 0)
      source_data = null_fragment_data;
    else if (source_fragment_data && (fragment_table -> source_to_target_fragments[fragment] == (source_fragment_count - 1)))
      source_data = source_fragment_data;
    else
      source_data = source -> data + flags -> fragment_size * fragment_table -> source_to_target_fragments[fragment];
    if (target_fragment_data && (fragment == (target_fragment_count - 1)))
      target_data = target_fragment_data;
    else
      target_data = target -> data + flags -> fragment_size * fragment;
    fragment_lengths[fragment] = calculate_fragment_length(source_data, target_data, flags, p);
  }
  // ...
}

unsigned calculate_fragment_length (const unsigned char * source, const unsigned char * target, const struct patching_flags * flags, int reversible) {
  // ...
}
