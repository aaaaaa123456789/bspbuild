#include "proto.h"

void define_select_output_function (void) {
  add_declared_label_to_codefile(builder_state -> codefile, get_label(select_output, "SelectOutput"));
  int done = 0;
  unsigned target_count = builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE_TARGET] +
                          builder_state -> options -> file_count_per_direction[DIRECTION_TARGET];
  if (!(builder_state -> options -> output_selection_on_single_option)) done = check_for_single_output(target_count);
  if (!done) {
    unsigned * targets = mr_malloc(builder_memory_region, sizeof(unsigned) * target_count);
    unsigned p;
    int sorted = 0;
    for (p = 0; p < target_count; p ++) targets[p] = p;
    if (builder_state -> options -> sort_targets_alphabetically) {
      sort_target_files_by_name(targets, target_count);
      sorted = 1;
    }
    if (builder_state -> options -> targets_per_page && (target_count > (builder_state -> options -> targets_per_page + 1)))
      show_paged_output_selection(targets, target_count);
    else
      show_output_selection(targets, target_count, sorted);
    mr_free(builder_memory_region, targets);
  }
  add_blank_line_to_codefile(builder_state -> codefile);
}

int check_for_single_output (unsigned target_count) {
  // ...
}

void sort_target_files_by_name (unsigned * targets, unsigned count) {
  // ...
}

void show_output_selection (const unsigned * targets, unsigned target_count, int alpha_sorted) {
  // ...
}

void show_paged_output_selection (const unsigned * targets, unsigned target_count) {
  // ...
}
