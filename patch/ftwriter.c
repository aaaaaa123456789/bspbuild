#include "proto.h"

/*
   Fragment permutation table format:
   data stored in 32-bit words, top bit: command, 31 bits: fragment affected
   top bit 1: clear fragment (1 word, payload indicates fragment ID)
   top bit 0: copy/swap (2 words, first word payload indicates source, second word payload indicates target, second word command is 0 for copy, 1 for swap)
   -1 ends list
*/

char * write_fragment_permutation_table_to_codefile (CodeFile codefile, const struct fragment_permutation_table * table, int reverse) {
  unsigned fragment_count, original_fragment_count;
  const int * fragment_targets;
  if (reverse) {
    fragment_count = table -> source_fragments;
    original_fragment_count = table -> target_fragments;
    fragment_targets = table -> target_to_source_fragments;
  } else {
    fragment_count = table -> target_fragments;
    original_fragment_count = table -> source_fragments;
    fragment_targets = table -> source_to_target_fragments;
  }
  char * error = NULL;
  unsigned fragment;
  if (original_fragment_count < fragment_count) {
    for (fragment = original_fragment_count; fragment < fragment_count; fragment ++) {
      if (fragment_targets[fragment] < 0) continue;
      error = write_patch_data_to_codefile(codefile, 2, (unsigned []) {fragment_targets[fragment], fragment}, NULL);
      if (error) return error;
    }
    fragment_count = original_fragment_count;
  }
  unsigned sum = 0;
  unsigned char * fragments_OK = calloc(fragment_count, 1);
  for (fragment = 0; fragment < fragment_count; fragment ++) if (fragment_targets[fragment] == fragment) {
    fragments_OK[fragment] = 1;
    sum ++;
  }
  if (sum != fragment_count)
    error = write_fragment_copy_commands_to_codefile(codefile, fragment_targets, fragments_OK, &sum, fragment_count);
  if (!error && (sum != fragment_count))
    error = write_fragment_swap_commands_to_codefile(codefile, fragment_targets, fragments_OK, fragment_count);
  free(fragments_OK);
  return error;
}

char * write_fragment_copy_commands_to_codefile (CodeFile codefile, const int * targets, unsigned char * fragments_OK, unsigned * sum, unsigned fragment_count) {
  // ...
}

char * write_fragment_swap_commands_to_codefile (CodeFile codefile, const int * targets, unsigned char * fragments_OK, unsigned fragment_count) {
  // ...
}
