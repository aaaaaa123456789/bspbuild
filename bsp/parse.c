#include "proto.h"

void process_input_line (const char * line) {
  if (!*line) return;
  char * pos;
  char * copy;
  if (!strchr("\t ", *line)) {
    pos = strchr(line, ':');
    if (pos) {
      copy = mr_malloc(bsp_memory_region, pos - line + 1);
      memcpy(copy, line, pos - line);
      copy[pos - line] = 0;
      line = pos + 1;
    } else {
      copy = duplicate_string(line);
      line = NULL;
    }
    declare_label(copy);
    mr_free(bsp_memory_region, copy);
    if (!(line && *line)) return;
  }
  char ** components = extract_components_from_line(line);
  if (!components) return;
  struct bsp_command * command = find_command(*components);
  if (!command) error_exit("unknown command: %s", *components);
  command -> parser(command -> argument, components + 1);
  destroy_component_array(components);
}

char ** extract_components_from_line (const char * line) {
  char * copy = duplicate_string(line);
  int pos = find_unquoted_character(copy, ';');
  if (pos >= 0) copy[pos] = 0;
  char * current = copy + strspn(copy, " \t");
  if ((!*current)) {
    mr_free(bsp_memory_region, copy);
    return NULL;
  }
  pos = strcspn(current, " \t");
  char ** result;
  if (!current[pos]) {
    result = mr_malloc(bsp_memory_region, sizeof(char *) * 2);
    *result = duplicate_string(current);
    result[1] = NULL;
    return result;
  }
  result = mr_malloc(bsp_memory_region, sizeof(char *));
  current[pos] = 0;
  *result = duplicate_string(current);
  current += pos + 1;
  unsigned components = 1;
  char * component;
  while (pos >= 0) {
    pos = find_unquoted_character(current, ',');
    if (pos >= 0) current[pos] = 0;
    component = trim_string(current);
    if (pos >= 0) current += pos + 1;
    if (*component) {
      result = mr_realloc(bsp_memory_region, result, sizeof(char *) * (components + 1));
      result[components ++] = component;
    } else
      mr_free(bsp_memory_region, component);
  }
  result = mr_realloc(bsp_memory_region, result, sizeof(char *) * (components + 1));
  result[components] = NULL;
  return result;
}

void destroy_component_array (char ** array) {
  char ** current;
  for (current = array; *current; current ++) mr_free(bsp_memory_region, *current);
  mr_free(bsp_memory_region, array);
}

unsigned count_parameters (char ** components) {
  if (!components) return -1;
  unsigned count = 0;
  while (components[count ++]);
  return count - 1;
}

int find_unquoted_character (const char * string, char character) {
  unsigned char in_quotes = 0;
  unsigned pos;
  const char * p;
  for (p = string, pos = 0; *p; p ++, pos ++) {
    if (*p == '"') {
      in_quotes ^= 1;
      continue;
    }
    if (in_quotes) continue;
    if (*p == character) return pos;
  }
  if (in_quotes) error_exit("mismatched quotes in string: %s", string);
  return -1;
}

struct bsp_command * find_command (const char * command_name) {
  struct bsp_command * result;
  for (result = script_commands; result -> name; result ++) if (!strcmp(result -> name, command_name)) return result;
  return NULL;
}
