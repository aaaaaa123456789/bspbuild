#include "proto.h"

void generate_runtime (void) {
  define_main_function();
  define_error_function();
  define_detect_input_function();
  // ...
  if (builder_state -> needed_functions.get_nth_string) define_get_nth_string_function();
  // ...
}

int get_defined_label (int * ID, const char * name) {
  if (*ID >= 0) return *ID;
  *ID = declare_label_for_codefile(builder_state -> codefile, name);
  if (*ID < 0) builder_throw("could not declare label '%s'", name);
  return *ID;
}

int generate_banner_lines (const char * banner, char *** lines) {
  *lines = NULL;
  if (!banner) return 0;
  char ** result = NULL;
  unsigned count = 0;
  const char * pos;
  while (*banner) {
    pos = strchr(banner, '\n');
    result = mr_realloc(builder_memory_region, result, sizeof(char *) * (count + 1));
    if (!pos) {
      result[count ++] = duplicate_string(banner);
      break;
    }
    result[count] = memcpy(mr_malloc(builder_memory_region, pos - banner + 1), banner, pos - banner);
    result[count ++][pos - banner] = 0;
    banner = pos + 1;
  }
  unsigned line, length;
  for (line = 0; line < count; line ++) {
    length = strlen(result[line]);
    if (!length) continue;
    if (result[line][length - 1] == '\r') result[line][length - 1] = 0;
  }
  *lines = result;
  return count;
}

void destroy_banner_lines (char ** banner_lines, unsigned count) {
  unsigned current;
  for (current = 0; current < count; current ++) mr_free(builder_memory_region, banner_lines[count]);
  mr_free(builder_memory_region, banner_lines);
}
