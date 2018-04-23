#include "proto.h"

void initialize_builder_state (Options options, CodeFile codefile) {
  builder_state = mr_calloc(builder_memory_region, sizeof(struct code_generation_state));
  builder_state -> options = options;
  builder_state -> codefile = codefile;
  memset(&(builder_state -> labels), -1, sizeof (builder_state -> labels)); // this should be enough to set them all to some negative value
  memset(&(builder_state -> constants), -1, sizeof (builder_state -> constants));
  memset(&(builder_state -> patch_engines), -1, sizeof (builder_state -> patch_engines));
  builder_state -> patch_engines.count = 0;
}

void initialize_code_generator (void) {
  declare_filename_labels();
  declare_patch_engine_labels();
  builder_state -> registers.file = declare_register("file", 0);
  builder_state -> registers.argument = declare_register("argument", 1);
  builder_state -> registers.temp = declare_register("temp", 2);
  builder_state -> registers.result = declare_register("result", 3);
  add_blank_line_to_codefile(builder_state -> codefile);
  *(builder_state -> constants.errors) = declare_constant("NO_ERRORS", 0);
  builder_state -> constants.errors[CODE_ERROR_INVALID_SOURCE] = declare_constant("ERROR_INVALID_SOURCE", CODE_ERROR_INVALID_SOURCE);
  builder_state -> constants.errors[CODE_ERROR_SOURCE_EQUALS_TARGET] = declare_constant("ERROR_SOURCE_EQUALS_TARGET", CODE_ERROR_SOURCE_EQUALS_TARGET);
  builder_state -> constants.errors[CODE_ERROR_INVALID_OUTPUT_SIZE] = declare_constant("ERROR_INVALID_OUTPUT_SIZE", CODE_ERROR_INVALID_OUTPUT_SIZE);
  builder_state -> constants.errors[CODE_ERROR_INVALID_OUTPUT_HASH] = declare_constant("ERROR_INVALID_OUTPUT_HASH", CODE_ERROR_INVALID_OUTPUT_HASH);
  builder_state -> constants.errors[CODE_ERROR_PATCH_NOT_REVERSIBLE] = declare_constant("ERROR_PATCH_NOT_REVERSIBLE", CODE_ERROR_PATCH_NOT_REVERSIBLE);
  builder_state -> constants.errors[CODE_ERROR_PADDING_NOT_MULTIPLE] = declare_constant("ERROR_PADDING_NOT_MULTIPLE", CODE_ERROR_PADDING_NOT_MULTIPLE);
  add_blank_line_to_codefile(builder_state -> codefile);
  builder_state -> constants.num_input_files = declare_constant("NUM_INPUT_FILES",
                                                                builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE] +
                                                                builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE_TARGET]);
  builder_state -> constants.first_output_file = declare_constant("FIRST_OUTPUT_FILE", builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE]);
  builder_state -> constants.hash_size = declare_constant("HASH_SIZE", 20);
  if (builder_state -> options -> padding_size)
    builder_state -> constants.padding_value = declare_constant("PADDING_VALUE", builder_state -> options -> padding_value);
  else
    builder_state -> constants.padding_value = -1;
  add_blank_line_to_codefile(builder_state -> codefile);
}

int declare_register (const char * register_name, unsigned char register_offset) {
  unsigned char num = builder_state -> options -> initial_register_number + register_offset;
  int result = add_register_definition_to_codefile(builder_state -> codefile, register_name, num);
  if (result < 0) builder_throw("could not declare register '%s'", register_name);
  return result;
}

int declare_constant (const char * constant_name, unsigned constant_value) {
  int result = add_constant_to_codefile(builder_state -> codefile, constant_name, constant_value);
  if (result < 0) builder_throw("could not declare constant '%s'", constant_name);
  return result;
}

void declare_filename_labels (void) {
  unsigned targets = builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE_TARGET] +
                     builder_state -> options -> file_count_per_direction[DIRECTION_TARGET];
  builder_state -> file_name_labels = mr_malloc(builder_memory_region, sizeof(int) * targets);
  unsigned p;
  char label_text[20];
  for (p = 0; p < targets; p ++) {
    sprintf(label_text, "Filename%u", p + builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE]);
    builder_state -> file_name_labels[p] = declare_label_for_codefile(builder_state -> codefile, label_text);
    if (builder_state -> file_name_labels[p] < 0)
      builder_throw("could not declare label '%s'", label_text);
  }
}

void declare_patch_engine_labels (void) {
  unsigned target = 1;
  int labelID;
  unsigned char patching_method;
  char label_text[15];
  if (builder_state -> options -> no_source_patches) target = builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE];
  for (; target < builder_state -> options -> input_file_count; target ++) {
    patching_method = builder_state -> options -> input_files[target].method;
    if (builder_state -> patch_engines.IDs[patching_method] >= 0) continue;
    builder_state -> patch_engines.IDs[patching_method] = builder_state -> patch_engines.count;
    builder_state -> patch_engines.methods[builder_state -> patch_engines.count] = patching_method;
    sprintf(label_text, "PatchEngine%hhu", patching_method);
    labelID = declare_label_for_codefile(builder_state -> codefile, label_text);
    if (labelID < 0) builder_throw("could not declare label '%s'", label_text);
    builder_state -> patch_engines.labels[builder_state -> patch_engines.count ++] = labelID;
  }
}
