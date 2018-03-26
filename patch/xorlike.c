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
  if (flags -> reversible_patch) {
    if (result = write_patch_data_to_codefile(codefile, 0, NULL, "reverse")) return result;
    add_blank_line_to_codefile(codefile);
  }
  unsigned source_fragment_count = source -> length ? 1 + (source -> length - 1) / flags -> fragment_size : 0;
  unsigned target_fragment_count = target -> length ? 1 + (target -> length - 1) / flags -> fragment_size : 0;
  unsigned fragment, p;
  int * forward_fragment_labels = malloc(sizeof(int) * target_fragment_count);
  int * reverse_fragment_labels = NULL;
  for (fragment = 0; fragment < target_fragment_count; fragment ++) {
    if ((forward_fragment_labels[fragment] = declare_numeric_local_for_codefile(codefile)) >= 0) continue;
    free(forward_fragment_labels);
    return duplicate_string("could not declare local labels for fragments");
  }
  if (flags -> reversible_patch) {
    reverse_fragment_labels = malloc(sizeof(int) * source_fragment_count);
    for (fragment = 0; fragment < source_fragment_count; fragment ++) reverse_fragment_labels[fragment] = -1;
  }
  unsigned * forward_fragment_lengths = malloc(sizeof(unsigned) * target_fragment_count);
  unsigned * reverse_fragment_lengths = NULL;
  if (flags -> reversible_patch) reverse_fragment_lengths = malloc(sizeof(unsigned) * source_fragment_count);
  void * source_fragment_data = generate_last_fragment_data(source, flags -> fragment_size);
  void * target_fragment_data = generate_last_fragment_data(target, flags -> fragment_size);
  void * null_fragment_data = calloc(1, flags -> fragment_size);
  for (fragment = 0; fragment < target_fragment_count; fragment ++) {
    if (flags -> reversible_patch && (fragment_table -> target_to_source_fragments[fragment_table -> source_to_target_fragments[fragment]] == fragment)) {
      reverse_fragment_labels[fragment_table -> source_to_target_fragments[fragment]] = forward_fragment_labels[fragment];
      p = 1;
    } else
      p = 0;
    forward_fragment_lengths[fragment] = calculate_fragment_length(
      select_fragment_data(fragment_table -> source_to_target_fragments[fragment], source, source_fragment_data, null_fragment_data, flags -> fragment_size),
      select_fragment_data(fragment, target, target_fragment_data, NULL, flags -> fragment_size), flags, p);
    if (p) reverse_fragment_lengths[fragment_table -> source_to_target_fragments[fragment]] = forward_fragment_lengths[fragment];
  }
  result = write_xor_like_fragmented_header(codefile, target -> length, 0, forward_fragment_labels, forward_fragment_lengths, flags, fragment_table);
  if (result) goto done;
  add_blank_line_to_codefile(codefile);
  // ...
  done:
  free(source_fragment_data);
  free(target_fragment_data);
  free(null_fragment_data);
  free(forward_fragment_lengths);
  free(reverse_fragment_lengths);
  free(forward_fragment_labels);
  free(reverse_fragment_labels);
  return result;
}

char * write_xor_like_fragmented_header (CodeFile codefile, unsigned length, int reverse, const int * fragment_labels, const unsigned * fragment_lengths,
                                         const struct patching_flags * flags, const struct fragment_permutation_table * fragment_table) {
  // ...
}

unsigned calculate_fragment_length (const unsigned char * source, const unsigned char * target, const struct patching_flags * flags, int reversible) {
  // ...
}

void * generate_last_fragment_data (Buffer buffer, unsigned fragment_size) {
  unsigned remainder = buffer -> length % fragment_size;
  if (!remainder) return NULL;
  char * result = malloc(fragment_size);
  memcpy(result, buffer -> data + (buffer -> length - remainder), remainder);
  memset(result + remainder, 0, fragment_size - remainder);
  return result;
}

const void * select_fragment_data (int selector, Buffer buffer, const void * last_fragment, const void * null_fragment, unsigned fragment_size) {
  if (selector < 0) return null_fragment;
  unsigned last_fragment_ID = buffer -> length / fragment_size; // only accurate if the length is not a multiple of fragment_size
  if (last_fragment && (selector == last_fragment_ID)) return last_fragment;
  return buffer -> data + selector * fragment_size;
}
