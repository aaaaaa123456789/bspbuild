#include "proto.h"

CodeFile new_codefile (void) {
  CodeFile result = calloc(1, sizeof(struct code_file_info));
  result -> label_prefix = result -> constant_prefix = result -> register_prefix = NULL;
  return result;
}

CodeFile new_codefile_with_prefix (const char * prefix) {
  if (!validate_named_object(prefix)) return NULL;
  CodeFile result = calloc(1, sizeof(struct code_file_info));
  result -> label_prefix = duplicate_string(prefix);
  result -> register_prefix = convert_label_prefix_to_register_prefix(prefix);
  char * cp = duplicate_string(result -> register_prefix);
  string_to_uppercase(cp);
  result -> constant_prefix = cp;
  return result;
}

CodeFile new_codefile_with_prefixes (const char * label_prefix, const char * register_prefix, const char * constant_prefix) {
  if (!(validate_named_object(label_prefix) && validate_named_object(register_prefix) && validate_named_object(constant_prefix))) return NULL;
  CodeFile result = calloc(1, sizeof(struct code_file_info));
  result -> label_prefix = duplicate_string(label_prefix);
  result -> register_prefix = duplicate_string(register_prefix);
  result -> constant_prefix = duplicate_string(constant_prefix);
  return result;
}

void destroy_codefile (CodeFile file) {
  destroy_string_array(file -> labels, file -> label_count);
  destroy_string_array(file -> constants, file -> constant_count);
  destroy_string_array(file -> registers, file -> register_count);
  destroy_string_array(file -> lines, file -> line_count);
  free(file -> label_prefix);
  free(file -> constant_prefix);
  free(file -> register_prefix);
  free(file);
}

char * export_codefile_data (CodeFile file, unsigned * length) {
  if (!(file -> lines && file -> line_count)) return duplicate_string("");
  unsigned total_length = file -> line_count; // 1 per newline
  unsigned line;
  unsigned * line_lengths = malloc(sizeof(unsigned) * file -> line_count);
  for (line = 0; line < file -> line_count; line ++) {
    line_lengths[line] = strlen(file -> lines[line]);
    total_length += line_lengths[line];
  }
  char * result = malloc(total_length + 1);
  char * current = result;
  for (line = 0; line < file -> line_count; line ++) {
    memcpy(current, file -> lines[line], line_lengths[line]);
    current += line_lengths[line];
    *(current ++) = '\n';
  }
  *current = 0;
  free(line_lengths);
  return result;
}

char * convert_codefile_to_data (CodeFile file, unsigned * length) {
  char * result = export_codefile_data(file, length);
  destroy_codefile(file);
  return result;
}
