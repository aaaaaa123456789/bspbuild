#include "proto.h"

void generate_patch_data (void) {
  define_file_names_data();
  write_file_patches();
  check_for_unopened_files();
  define_patch_list();
  define_file_sizes();
  define_file_hashes();
}

static inline const char * get_label_text_for_file (unsigned file) {
  const char * result = builder_state -> options -> input_files[file].label;
  if (result) return result;
  return builder_state -> options -> input_files[file].name;
}

void define_file_names_data (void) {
  unsigned current_file;
  const unsigned source_files = builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE];
  if (builder_state -> needed_functions.file_names) {
    add_declared_label_to_codefile(builder_state -> codefile, get_label(file_names, "FileNames"));
    for (current_file = 0; current_file < source_files; current_file ++)
      if (!add_string_to_codefile(builder_state -> codefile, get_label_text_for_file(current_file)))
        builder_throw("could not add file label \"%s\" to codefile", get_label_text_for_file(current_file));
  }
  for (current_file = source_files; current_file < builder_state -> options -> input_file_count; current_file ++) {
    add_declared_label_to_codefile(builder_state -> codefile, builder_state -> file_name_labels[current_file - source_files]);
    if (!add_string_to_codefile(builder_state -> codefile, get_label_text_for_file(current_file)))
      builder_throw("could not add file label \"%s\" to codefile", get_label_text_for_file(current_file));
  }
  add_blank_line_to_codefile(builder_state -> codefile);
}

void check_for_unopened_files (void) {
  const unsigned char null_hash[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  unsigned file;
  for (file = 0; file < builder_state -> options -> input_file_count; file ++)
    if (!(builder_state -> file_data[file].size || memcmp(builder_state -> file_data[file].hash, null_hash, 20)))
      builder_throw("no patch data generated for file %s", builder_state -> options -> input_files[file].name);
}

void define_patch_list (void) {
  add_declared_label_to_codefile(builder_state -> codefile, get_label(patch_list, "PatchList"));
  unsigned file;
  if (builder_state -> needed_functions.full_patch_list)
    file = 1;
  else
    file = builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE];
  if (!file) file = 1;
  for (; file < builder_state -> options -> input_file_count; file ++) {
    if (!(builder_state -> file_data[file].data_label))
      builder_throw("no patch data generated for file %s", builder_state -> options -> input_files[file].name);
    inst(INST_DW, imm(builder_state -> file_data[file].reference_file), dat(builder_state -> file_data[file].data_label));
  }
  add_blank_line_to_codefile(builder_state -> codefile);
}

void define_file_sizes (void) {
  add_declared_label_to_codefile(builder_state -> codefile, get_label(file_sizes, "FileSizes"));
  unsigned file, limit;
  if (builder_state -> needed_functions.full_file_sizes)
    limit = builder_state -> options -> input_file_count;
  else
    limit = builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE] + builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE_TARGET];
  for (file = 0; file < limit; file ++)
    inst(INST_DW, imm(builder_state -> file_data[file].size));
  add_blank_line_to_codefile(builder_state -> codefile);
}

void define_file_hashes (void) {
  add_declared_label_to_codefile(builder_state -> codefile, get_label(file_hashes, "FileHashes"));
  unsigned file, limit;
  if (builder_state -> needed_functions.full_file_hashes)
    limit = builder_state -> options -> input_file_count;
  else
    limit = builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE] + builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE_TARGET];
  for (file = 0; file < limit; file ++)
    add_data_to_codefile(builder_state -> codefile, builder_state -> file_data[file].hash, 20);
  add_blank_line_to_codefile(builder_state -> codefile);
}
