#include "proto.h"

void generate_runtime (void) {
  define_main_function();
  define_error_function();
  define_detect_input_function();
  define_select_output_function();
  define_generate_output_function();
  if (builder_state -> needed_functions.generate_reverse_output) define_generate_reverse_output_function();
  define_apply_patch_function();
  if (builder_state -> needed_functions.apply_reverse_patch) define_apply_reverse_patch_function();
  define_execute_patch_function();
  define_patch_engine_functions();
  if (builder_state -> needed_functions.print_detected_input) define_print_detected_input_function();
  if (builder_state -> needed_functions.validate_output) define_validate_output_function();
  if (builder_state -> needed_functions.get_nth_string) define_get_nth_string_function();
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
      result[count ++] = mr_duplicate_string(builder_memory_region, banner);
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
  for (current = 0; current < count; current ++) mr_free(builder_memory_region, banner_lines[current]);
  mr_free(builder_memory_region, banner_lines);
}

void builder_declare_local (const char * local_label) {
  if (add_local_label_to_codefile(builder_state -> codefile, local_label) < 0)
    builder_throw("could not declare local label '.%s'", local_label);
}
