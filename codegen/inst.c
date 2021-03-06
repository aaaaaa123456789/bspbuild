#include "proto.h"

char * add_instruction_to_codefile (CodeFile file, int instruction, ...) {
  struct instruction_argument * arguments = NULL;
  struct instruction_argument next_argument;
  unsigned argument_count = 0;
  char * result;
  va_list ap;
  va_start(ap, instruction);
  while (next_argument.type = va_arg(ap, int)) {
    switch (next_argument.type) {
      case ARGTYPE_IMMEDIATE: case ARGTYPE_NAMED_CONSTANT: case ARGTYPE_REGISTER: case ARGTYPE_NAMED_REGISTER: case ARGTYPE_NAMED_LABEL:
      case ARGTYPE_NUMERIC_LOCAL: case ARGTYPE_NUMERIC_DATA:
        next_argument.value = va_arg(ap, unsigned);
        break;
      case ARGTYPE_LOCAL_LABEL: case ARGTYPE_GLOBAL_LABEL: case ARGTYPE_PASSTHROUGH:
        next_argument.string = va_arg(ap, const char *);
        break;
      default:
        result = generate_string("invalid argument type: %hhu", next_argument.type);
        goto done;
    }
    arguments = realloc(arguments, sizeof(struct instruction_argument) * (argument_count + 1));
    arguments[argument_count ++] = next_argument;
  }
  result = add_instruction_with_arguments_to_codefile(file, instruction, argument_count, arguments);
  done:
  va_end(ap); // should be done only once and at the same level as va_start for compatibility (some ancient implementations involve matching braces)
  free(arguments);
  return result;
}

char * add_instruction_with_arguments_to_codefile (CodeFile file, int instruction, unsigned argument_count, const struct instruction_argument * arguments) {
  unsigned p;
  const char * instruction_name;
  if (instruction >= 0) {
    if (instruction >= codegen_instruction_count) return generate_string("unknown instruction: %d", instruction);
    const struct code_generation_instruction_entry * instdata = codegen_instruction_table + instruction;
    if (argument_count != (instdata -> variable_operands + instdata -> any_operands))
      return generate_string("wrong number of operands for %s(%d): expected %d, got %u", instdata -> name, instruction,
                             instdata -> variable_operands + instdata -> any_operands, argument_count);
    for (p = 0; p < argument_count; p ++) if (arguments[p].type >= ARGTYPE_INVALID) return generate_string("invalid argument type: %hhu", arguments[p].type);
    for (p = 0; p < instdata -> variable_operands; p ++) if (!value_in_list(arguments[p].type, ARGTYPE_REGISTER, ARGTYPE_NAMED_REGISTER, ARGTYPE_PASSTHROUGH, -1))
      return generate_string("first %u operands for %s(%d) must be variables, but operand %u isn't", instdata -> variable_operands,
                             instdata -> name, instruction, p + 1);
    instruction_name = instdata -> name;
  } else {
    if ((-instruction) > codegen_directive_count) return generate_string("unknown directive: %d", -instruction);
    instruction_name = codegen_directive_names[~instruction];
    if (!argument_count) return generate_string("no arguments passed to directive %s(%d)", instruction_name, -instruction);
    for (p = 0; p < argument_count; p ++) {
      if (arguments[p].type >= ARGTYPE_INVALID) return generate_string("invalid argument type: %hhu", arguments[p].type);
      if (value_in_list(arguments[p].type, ARGTYPE_REGISTER, ARGTYPE_NAMED_REGISTER, -1))
        return generate_string("argument %u for directive %s(%d) is a variable", p + 1, instruction_name, -instruction);
    }
  }
  unsigned result_length = strlen(instruction_name) + 1;
  char * result = malloc(result_length + 1);
  *result = '\t';
  memcpy(result + 1, instruction_name, result_length); // the +1 in result_length already includes the trailing null
  char * formatted_argument;
  char * error;
  unsigned argument_length;
  for (p = 0; p < argument_count; p ++) {
    formatted_argument = generate_formatted_argument(file, arguments[p], &error);
    if (!formatted_argument) {
      free(result);
      return error;
    }
    argument_length = strlen(formatted_argument);
    if (p) {
      result = realloc(result, result_length + argument_length + 3);
      result[result_length ++] = ',';
    } else
      result = realloc(result, result_length + argument_length + 2);
    result[result_length ++] = ' ';
    memcpy(result + result_length, formatted_argument, argument_length + 1);
    result_length += argument_length;
    free(formatted_argument);
  }
  add_line_to_codefile(file, result);
  free(result);
  return NULL;
}
