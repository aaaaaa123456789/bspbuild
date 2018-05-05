#include "proto.h"

void define_error_function (void) {
  add_declared_label_to_codefile(builder_state -> codefile, get_label(error, "Error"));
  char ** error_message_lines = NULL;
  int error_message_line_count = 0;
  if (!(builder_state -> options -> suppress_error_messages))
    error_message_line_count = generate_banner_lines(builder_state -> options -> messages.error, &error_message_lines);
  if (error_message_line_count <= 0) {
    inst(INST_EXIT, reg(argument));
    add_blank_line_to_codefile(builder_state -> codefile);
    return;
  }
  unsigned current_line = error_message_line_count;
  if (builder_state -> options -> messages.error_replacement && *(builder_state -> options -> messages.error_replacement))
    for (current_line = 0; current_line < error_message_line_count; current_line ++)
      if (strstr(error_message_lines[current_line], builder_state -> options -> messages.error_replacement)) break;
  if (current_line == error_message_line_count)
    simple_error_message_function(error_message_lines, error_message_line_count);
  else
    descriptive_error_message_function(error_message_lines, error_message_line_count);
  destroy_banner_lines(error_message_lines, error_message_line_count);
  add_blank_line_to_codefile(builder_state -> codefile);
}

void simple_error_message_function (char ** message_lines, unsigned message_line_count) {
  int * message_labels = mr_malloc(builder_memory_region, sizeof(int) * message_line_count);
  unsigned current;
  for (current = 0; current < message_line_count; current ++)
    if ((message_labels[current] = declare_numeric_local_for_codefile(builder_state -> codefile)) < 0) break;
  if (current < message_line_count) builder_throw("could not declare numeric local");
  for (current = 0; current < message_line_count; current ++)
    inst(INST_PRINT, nloc(message_labels[current]));
  inst(INST_EXIT, reg(argument));
  add_blank_line_to_codefile(builder_state -> codefile);
  for (current = 0; current < message_line_count; current ++) {
    add_declared_numeric_local_to_codefile(builder_state -> codefile, message_labels[current]);
    if (!add_string_to_codefile(builder_state -> codefile, message_lines[current])) builder_throw("could not add error message strings to output");
  }
  mr_free(builder_memory_region, message_labels);
}

void descriptive_error_message_function (char ** message_lines, unsigned message_line_count) {
  inst(INST_PUSH, reg(argument));
  inst(INST_DECREMENT, reg(argument));
  builder_state -> needed_functions.get_nth_string = 1;
  inst(INST_SET, reg(result), loc("error_strings"));
  inst(INST_CALL, lbl(get_nth_string, "GetNthString"));
  unsigned pos, label_count = 0;
  char ** strings = NULL;
  int * labels = NULL;
  char * match_pos;
  char * remaining;
  const unsigned replacement_length = strlen(builder_state -> options -> messages.error_replacement);
  int label;
  inst(INST_CLEARBUF); // just in case
  for (pos = 0; pos < message_line_count; pos ++)
    if (strstr(message_lines[pos], builder_state -> options -> messages.error_replacement)) {
      remaining = message_lines[pos];
      while (match_pos = strstr(remaining, builder_state -> options -> messages.error_replacement)) {
        if (match_pos != remaining) {
          *match_pos = 0;
          if ((label = add_string_to_printed_error_messages(&labels, &strings, &label_count, remaining)) < 0)
            builder_throw("could not declare numeric local");
          inst(INST_BUFSTRING, nloc(label));
        }
        inst(INST_BUFSTRING, reg(result));
        remaining = match_pos + replacement_length;
      }
      if (*remaining) {
        if ((label = add_string_to_printed_error_messages(&labels, &strings, &label_count, remaining)) < 0)
          builder_throw("could not declare numeric local");
        inst(INST_BUFSTRING, nloc(label));
      }
      inst(INST_PRINTBUF);
    } else {
      if ((label = add_string_to_printed_error_messages(&labels, &strings, &label_count, message_lines[pos])) < 0)
        builder_throw("could not declare numeric local");
      inst(INST_PRINT, nloc(label));
    }
  inst(INST_POP, reg(argument));
  inst(INST_EXIT, reg(argument));
  add_blank_line_to_codefile(builder_state -> codefile);
  builder_declare_local("error_strings");
  for (pos = 0; pos < NUM_CODE_ERRORS; pos ++)
    if (!add_string_to_codefile(builder_state -> codefile, code_error_messages[pos])) builder_throw("could not add error messages to output");
  add_blank_line_to_codefile(builder_state -> codefile);
  for (pos = 0; pos < label_count; pos ++) {
    add_declared_numeric_local_to_codefile(builder_state -> codefile, labels[pos]);
    if (!add_string_to_codefile(builder_state -> codefile, strings[pos])) builder_throw("could not add error message strings to output");
  }
  mr_free(builder_memory_region, strings);
  mr_free(builder_memory_region, labels);
}

int add_string_to_printed_error_messages (int ** labels, char *** strings, unsigned * label_count, char * string) {
  int result = declare_numeric_local_for_codefile(builder_state -> codefile);
  if (result < 0) return result;
  *labels = mr_realloc(builder_memory_region, *labels, sizeof(int) * (*label_count + 1));
  *strings = mr_realloc(builder_memory_region, *strings, sizeof(char *) * (*label_count + 1));
  (*labels)[*label_count] = result;
  (*strings)[(*label_count) ++] = string;
  return result;
}
