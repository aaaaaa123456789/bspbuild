#include "proto.h"

void define_file_names_data (void) {
  unsigned current_file;
  const unsigned source_files = builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE];
  if (builder_state -> needed_functions.file_names) {
    add_declared_label_to_codefile(builder_state -> codefile, get_label(file_names, "FileNames"));
    for (current_file = 0; current_file < source_files; current_file ++)
      if (!add_string_to_codefile(builder_state -> codefile, builder_state -> options -> input_files[current_file].label))
        builder_throw("could not add file label \"%s\" to codefile", builder_state -> options -> input_files[current_file].label);
  }
  for (current_file = source_files; current_file < builder_state -> options -> input_file_count; current_file ++) {
    add_declared_label_to_codefile(builder_state -> codefile, builder_state -> file_name_labels[current_file - source_files]);
    if (!add_string_to_codefile(builder_state -> codefile, builder_state -> options -> input_files[current_file].label))
      builder_throw("could not add file label \"%s\" to codefile", builder_state -> options -> input_files[current_file].label);
  }
  add_blank_line_to_codefile(builder_state -> codefile);
}
