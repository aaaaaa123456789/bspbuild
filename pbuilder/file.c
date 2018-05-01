#include "proto.h"

void write_file_patch (int from_file, unsigned to_file) {
  if (from_file < 0)
    builder_state -> file_data[to_file].data_label = write_rainbow_file_patch(to_file);
  else
    builder_state -> file_data[to_file].data_label = write_individual_file_patch(from_file, to_file);
  builder_state -> file_data[to_file].reference_file = from_file;
}

unsigned write_rainbow_file_patch (unsigned file) {
  unsigned * data_labels = mr_malloc(builder_memory_region, sizeof(unsigned) * builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE]);
  unsigned current_file;
  for (current_file = 0; current_file < builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE]; current_file ++)
    data_labels[current_file] = write_individual_file_patch(current_file, file);
  unsigned result = add_numeric_data_label_to_codefile(builder_state -> codefile);
  write_file_patch_header_comment("patches for", file);
  for (current_file = 0; current_file < builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE]; current_file ++)
    inst(INST_DW, dat(data_labels[current_file]));
  mr_free(builder_memory_region, data_labels);
  add_blank_line_to_codefile(builder_state -> codefile);
  return result;
}

void write_file_patch_header_comment (const char * tag, unsigned file) {
  // ...
}

unsigned write_individual_file_patch (unsigned from_file, unsigned to_file) {
  // ...
}
