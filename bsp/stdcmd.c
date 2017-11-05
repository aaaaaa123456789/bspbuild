#include "proto.h"

void no_arguments_command (int opcode_byte, char ** arguments) {
  standard_command(opcode_byte, 0, 0, arguments);
}

void one_argument_command (int opcode_byte, char ** arguments) {
  standard_command(opcode_byte, 0, 1, arguments);
}

void two_arguments_command (int opcode_byte, char ** arguments) {
  standard_command(opcode_byte, 0, 2, arguments);
}

void one_variable_command (int opcode_byte, char ** arguments) {
  standard_command(opcode_byte, 1, 0, arguments);
}

void one_variable_one_argument_command (int opcode_byte, char ** arguments) {
  standard_command(opcode_byte, 1, 1, arguments);
}

void one_variable_two_arguments_command (int opcode_byte, char ** arguments) {
  standard_command(opcode_byte, 1, 2, arguments);
}

void two_variables_command (int opcode_byte, char ** arguments) {
  standard_command(opcode_byte, 2, 0, arguments);
}

void two_variables_two_arguments_command (int opcode_byte, char ** arguments) {
  standard_command(opcode_byte, 2, 2, arguments);
}

void standard_command (unsigned char opcode_byte, unsigned char expected_variable_count, unsigned char expected_argument_count, char ** arguments) {
  unsigned temp = count_parameters(arguments);
  if (temp != (expected_variable_count + expected_argument_count))
    error_exit(1, "command expects %hhu argument(s), got %u", expected_variable_count + expected_argument_count, temp);
  unsigned char * buffer = mr_calloc(bsp_memory_region, 1 + expected_variable_count + 4 * expected_argument_count);
  unsigned char * current = buffer + 1;
  struct bsp_argument * argument;
  while (expected_variable_count --) {
    argument = get_argument(*(arguments ++));
    if (argument -> kind != 1) error_exit(1, "argument must be a variable");
    *(current ++) = argument -> value;
    mr_free(bsp_memory_region, argument);
  }
  while (expected_argument_count --) {
    argument = get_argument(*(arguments ++));
    switch (argument -> kind) {
      case 0:
        write_word_to_buffer(current, argument -> value);
        current += 4;
        break;
      case 1:
        *(current ++) = argument -> value;
        opcode_byte |= 1 << expected_argument_count;
        break;
      case 2:
        create_reference(argument -> reference, script_data -> length + (current - buffer));
        current += 4;
    }
    mr_free(bsp_memory_region, argument);
  }
  *buffer = opcode_byte;
  append_data_to_script(buffer, current - buffer);
  mr_free(bsp_memory_region, buffer);
}

void one_byte_argument_command (int opcode_byte, char ** arguments) {
  if (count_parameters(arguments) != 1) error_exit(1, "command expects 1 argument(s), got %u", count_parameters(arguments));
  struct bsp_argument * argument = get_argument(*arguments);
  if (argument -> kind == 2) error_exit(1, "cannot use a reference as a byte-sized argument");
  char buffer[2];
  *buffer = opcode_byte + argument -> kind;
  buffer[1] = argument -> value;
  mr_free(bsp_memory_region, argument);
  append_data_to_script(buffer, 2);
}

void one_halfword_argument_command (int opcode_byte, char ** arguments) {
  if (count_parameters(arguments) != 1) error_exit(1, "command expects 1 argument(s), got %u", count_parameters(arguments));
  struct bsp_argument * argument = get_argument(*arguments);
  if (argument -> kind == 2) error_exit(1, "cannot use a reference as a halfword-sized argument");
  char buffer[3];
  *buffer = opcode_byte + argument -> kind;
  if (argument -> kind) {
    buffer[1] = argument -> value;
    append_data_to_script(buffer, 2);
  } else {
    write_halfword_to_buffer(buffer + 1, argument -> value);
    append_data_to_script(buffer, 3);
  }
  mr_free(bsp_memory_region, argument);
}

void one_argument_one_byte_argument (int opcode_byte, char ** arguments) {
  if (count_parameters(arguments) != 2) error_exit(1, "command expects 2 argument(s), got %u", count_parameters(arguments));
  struct bsp_argument * argument = get_argument(*arguments);
  char buffer[6];
  char * current = buffer + 1;
  switch (argument -> kind) {
    case 0:
      write_word_to_buffer(current, argument -> value);
      current += 4;
      break;
    case 1:
      *(current ++) = argument -> value;
      opcode_byte += 2;
      break;
    case 2:
      create_reference(argument -> reference, script_data -> length + 1);
      memset(buffer, 0, 4);
      current += 4;
  }
  mr_free(bsp_memory_region, argument);
  argument = get_argument(arguments[1]);
  if (argument -> kind == 2) error_exit(1, "cannot use a reference as a byte-sized argument");
  opcode_byte += argument -> kind;
  *(current ++) = argument -> value;
  mr_free(bsp_memory_region, argument);
  *buffer = opcode_byte;
  append_data_to_script(buffer, current - buffer);
}

void one_argument_one_halfword_argument (int opcode_byte, char ** arguments) {
  if (count_parameters(arguments) != 2) error_exit(1, "command expects 2 argument(s), got %u", count_parameters(arguments));
  struct bsp_argument * argument = get_argument(*arguments);
  char buffer[7];
  char * current = buffer + 1;
  switch (argument -> kind) {
    case 0:
      write_word_to_buffer(current, argument -> value);
      current += 4;
      break;
    case 1:
      *(current ++) = argument -> value;
      opcode_byte += 2;
      break;
    case 2:
      create_reference(argument -> reference, script_data -> length + 1);
      memset(buffer, 0, 4);
      current += 4;
  }
  mr_free(bsp_memory_region, argument);
  argument = get_argument(arguments[1]);
  if (argument -> kind == 2) error_exit(1, "cannot use a reference as a halfword-sized argument");
  opcode_byte += argument -> kind;
  if (argument -> kind)
    *(current ++) = argument -> value;
  else {
    write_halfword_to_buffer(current, argument -> value);
    current += 2;
  }
  mr_free(bsp_memory_region, argument);
  *buffer = opcode_byte;
  append_data_to_script(buffer, current - buffer);
}
