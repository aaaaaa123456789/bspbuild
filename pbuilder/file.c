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
  char * comment;
  if (builder_state -> options -> input_files[file].label)
    comment = generate_string("%s: \"%s\" (%s)", tag, builder_state -> options -> input_files[file].label, builder_state -> options -> input_files[file].name);
  else
    comment = generate_string("%s: %s", tag, builder_state -> options -> input_files[file].name);
  add_comment_to_codefile(builder_state -> codefile, comment, 1);
  free(comment);
}

unsigned write_individual_file_patch (unsigned from_file, unsigned to_file) {
  char * error;
  char * message;
  int rv;
  Buffer source = read_file_into_buffer(builder_state -> options -> input_files[from_file].name, &error);
  if (!error) rv = get_file_length_and_hash(from_file, source);
  if (error || !rv) {
    char * message = error ? mr_duplicate_string(builder_memory_region, error) : "file was modified during patch generation";
    free(error);
    builder_throw("%s: %s", builder_state -> options -> input_files[from_file].name, message);
  }
  Buffer target = read_file_into_buffer(builder_state -> options -> input_files[to_file].name, &error);
  if (!error) rv = get_file_length_and_hash(to_file, target);
  if (error || !rv) {
    free(source);
    char * message = error ? mr_duplicate_string(builder_memory_region, error) : "file was modified during patch generation";
    free(error);
    builder_throw("%s: %s", builder_state -> options -> input_files[from_file].name, message);
  }
  unsigned result = add_numeric_data_label_to_codefile(builder_state -> codefile);
  write_file_patch_header_comment("from", from_file);
  write_file_patch_header_comment("to", to_file);
  struct patching_flags flags = {
    .fragment_size = builder_state -> options -> fragment_size,
    .padding_value = builder_state -> options -> padding_value,
    .patching_method = builder_state -> options -> input_files[to_file].method,
    .padding_length = builder_state -> options -> padding_size,
    .fragmentation_enabled = (builder_state -> options -> fragment_size >= MINIMUM_FRAGMENT_SIZE),
    .fragment_reordering = builder_state -> options -> detect_fragment_permutation,
    .reversible_patch = (builder_state -> options -> input_files[from_file].direction == DIRECTION_SOURCE_TARGET) && 
                        (builder_state -> options -> input_files[to_file].direction == DIRECTION_SOURCE_TARGET)
  };
  unsigned char patch_method_descriptor_byte = builder_state -> patch_engines.IDs[flags.patching_method];
  if (flags.reversible_patch) patch_method_descriptor_byte |= 0x80;
  message = generate_string("method: %s", get_patch_method_name(flags.patching_method));
  add_comment_to_codefile(builder_state -> codefile, message, 1);
  free(message);
  inst(INST_DB, imm(patch_method_descriptor_byte));
  add_blank_line_to_codefile(builder_state -> codefile);
  error = write_patch_to_codefile(builder_state -> codefile, source, target, &flags);
  free(source);
  free(target);
  if (error) {
    message = mr_duplicate_string(builder_memory_region, error);
    free(error);
    builder_throw("patching %s to %s: %s", builder_state -> options -> input_files[from_file].name, builder_state -> options -> input_files[to_file].name, message);
  }
  add_blank_line_to_codefile(builder_state -> codefile);
  return result;
}

int get_file_length_and_hash (unsigned file, Buffer contents) {
  const unsigned char null_hash[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  unsigned char * file_hash = calculate_sha1(contents -> data, contents -> length);
  if (builder_state -> file_data[file].size || memcmp(builder_state -> file_data[file].hash, null_hash, 20)) {
    int rv = memcmp(builder_state -> file_data[file].hash, file_hash, 20);
    free(file_hash);
    if (rv) return 0;
    return contents -> length == builder_state -> file_data[file].size;
  } else {
    memcpy(builder_state -> file_data[file].hash, file_hash, 20);
    free(file_hash);
    builder_state -> file_data[file].size = contents -> length;
    return 1;
  }
}
