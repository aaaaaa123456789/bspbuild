#include "proto.h"

void define_error_function (void) {
  add_declared_label_to_codefile(builder_state -> codefile, get_label(error, "Error"));
  char ** error_message_lines = NULL;
  int error_message_line_count = 0;
  if (!(builder_state -> options -> suppress_error_messages))
    error_message_line_count = generate_banner_lines(builder_state -> options -> messages.error, &error_message_lines);
  if (error_message_line_count <= 0) {
    inst(INST_EXIT, ARGTYPE_NAMED_REGISTER, builder_state -> registers.argument);
    add_blank_line_to_codefile(builder_state -> codefile);
    return;
  }
  unsigned current_line = error_message_line_count;
  if (builder_state -> options -> messages.error_replacement && *(builder_state -> options -> messages.error_replacement))
    for (current_line = 0; current_line < error_message_line_count; current_line ++)
      if (strstr(error_message_lines[current_line], builder_state -> options -> messages.error_replacement)) break;
  const char * error;
  if (current_line == error_message_line_count)
    error = simple_error_message_function(error_message_lines, error_message_line_count);
  else
    error = descriptive_error_message_function(error_message_lines, error_message_line_count);
  destroy_string_array(error_message_lines, error_message_line_count);
  if (error) builder_throw("%s", error);
  add_blank_line_to_codefile(builder_state -> codefile);
}

const char * simple_error_message_function (char ** message_lines, unsigned message_line_count) {
  int * message_labels = malloc(sizeof(int) * message_line_count);
  unsigned current;
  for (current = 0; current < message_line_count; current ++)
    if ((message_labels[current] = declare_numeric_local_for_codefile(builder_state -> codefile)) < 0) break;
  if (current < message_line_count) {
    free(message_labels);
    return "could not declare numeric local";
  }
  for (current = 0; current < message_line_count; current ++) inst(INST_PRINT, ARGTYPE_NUMERIC_LOCAL, message_labels[current]);
  inst(INST_EXIT, ARGTYPE_NAMED_REGISTER, builder_state -> registers.argument);
  add_blank_line_to_codefile(builder_state -> codefile);
  for (current = 0; current < message_line_count; current ++) {
    add_declared_numeric_local_to_codefile(builder_state -> codefile, message_labels[current]);
    if (!add_string_to_codefile(builder_state -> codefile, message_lines[current])) {
      free(message_labels);
      return "could not add error message strings to output";
    }
  }
  free(message_labels);
  return NULL;
}

const char * descriptive_error_message_function (char ** message_lines, unsigned message_line_count) {
  // ...
}
