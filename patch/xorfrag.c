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
  unsigned fragment, p;
  int * forward_fragment_labels = malloc(sizeof(int) * fragment_table -> target_fragments);
  int * reverse_fragment_labels = NULL;
  unsigned char * reverse_fragment_data_needed = NULL;
  for (fragment = 0; fragment < fragment_table -> target_fragments; fragment ++) {
    if ((forward_fragment_labels[fragment] = declare_numeric_local_for_codefile(codefile)) >= 0) continue;
    free(forward_fragment_labels);
    return duplicate_string("could not declare local labels for fragments");
  }
  if (flags -> reversible_patch) {
    reverse_fragment_labels = malloc(sizeof(int) * fragment_table -> source_fragments);
    for (fragment = 0; fragment < fragment_table -> source_fragments; fragment ++) reverse_fragment_labels[fragment] = -1;
    reverse_fragment_data_needed = calloc(1, fragment_table -> source_fragments);
  }
  unsigned * forward_fragment_lengths = malloc(sizeof(unsigned) * fragment_table -> target_fragments);
  unsigned * reverse_fragment_lengths = NULL;
  if (flags -> reversible_patch) reverse_fragment_lengths = malloc(sizeof(unsigned) * fragment_table -> source_fragments);
  void * source_fragment_data = generate_last_fragment_data(source, flags -> fragment_size);
  void * target_fragment_data = generate_last_fragment_data(target, flags -> fragment_size);
  void * null_fragment_data = calloc(1, flags -> fragment_size);
  int sttf;
  for (fragment = 0; fragment < fragment_table -> target_fragments; fragment ++) {
    sttf = fragment_table -> source_to_target_fragments[fragment];
    if (flags -> reversible_patch && (sttf >= 0) && (fragment_table -> target_to_source_fragments[sttf] == fragment)) {
      reverse_fragment_labels[sttf] = forward_fragment_labels[fragment];
      p = 1;
    } else
      p = 0;
    forward_fragment_lengths[fragment] = calculate_fragment_length(
      p ? select_fragment_data(sttf, source, source_fragment_data, null_fragment_data, flags -> fragment_size) : NULL,
      select_fragment_data(fragment, target, target_fragment_data, NULL, flags -> fragment_size), flags);
    if (p) reverse_fragment_lengths[sttf] = forward_fragment_lengths[fragment];
  }
  result = write_xor_like_fragmented_header(codefile, target -> length, 0, forward_fragment_labels, forward_fragment_lengths, flags, fragment_table);
  if (result) goto done;
  if (flags -> reversible_patch) {
    for (fragment = 0; fragment < fragment_table -> source_fragments; fragment ++) {
      if (reverse_fragment_labels[fragment] >= 0) continue;
      if ((reverse_fragment_labels[fragment] = declare_numeric_local_for_codefile(codefile)) < 0) {
        result = duplicate_string("could not declare local labels for fragments");
        goto done;
      }
      reverse_fragment_data_needed[fragment] = 1;
      reverse_fragment_lengths[fragment] = calculate_fragment_length(
        NULL, select_fragment_data(fragment, source, source_fragment_data, NULL, flags -> fragment_size), flags);
    }
    if (add_local_label_to_codefile(codefile, "reverse") < 0) {
      result = duplicate_string("could not declare local label '.reverse'");
      goto done;
    }
    result = write_xor_like_fragmented_header(codefile, source -> length, 1, reverse_fragment_labels, reverse_fragment_lengths, flags, fragment_table);
    if (result) goto done;
  }
  for (fragment = 0; fragment < fragment_table -> target_fragments; fragment ++) {
    result = write_xor_like_fragment(codefile,
      select_fragment_data(fragment_table -> source_to_target_fragments[fragment], source, source_fragment_data, null_fragment_data, flags -> fragment_size),
      select_fragment_data(fragment, target, target_fragment_data, NULL, flags -> fragment_size),
      forward_fragment_lengths[fragment], forward_fragment_labels[fragment], data_writer);
    if (result) goto done;
  }
  if (flags -> reversible_patch) for (fragment = 0; fragment < fragment_table -> source_fragments; fragment ++) {
    if (!reverse_fragment_data_needed[fragment]) continue;
    result = write_xor_like_fragment(codefile,
      select_fragment_data(fragment_table -> target_to_source_fragments[fragment], target, target_fragment_data, null_fragment_data, flags -> fragment_size),
      select_fragment_data(fragment, source, source_fragment_data, NULL, flags -> fragment_size),
      reverse_fragment_lengths[fragment], reverse_fragment_labels[fragment], data_writer);
    if (result) goto done;
  }
  done:
  free(source_fragment_data);
  free(target_fragment_data);
  free(null_fragment_data);
  free(forward_fragment_lengths);
  free(reverse_fragment_lengths);
  free(forward_fragment_labels);
  free(reverse_fragment_labels);
  free(reverse_fragment_data_needed);
  return result;
}

char * write_xor_like_fragmented_header (CodeFile codefile, unsigned length, int reverse, const int * fragment_labels, const unsigned * fragment_lengths,
                                         const struct patching_flags * flags, const struct fragment_permutation_table * fragment_table) {
  const char * fragment_table_name;
  if (flags -> fragment_reordering)
    fragment_table_name = reverse ? "reverse_fragment_table" : "forward_fragment_table";
  else
    fragment_table_name = "fragment_table";
  char * result = write_patch_data_to_codefile(codefile, 1, &length, fragment_table_name);
  if (result) return result;
  if (flags -> fragment_reordering)
    if (result = write_fragment_permutation_table_to_codefile(codefile, fragment_table, reverse)) return result;
  add_blank_line_to_codefile(codefile);
  unsigned fragment_count = reverse ? fragment_table -> source_fragments : fragment_table -> target_fragments;
  if (!(flags -> fragment_reordering) && ((!reverse) == (fragment_table -> source_fragments > fragment_table -> target_fragments))) return NULL;
  if (add_local_label_to_codefile(codefile, fragment_table_name) < 0) return generate_string("could not declare local label '.%s'", fragment_table_name);
  unsigned fragment;
  for (fragment = 0; fragment < fragment_count; fragment ++) {
    if (fragment_lengths[fragment])
      result = add_instruction_to_codefile(codefile, INST_DW, ARGTYPE_IMMEDIATE, fragment_lengths[fragment], ARGTYPE_NUMERIC_LOCAL, fragment_labels[fragment],
                                           ARGTYPE_END);
    else
      result = add_instruction_to_codefile(codefile, INST_DW, ARGTYPE_IMMEDIATE, 0, ARGTYPE_END);
    if (result) return result;
  }
  add_blank_line_to_codefile(codefile);
  return NULL;
}

char * write_xor_like_fragment (CodeFile codefile, const unsigned char * source, const unsigned char * target, unsigned length, int label,
                                char * (* data_writer) (CodeFile, const unsigned char *, unsigned)) {
  if (add_declared_numeric_local_to_codefile(codefile, label) < 0) return duplicate_string("could not declare local label for data");
  void * data = generate_xor_data_buffer(source, target, length);
  char * result = data_writer(codefile, data, length);
  free(data);
  if (!result) add_blank_line_to_codefile(codefile);
  return result;
}

unsigned calculate_fragment_length (const unsigned char * source, const unsigned char * target, const struct patching_flags * flags) {
  unsigned length, result = 0;
  while (target) {
    length = calculate_unpadded_data_length(target, flags -> fragment_size, flags);
    if (length > result) result = length;
    target = source;
    source = NULL;
  }
  return result;
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
