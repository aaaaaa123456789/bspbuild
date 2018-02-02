#include "proto.h"

struct bsp_argument * get_argument (const char * string) {
  struct bsp_argument * result;
  if (*string == '#') {
    result = get_argument(string + 1);
    if (result -> kind) bsp_throw_error("invalid variable argument: %s", string);
    if (result -> value > 255) bsp_throw_error("invalid variable number: %u", result -> value);
    result -> kind = 1;
    return result;
  }
  if (strchr(NUMERIC_CHARACTERS, *string)) {
    char * error;
    result = mr_malloc(bsp_memory_region, sizeof(struct bsp_argument));
    result -> kind = 0;
    result -> value = convert_string_to_number(string, &error);
    if (error) bsp_throw_error("%s", error);
    return result;
  }
  struct bsp_symbol * definition = find_definition(string);
  if (definition) {
    result = mr_malloc(bsp_memory_region, sizeof(struct bsp_argument));
    result -> kind = 0;
    result -> value = definition -> value;
    return result;
  }
  if (!(validate_label(string) || ((*string == '.') && validate_label(string + 1)))) bsp_throw_error("invalid label identifier: %s", string);
  result = mr_malloc(bsp_memory_region, sizeof(struct bsp_argument) + strlen(string) + 1);
  result -> kind = 2;
  strcpy(result -> reference, string);
  return result;
}

char * get_string_argument (const char * argument) {
  if (*argument != '"') bsp_throw_error("unquoted string");
  char * result = mr_duplicate_string(bsp_memory_region, argument + 1);
  unsigned length = strlen(result) - 1;
  if (result[length] != '"') bsp_throw_error("unquoted string");
  result[length] = 0;
  char * pos = result;
  while (pos = strstr(pos, "\"\"")) {
    pos ++;
    memmove(pos, pos + 1, strlen(pos));
  }
  result = mr_realloc(bsp_memory_region, result, strlen(result) + 1);
  return result;
}
