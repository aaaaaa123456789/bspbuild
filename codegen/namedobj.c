#include "proto.h"

int add_register_definition_to_codefile (CodeFile file, const char * register_name, unsigned char register_number) {
  unsigned p;
  if (!validate_named_object(register_name)) return -1;
  char * prefixed = generate_prefixed_register(file, register_name);
  for (p = 0; p < file -> register_count; p ++) if (!strcmp(file -> registers[p], prefixed)) {
    free(prefixed);
    return -1;
  }
  file -> registers = realloc(file -> registers, sizeof(char *) * (file -> register_count + 1));
  file -> registers[file -> register_count] = prefixed;
  add_formatted_line_to_codefile(file, "\tdefine %s, %hhu", prefixed, register_number);
  return file -> register_count ++;
}

int add_constant_to_codefile (CodeFile file, const char * constant_name, unsigned constant_value) {
  unsigned p;
  if (!validate_named_object(constant_name)) return -1;
  char * prefixed = generate_prefixed_constant(file, constant_name);
  for (p = 0; p < file -> constant_count; p ++) if (!strcmp(file -> constants[p], prefixed)) {
    free(prefixed);
    return -1;
  }
  file -> constants = realloc(file -> constants, sizeof(char *) * (file -> constant_count + 1));
  file -> constants[file -> constant_count] = prefixed;
  char * fv = generate_formatted_number_for_file(constant_value);
  add_formatted_line_to_codefile(file, "\tdefine %s, %s", prefixed, fv);
  free(fv);
  return file -> constant_count ++;
}

char * generate_prefixed_register (CodeFile file, const char * name) {
  return generate_named_object(name, file -> register_prefix);
}

char * generate_prefixed_constant (CodeFile file, const char * name) {
  return generate_named_object(name, file -> constant_prefix);
}

char * generate_named_object (const char * object, const char * prefix) {
  if (!(prefix && *prefix)) return duplicate_string(object);
  unsigned prefix_length = strlen(prefix), object_length = strlen(object);
  char * result = malloc(object_length + prefix_length + 1);
  memcpy(result, prefix, prefix_length);
  memcpy(result + prefix_length, object, object_length);
  result[prefix_length + object_length] = 0;
  return result;
}

int validate_named_object (const char * name) {
  if (!(name && *name)) return 0;
  if (strspn(name, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_") != strlen(name)) return 0;
  return !strchr("0123456789", *name);
}

char * convert_label_prefix_to_register_prefix (const char * label_prefix) {
  // ...
}
