#include "proto.h"

void define_select_output_function (void) {
  add_declared_label_to_codefile(builder_state -> codefile, get_label(select_output, "SelectOutput"));
  int done = 0;
  unsigned target_count = builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE_TARGET] +
                          builder_state -> options -> file_count_per_direction[DIRECTION_TARGET];
  if (!(builder_state -> options -> output_selection_on_single_option)) done = check_for_single_output(target_count);
  if (!done) {
    unsigned * targets = NULL;
    unsigned p;
    if (builder_state -> options -> sort_targets_alphabetically) {
      targets = mr_malloc(builder_memory_region, sizeof(unsigned) * target_count);
      for (p = 0; p < target_count; p ++) targets[p] = p;
      qsort(targets, target_count, sizeof(unsigned), &compare_target_names);
    }
    if (builder_state -> options -> targets_per_page && (target_count > (builder_state -> options -> targets_per_page + 1)))
      show_paged_output_selection(targets, target_count);
    else
      show_output_selection(targets, target_count);
    if (targets) mr_free(builder_memory_region, targets);
  }
  add_blank_line_to_codefile(builder_state -> codefile);
}

int check_for_single_output (unsigned target_count) {
  if (target_count > 2) return 0; // nothing to do here
  unsigned target1, target2;
  target1 = builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE];
  if (target_count == 1) {
    inst(INST_SET, reg(result), imm(target1));
    inst(INST_RETURN);
    return 1;
  }
  target2 = target1 + 1;
  char * comment = generate_string("valid targets: %u, %u", target1, target2);
  add_comment_to_codefile(builder_state -> codefile, comment, 1);
  free(comment);
  inst(INST_SUBTRACT, reg(temp), reg(file), imm(target1));
  inst(INST_IFGT, reg(temp), imm(1), loc("multiple_available"));
  inst(INST_SUBTRACT, reg(result), imm(target1 + target2), reg(file));
  inst(INST_RETURN);
  if (add_local_label_to_codefile(builder_state -> codefile, "multiple_available") < 0) builder_throw("could not declare local label '.multiple_available'");
  return 0;
}

void show_output_selection (const unsigned * targets, unsigned target_count) {
  unsigned char selected_instruction_width;
  if (builder_state -> options -> input_file_count < 0x100)
    selected_instruction_width = 0;
  else if (builder_state -> options -> input_file_count < 0x10000)
    selected_instruction_width = 1;
  else
    selected_instruction_width = 2;
  inst(INST_MENU, reg(result), loc("menu"));
  if (targets) {
    if (selected_instruction_width)
      inst(INST_SHIFTLEFT2, reg(result), imm(selected_instruction_width));
    inst(INST_ADD2, reg(result), loc("file_IDs"));
    inst(selected_instruction_width[(int []) {INST_GETBYTE, INST_GETHALFWORD, INST_GETWORD}], reg(result), reg(result));
  } else if (builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE])
    inst(INST_ADD2, reg(result), cnst(first_output_file));
  inst(INST_RETURN);
  add_blank_line_to_codefile(builder_state -> codefile);
  if (add_local_label_to_codefile(builder_state -> codefile, "menu") < 0) builder_throw("could not declare local label '.menu'");
  if (targets) {
    generate_filename_menu(targets, target_count);
    inst(INST_DW, imm(-1));
    add_blank_line_to_codefile(builder_state -> codefile);
    if (add_local_label_to_codefile(builder_state -> codefile, "file_IDs") < 0) builder_throw("could not declare local label '.file_IDs'");
    generate_file_numbers_for_menu(targets, target_count, selected_instruction_width[(int []) {INST_DB, INST_DH, INST_DW}]);
  } else {
    unsigned * target_numbers = mr_malloc(builder_memory_region, sizeof(unsigned) * target_count);
    unsigned target;
    for (target = 0; target < target_count; target ++) target_numbers[target] = target;
    generate_filename_menu(target_numbers, target_count);
    mr_free(builder_memory_region, target_numbers);
    inst(INST_DW, imm(-1)); 
  }
}

void show_paged_output_selection (const unsigned * targets, unsigned target_count) {
  // ...
}

int compare_target_names (const void * first, const void * second) {
  unsigned first_index = *(const unsigned *) first + builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE];
  unsigned second_index = *(const unsigned *) second + builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE];
  return strcmp(builder_state -> options -> input_files[first_index].label, builder_state -> options -> input_files[second_index].label);
}

void generate_filename_menu (const unsigned * targets, unsigned target_count) {
  unsigned target;
  for (target = 0; target < target_count; target ++) inst(INST_DW, flbl(targets[target]));
}

void generate_file_numbers_for_menu (const unsigned * targets, unsigned target_count, int data_instruction) {
  unsigned target;
  for (target = 0; target < target_count; target ++)
    inst(data_instruction, imm(builder_state -> options -> file_count_per_direction[DIRECTION_SOURCE] + targets[target]));
}
