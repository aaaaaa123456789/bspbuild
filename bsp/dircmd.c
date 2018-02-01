#include "proto.h"

void data_command (int width, char ** arguments) {
  struct bsp_argument * argument;
  char buffer[4];
  char * string;
  unsigned length;
  for (; *arguments; arguments ++) {
    if (**arguments == '"') {
      string = get_string_argument(*arguments);
      memset(buffer, 0, 4);
      length = strlen(string);
      append_data_to_script(string, length);
      mr_free(bsp_memory_region, string);
      if (length % width) append_data_to_script(buffer, width - (length % width));
      continue;
    }
    argument = get_argument(*arguments);
    switch (argument -> kind) {
      case 0:
        write_word_to_buffer(buffer, argument -> value);
        append_data_to_script(buffer, width);
        break;
      case 1:
        bsp_throw_error("variables are not allowed in data commands");
      case 2:
        if (width != 4) bsp_throw_error("references are not allowed in data commands narrower than 32 bits");
        memset(buffer, 0, 4);
        create_reference(argument -> reference, script_data -> length);
        append_data_to_script(buffer, 4);
    }
    mr_free(bsp_memory_region, argument);
  }
}

void hexdata_command (int _, char ** arguments) {
  char * buffer;
  unsigned argument_number, length, pos;
  for (argument_number = 0; arguments[argument_number]; argument_number ++) {
    if (strspn(arguments[argument_number], "0123456789abcdefABCDEF") != strlen(arguments[argument_number]))
      bsp_throw_error("argument %u to hexdata is not a valid hex string", argument_number + 1);
    length = (strlen(arguments[argument_number]) + 1) >> 1;
    buffer = mr_malloc(bsp_memory_region, length);
    for (pos = 0; pos < length; pos ++)
      buffer[pos] = (get_hex_digit(arguments[argument_number][pos << 1]) << 4) | get_hex_digit(arguments[argument_number][(pos << 1) + 1]);
    append_data_to_script(buffer, length);
    mr_free(bsp_memory_region, buffer);
  }
}

void define_command (int _, char ** arguments) {
  if (count_parameters(arguments) != 2) bsp_throw_error("command expects 2 argument(s), got %u", count_parameters(arguments));
  if (!validate_label(*arguments)) bsp_throw_error("'%s' is not a valid symbol name", *arguments);
  struct bsp_argument * argument = get_argument(arguments[1]);
  if (argument -> kind) bsp_throw_error("the second argument to define must be a number (got: %s)", arguments[1]);
  unsigned value = argument -> value;
  mr_free(bsp_memory_region, argument);
  struct bsp_symbol * definition = find_definition(*arguments);
  if (definition)
    definition -> value = value;
  else
    create_definition(*arguments, value);
}

void include_command (int is_binary, char ** arguments) {
  if (count_parameters(arguments) != 1) bsp_throw_error("command expects 1 argument(s), got %u", count_parameters(arguments));
  char * filename = get_string_argument(*arguments);
  if (is_binary)
    append_binary_file_to_script(filename);
  else
    bsp_push_file(filename);
  mr_free(bsp_memory_region, filename);
}

void string_command (int _, char ** arguments) {
  char * string;
  for (; *arguments; arguments ++) {
    string = get_string_argument(*arguments);
    append_data_to_script(string, strlen(string) + 1);
    mr_free(bsp_memory_region, string);
  }
}

void align_command (int _, char ** arguments) {
  if (count_parameters(arguments) != 1) bsp_throw_error("command expects 1 argument(s), got %u", count_parameters(arguments));
  struct bsp_argument * argument = get_argument(*arguments);
  if (argument -> kind || !(argument -> value)) bsp_throw_error("the argument to align must be a non-zero number (got: %s)", *arguments);
  unsigned alignment = argument -> value;
  mr_free(bsp_memory_region, argument);
  if (!(script_data -> length % alignment)) return;
  alignment -= script_data -> length % alignment;
  void * padding = mr_calloc(bsp_memory_region, alignment);
  append_data_to_script(padding, alignment);
  mr_free(bsp_memory_region, padding);
}
