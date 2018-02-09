#include "proto.h"

void define_main_function (void) {
  add_declared_label_to_codefile(builder_state -> codefile, get_label(main, "Main"));
  char ** opening_banner_lines;
  int * opening_banner_labels = NULL;
  int opening_banner_line_count = generate_banner_lines(builder_state -> options -> messages.opening, &opening_banner_lines);
  unsigned pos;
  if (opening_banner_line_count > 0) {
    opening_banner_labels = mr_malloc(builder_memory_region, sizeof(int) * opening_banner_line_count);
    for (pos = 0; pos < opening_banner_line_count; pos ++) {
      opening_banner_labels[pos] = declare_numeric_local_for_codefile(builder_state -> codefile);
      if (opening_banner_labels[pos] < 0) builder_throw("could not declare numeric local");
    }
    for (pos = 0; pos < opening_banner_line_count; pos ++) inst(INST_PRINT, ARGTYPE_NUMERIC_LOCAL, opening_banner_labels[pos]);
  }
  inst(INST_CALL, ARGTYPE_NAMED_LABEL, get_label(detect_input, "DetectInput"));
  if (!(builder_state -> options -> suppress_source_detection_message)) {
    builder_state -> needed_functions.print_detected_input = 1;
    inst(INST_CALL, ARGTYPE_NAMED_LABEL, get_label(print_detected_input, "PrintDetectedInput"));
  }
  inst(INST_CALL, ARGTYPE_NAMED_LABEL, get_label(select_output, "SelectOutput"));
  inst(INST_SET, ARGTYPE_NAMED_REGISTER, builder_state -> registers.argument,
                 ARGTYPE_NAMED_CONSTANT, builder_state -> constants.errors[CODE_ERROR_SOURCE_EQUALS_TARGET]);
  inst(INST_IFEQ, ARGTYPE_NAMED_REGISTER, builder_state -> registers.file, ARGTYPE_NAMED_REGISTER, builder_state -> registers.result,
                  ARGTYPE_NAMED_LABEL, get_label(error, "Error"));
  inst(INST_SET, ARGTYPE_NAMED_REGISTER, builder_state -> registers.argument, ARGTYPE_NAMED_REGISTER, builder_state -> registers.result);
  if (builder_state -> options -> disable_output_validations != VALIDATE_NONE)
    inst(INST_PUSH, ARGTYPE_NAMED_REGISTER, builder_state -> registers.argument);
  inst(INST_SEEK, ARGTYPE_IMMEDIATE, 0);
  inst(INST_CALL, ARGTYPE_NAMED_LABEL, get_label(generate_output, "GenerateOutput"));
  if (builder_state -> options -> disable_output_validations != VALIDATE_NONE) {
    inst(INST_POP, ARGTYPE_NAMED_REGISTER, builder_state -> registers.argument);
    builder_state -> needed_functions.validate_output = 1;
    inst(INST_CALL, ARGTYPE_NAMED_LABEL, get_label(validate_output, "ValidateOutput"));
  }
  char ** success_banner_lines;
  int * success_banner_labels = NULL;
  int success_banner_line_count = generate_banner_lines(builder_state -> options -> messages.success, &success_banner_lines);
  if (success_banner_line_count > 0) {
    success_banner_labels = mr_malloc(builder_memory_region, sizeof(int) * success_banner_line_count);
    for (pos = 0; pos < success_banner_line_count; pos ++) {
      success_banner_labels[pos] = declare_numeric_local_for_codefile(builder_state -> codefile);
      if (success_banner_labels[pos] < 0) builder_throw("could not declare numeric local");
    }
    for (pos = 0; pos < success_banner_line_count; pos ++) inst(INST_PRINT, ARGTYPE_NUMERIC_LOCAL, success_banner_labels[pos]);
  }
  inst(INST_EXIT, ARGTYPE_NAMED_CONSTANT, *(builder_state -> constants.errors));
  add_blank_line_to_codefile(builder_state -> codefile);
  int keep_going = 1;
  if (opening_banner_line_count > 0) {
    for (pos = 0; keep_going && (pos < opening_banner_line_count); pos ++) {
      add_declared_numeric_local_to_codefile(builder_state -> codefile, opening_banner_labels[pos]);
      keep_going = add_string_to_codefile(builder_state -> codefile, opening_banner_lines[pos]);
    }
    mr_free(builder_memory_region, opening_banner_labels);
    destroy_banner_lines(opening_banner_lines, opening_banner_line_count);
    add_blank_line_to_codefile(builder_state -> codefile);
  }
  if (success_banner_line_count > 0) {
    for (pos = 0; keep_going && (pos < success_banner_line_count); pos ++) {
      add_declared_numeric_local_to_codefile(builder_state -> codefile, success_banner_labels[pos]);
      keep_going = add_string_to_codefile(builder_state -> codefile, success_banner_lines[pos]);
    }
    mr_free(builder_memory_region, success_banner_labels);
    destroy_banner_lines(success_banner_lines, success_banner_line_count);
    add_blank_line_to_codefile(builder_state -> codefile);
  }
  if (!keep_going) builder_throw("could not add banner strings to output");
}
