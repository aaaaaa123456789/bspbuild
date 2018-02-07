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
  if (current_line == error_message_line_count)
    simple_error_message_function(error_message_lines, error_message_line_count);
  else
    descriptive_error_message_function(error_message_lines, error_message_line_count);
  add_blank_line_to_codefile(builder_state -> codefile);
  destroy_string_array(error_message_lines, error_message_line_count);
}

void simple_error_message_function (char ** message_lines, unsigned message_line_count) {
  // ...
}

void descriptive_error_message_function (char ** message_lines, unsigned message_line_count) {
  // ...
}
