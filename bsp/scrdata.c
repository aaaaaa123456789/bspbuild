#include "proto.h"

void initialize_script_data (void) {
  script_data = mr_calloc(bsp_memory_region, sizeof(struct script_data));
}

void append_data_to_script (const char * data, unsigned length) {
  script_data = mr_realloc(bsp_memory_region, script_data, sizeof(struct script_data) + script_data -> length + length);
  memcpy(script_data -> data + script_data -> length, data, length);
  script_data -> length += length;
}

void append_binary_file_to_script (const char * file) {
  char * prev_current_file = current_file;
  current_file = NULL;
  char * error;
  FILE * fp = open_binary_file(file, &error);
  if (error) bsp_throw_error("%s", error);
  char * buffer = mr_malloc(bsp_memory_region, 65536);
  unsigned rv;
  while (!feof(fp)) {
    rv = fread(buffer, 1, 65536, fp);
    if ((!rv) && ferror(fp)) bsp_throw_error("could not read data from binary file %s", file);
    append_data_to_script(buffer, rv);
  }
  fclose(fp);
  mr_free(bsp_memory_region, buffer);
  current_file = prev_current_file;
}
