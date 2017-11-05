#include "proto.h"

void declare_label (const char * label) {
  int valid;
  if (*label == '.')
    valid = validate_label(label + 1);
  else {
    flush_locals();
    valid = validate_label(label);
  }
  if (!valid) bsp_throw_error("invalid label: %s", label);
  if (find_symbol(label)) bsp_throw_error("label '%s' already exists", label);
  if (find_definition(label)) bsp_throw_error("label '%s' overrides a define statement", label);
  create_symbol(label, script_data -> length);
}

void flush_locals (void) {
  unsigned pos = 0;
  while (pos < script_data -> symbol_count) {
    if (*(script_data -> symbols[pos] -> name) != '.') {
      pos ++;
      continue;
    }
    resolve(script_data -> symbols[pos]);
    mr_free(bsp_memory_region, script_data -> symbols[pos]);
    script_data -> symbols[pos] = script_data -> symbols[-- script_data -> symbol_count];
    script_data -> symbols = mr_realloc(bsp_memory_region, script_data -> symbols, sizeof(struct bsp_symbol *) * script_data -> symbol_count);
  }
  for (pos = 0; pos < script_data -> reference_count; pos ++) if (*(script_data -> references[pos] -> name) == '.')
    bsp_throw_error("encountered new global label before local '%s' was resolved", script_data -> references[pos] -> name);
}

void flush_all_symbols (void) {
  unsigned pos;
  for (pos = 0; pos < script_data -> symbol_count; pos ++) {
    resolve(script_data -> symbols[pos]);
    mr_free(bsp_memory_region, script_data -> symbols[pos]);
  }
  mr_free(bsp_memory_region, script_data -> symbols);
  script_data -> symbols = NULL;
  script_data -> symbol_count = 0;
  if (script_data -> reference_count) bsp_throw_error("unresolved label: %s", (**(script_data -> references)).name);
  mr_free(bsp_memory_region, script_data -> references);
  script_data -> references = NULL;
}

int validate_label (const char * label) {
  return (*label) && (strspn(label, VALID_ID_CHARACTERS) == strlen(label)) && strchr(LETTERS_OR_UNDERSCORE, *label);
}
