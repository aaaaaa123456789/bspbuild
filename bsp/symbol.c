#include "proto.h"

struct bsp_symbol * new_symbol (const char * name, unsigned value) {
  struct bsp_symbol * result = mr_malloc(bsp_memory_region, sizeof(struct bsp_symbol) + strlen(name) + 1);
  result -> value = value;
  strcpy(result -> name, name);
  return result;
}

struct bsp_symbol * find_symbol (const char * name) {
  return find_identifier_by_name(name, script_data -> symbols, script_data -> symbol_count);
}

struct bsp_symbol * find_definition (const char * name) {
  return find_identifier_by_name(name, script_data -> definitions, script_data -> definition_count);
}

struct bsp_symbol * find_identifier_by_name (const char * identifier, struct bsp_symbol ** identifiers, unsigned count) {
  unsigned pos;
  for (pos = 0; pos < count; pos ++) if (!strcmp(identifiers[pos] -> name, identifier)) return identifiers[pos];
  return NULL;
}

void create_symbol (const char * name, unsigned value) {
  script_data -> symbols = mr_realloc(bsp_memory_region, script_data -> symbols, sizeof(struct bsp_symbol *) * (script_data -> symbol_count + 1));
  script_data -> symbols[script_data -> symbol_count ++] = new_symbol(name, value);
}

void create_definition (const char * name, unsigned value) {
  script_data -> definitions = mr_realloc(bsp_memory_region, script_data -> definitions, sizeof(struct bsp_symbol *) * (script_data -> definition_count + 1));
  script_data -> definitions[script_data -> definition_count ++] = new_symbol(name, value);
}

void create_reference (const char * target, unsigned value) {
  script_data -> references = mr_realloc(bsp_memory_region, script_data -> references, sizeof(struct bsp_symbol *) * (script_data -> reference_count + 1));
  script_data -> references[script_data -> reference_count ++] = new_symbol(target, value);
}

void resolve (const struct bsp_symbol * symbol) {
  unsigned pos = 0;
  while (pos < script_data -> reference_count) {
    if (strcmp(symbol -> name, script_data -> references[pos] -> name)) {
      pos ++;
      continue;
    }
    write_word_to_buffer(script_data -> data + script_data -> references[pos] -> value, symbol -> value);
    mr_free(bsp_memory_region, script_data -> references[pos]);
    script_data -> references[pos] = script_data -> references[-- script_data -> reference_count];
    script_data -> references = mr_realloc(bsp_memory_region, script_data -> references, sizeof(struct bsp_symbol *) * script_data -> reference_count);
  }
}
