#include "proto.h"

void calculation_command (int opcode_byte, char ** arguments) {
  if (count_parameters(arguments) != 2) {
    standard_command(opcode_byte, 1, 2, arguments);
    return;
  }
  struct bsp_argument * argument = get_argument(*arguments);
  if (argument -> kind != 1) bsp_throw_error("argument must be a variable");
  char buffer[7];
  *buffer = opcode_byte + 2;
  buffer[1] = buffer[2] = argument -> value;
  mr_free(bsp_memory_region, argument);
  argument = get_argument(arguments[1]);
  switch (argument -> kind) {
    case 0:
      write_word_to_buffer(buffer + 3, argument -> value);
      append_data_to_script(buffer, 7);
      break;
    case 1:
      buffer[3] = argument -> value;
      (*buffer) ++;
      append_data_to_script(buffer, 4);
      break;
    case 2:
      memset(buffer + 3, 0, 4);
      create_reference(argument -> reference, script_data -> length + 3);
      append_data_to_script(buffer, 7);
  }
  mr_free(bsp_memory_region, argument);
}

void mulacum_command (int opcode_byte, char ** arguments) {
  if (count_parameters(arguments) != 3) bsp_throw_error("command expects 3 argument(s), got %u", count_parameters(arguments));
  struct bsp_argument * argument = get_argument(*(arguments ++));
  char buffer[11];
  char * current = buffer + 3;
  unsigned char arg_number = 2;
  *buffer = opcode_byte;
  if (argument -> kind != 1) bsp_throw_error("argument must be a variable");
  buffer[1] = buffer[2] = argument -> value;
  mr_free(bsp_memory_region, argument);
  while (arg_number --) {
    argument = get_argument(*(arguments ++));
    switch (argument -> kind) {
      case 0:
        write_word_to_buffer(current, argument -> value);
        current += 4;
        break;
      case 1:
        *(current ++) = argument -> value;
        *buffer |= 1 << arg_number;
        break;
      case 2:
        create_reference(argument -> reference, script_data -> length + (current - buffer));
        memset(current, 0, 4);
        current += 4;
    }
    mr_free(bsp_memory_region, argument);
  }
  append_data_to_script(buffer, current - buffer);
}

void bit_shift_command (int bit_shift_type, char ** arguments) {
  unsigned char shorthand;
  switch (count_parameters(arguments)) {
    case 2:
      shorthand = 1;
      break;
    case 3:
      shorthand = 0;
      break;
    default:
      bsp_throw_error("command expects 3 argument(s), got %u", count_parameters(arguments));
  }
  struct bsp_argument * argument = get_argument(*arguments);
  if (argument -> kind != 1) bsp_throw_error("argument must be a variable");
  unsigned char variable = argument -> value;
  mr_free(bsp_memory_region, argument);
  unsigned char shift_type, shift_count;
  argument = get_argument(arguments[shorthand ? 1 : 2]);
  if (argument -> kind == 2) bsp_throw_error("cannot use a reference as a shift count");
  if ((!argument -> kind) && (argument -> value > 31) && (argument -> value < -31u)) bsp_throw_error("shift count must be between -31 and 31");
  shift_type = argument -> kind;
  shift_count = argument -> value;
  mr_free(bsp_memory_region, argument);
  if (!shift_type) shift_count &= 31;
  argument = shorthand ? NULL : get_argument(arguments[1]);
  unsigned char buffer[8];
  unsigned char buffer_length;
  if (!(shift_type || shift_count)) {
    *buffer = SET_OPCODE;
    if (shorthand || (argument -> kind == 1)) (*buffer) ++;
    buffer_length = 1;
  } else {
    *buffer = SHIFT_OPCODE;
    bit_shift_type &= 3;
    buffer[1] = (shift_type ? 0 : shift_count) | (bit_shift_type << 5);
    if (shorthand || (argument -> kind == 1)) buffer[1] |= 0x80;
    buffer_length = 2;
  }
  buffer[buffer_length ++] = variable;
  if (shorthand)
    buffer[buffer_length ++] = variable;
  else
    switch (argument -> kind) {
      case 0:
        write_word_to_buffer(buffer + buffer_length, argument -> value);
        buffer_length += 4;
        break;
      case 1:
        buffer[buffer_length ++] = argument -> value;
        break;
      case 2:
        create_reference(argument -> reference, script_data -> length + buffer_length);
        memset(buffer + buffer_length, 0, 4);
        buffer_length += 4;
    }
  mr_free(bsp_memory_region, argument);
  if (shift_type) buffer[buffer_length ++] = shift_count;
  append_data_to_script(buffer, buffer_length);
}
