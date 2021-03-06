#include "proto.h"

void define_detect_input_function (void) {
  add_declared_label_to_codefile(builder_state -> codefile, get_label(detect_input, "DetectInput"));
  inst(INST_XOR2, reg(file), reg(file));
  inst(INST_LENGTH, reg(result));
  inst(INST_SET, reg(argument), lbl(file_sizes, "FileSizes"));
  builder_declare_local("loop");
  inst(INST_GETWORDINC, reg(temp), reg(argument));
  inst(INST_IFNE, reg(temp), reg(result), loc("next_file"));
  inst(INST_MULTIPLY, reg(temp), reg(file), cnst(hash_size));
  inst(INST_ADD2, reg(temp), lbl(file_hashes, "FileHashes"));
  inst(INST_CHECKSHA1, reg(temp), reg(temp));
  inst(INST_RETZ, reg(temp));
  builder_declare_local("next_file");
  inst(INST_INCREMENT, reg(file));
  inst(INST_IFLT, reg(file), cnst(num_input_files), loc("loop"));
  inst(INST_SET, reg(argument), err(CODE_ERROR_INVALID_SOURCE));
  inst(INST_JUMP, lbl(error, "Error"));
  add_blank_line_to_codefile(builder_state -> codefile);
}

void define_print_detected_input_function (void) {
  char ** banner_lines;
  int banner_line_count = generate_banner_lines(builder_state -> options -> messages.source_detection, &banner_lines);
  int * banner_line_labels;
  unsigned pos;
  add_declared_label_to_codefile(builder_state -> codefile, get_label(print_detected_input, "PrintDetectedInput"));
  if (banner_line_count > 0) {
    banner_line_labels = mr_malloc(builder_memory_region, sizeof(int) * banner_line_count);
    for (pos = 0; pos < banner_line_count; pos ++) {
      if ((banner_line_labels[pos] = declare_numeric_local_for_codefile(builder_state -> codefile)) < 0) builder_throw("could not declare numeric local");
      if (pos != (banner_line_count - 1))
        inst(INST_PRINT, nloc(banner_line_labels[pos]));
      else
        inst(INST_BUFSTRING, nloc(banner_line_labels[pos]));
    }
  }
  builder_state -> needed_functions.file_names = 1;
  builder_state -> needed_functions.get_nth_string = 1;
  inst(INST_SET, reg(argument), reg(file));
  inst(INST_SET, reg(result), lbl(file_names, "FileNames"));
  inst(INST_CALL, lbl(get_nth_string, "GetNthString"));
  inst(INST_BUFSTRING, reg(result));
  inst(INST_PRINTBUF);
  inst(INST_RETURN);
  add_blank_line_to_codefile(builder_state -> codefile);
  if (banner_line_count <= 0) return;
  for (pos = 0; pos < banner_line_count; pos ++) {
    add_declared_numeric_local_to_codefile(builder_state -> codefile, banner_line_labels[pos]);
    if (!add_string_to_codefile(builder_state -> codefile, banner_lines[pos])) builder_throw("could not add input detection lines to codefile");
  }
  mr_free(builder_memory_region, banner_line_labels);
  destroy_banner_lines(banner_lines, banner_line_count);
  add_blank_line_to_codefile(builder_state -> codefile);
}

void define_validate_output_function (void) {
  add_declared_label_to_codefile(builder_state -> codefile, get_label(validate_output, "ValidateOutput"));
  inst(INST_SHIFTLEFT, reg(temp), reg(argument), imm(2));
  builder_state -> needed_functions.full_file_sizes = 1;
  inst(INST_ADD2, reg(temp), lbl(file_sizes, "FileSizes"));
  inst(INST_GETWORD, reg(temp), reg(temp));
  inst(INST_LENGTH, reg(result));
  if (builder_state -> options -> disable_output_validations == VALIDATE_ALL)
    inst(INST_PUSH, reg(argument));
  inst(INST_SET, reg(argument), err(CODE_ERROR_INVALID_OUTPUT_SIZE));
  inst(INST_IFNE, reg(temp), reg(result), lbl(error, "Error"));
  if (builder_state -> options -> disable_output_validations == VALIDATE_ALL) {
    inst(INST_POP, reg(temp));
    inst(INST_MULTIPLY2, reg(temp), cnst(hash_size));
    builder_state -> needed_functions.full_file_hashes = 1;
    inst(INST_ADD2, reg(temp), lbl(file_hashes, "FileHashes"));
    inst(INST_CHECKSHA1, reg(temp), reg(temp));
    inst(INST_SET, reg(argument), err(CODE_ERROR_INVALID_OUTPUT_HASH));
    inst(INST_JUMPNZ, reg(temp), lbl(error, "Error"));
  }
  inst(INST_RETURN);
  add_blank_line_to_codefile(builder_state -> codefile);
}

void define_get_nth_string_function (void) {
  add_declared_label_to_codefile(builder_state -> codefile, get_label(get_nth_string, "GetNthString"));
  inst(INST_RETZ, reg(argument));
  builder_declare_local("loop");
  inst(INST_GETBYTEINC, reg(temp), reg(result));
  inst(INST_JUMPNZ, reg(temp), loc("loop"));
  inst(INST_DECREMENT, reg(argument));
  inst(INST_JUMPNZ, reg(argument), loc("loop"));
  inst(INST_RETURN);
  add_blank_line_to_codefile(builder_state -> codefile);
}
