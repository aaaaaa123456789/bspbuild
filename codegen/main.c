#include "proto.h"

CodeFile new_codefile (void) {
  CodeFile result = create_codefile_object();
  result -> label_prefix = result -> constant_prefix = result -> register_prefix = NULL;
  return result;
}

CodeFile new_codefile_with_prefix (const char * prefix) {
  if (!validate_named_object(prefix)) return NULL;
  CodeFile result = create_codefile_object();
  result -> label_prefix = duplicate_string(prefix);
  result -> register_prefix = convert_label_prefix_to_register_prefix(prefix);
  char * cp = duplicate_string(result -> register_prefix);
  string_to_uppercase(cp);
  result -> constant_prefix = cp;
  return result;
}

CodeFile new_codefile_with_prefixes (const char * label_prefix, const char * register_prefix, const char * constant_prefix) {
  if (!(validate_named_object(label_prefix) && validate_named_object(register_prefix) && validate_named_object(constant_prefix))) return NULL;
  CodeFile result = create_codefile_object();
  result -> label_prefix = duplicate_string(label_prefix);
  result -> register_prefix = duplicate_string(register_prefix);
  result -> constant_prefix = duplicate_string(constant_prefix);
  return result;
}

void destroy_codefile (CodeFile file) {
  // ...
}

char * export_codefile_data (CodeFile file, unsigned * length) {
  // ...
}

char * convert_codefile_to_data (CodeFile file, unsigned * length) {
  char * result = export_codefile_data(file, length);
  destroy_codefile(file);
  return result;
}

CodeFile create_codefile_object (void) {
  // ...
}
