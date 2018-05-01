#include "proto.h"

void write_file_patches (void) {
  int patch_source, source_patches_source;
  unsigned patch_target, first_target_file;
  first_target_file = builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE] + 
                      builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE_TARGET];
  if (builder_state -> options -> no_source_patches && (builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE] > 1)) {
    patch_source = -1;
    patch_target = builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE];
    if (builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE_TARGET])
      source_patches_source = first_target_file - 1;
    else
      source_patches_source = -1;
  } else {
    patch_source = 0;
    patch_target = 1;
    source_patches_source = first_target_file - 1;
  }
  write_file_patch(patch_source, patch_target ++);
  while (patch_target < first_target_file) {
    write_file_patch(patch_target - 1, patch_target);
    patch_target ++;
  }
  while (patch_target < builder_state -> options -> input_file_count) {
    patch_source = builder_state -> options -> input_files[patch_target].reference;
    switch (patch_source) {
      case TARGET_TYPE_PREVIOUS:
        patch_source = patch_target - 1;
        break;
      case TARGET_TYPE_FROM_SOURCE:
        patch_source = source_patches_source;
    }
    write_file_patch(patch_source, patch_target ++);
  }
}
