#include "proto.h"

void initialize_script_data (void) {
  script_data = calloc(1, sizeof(struct script_data));
}

void append_data_to_script (const char * data, unsigned length) {
  script_data = realloc(script_data, sizeof(struct script_data) + script_data -> length + length);
  memcpy(script_data -> data + script_data -> length, data, length);
  script_data -> length += length;
}

void append_binary_file_to_script (const char * file) {
  char * prev_current_file = current_file;
  current_file = NULL;
  char * error;
  FILE * fp = open_binary_file(file, &error);
  if (error) error_exit("%s", error);
  char * buffer = malloc(65536);
  unsigned rv;
  while (!feof(fp)) {
    rv = fread(buffer, 1, 65536, fp);
    if ((!rv) && ferror(fp)) error_exit("could not read data from binary file %s", file);
    append_data_to_script(buffer, rv);
  }
  fclose(fp);
  free(buffer);
  current_file = prev_current_file;
}

void write_halfword_to_buffer (void * buffer, unsigned short number) {
  write_number_to_buffer(buffer, number, 2);
}

void write_word_to_buffer (void * buffer, unsigned number) {
  write_number_to_buffer(buffer, number, 4);
}

void write_number_to_buffer (unsigned char * buffer, unsigned number, unsigned char length) {
  unsigned char pos;
  for (pos = 0; pos < length; pos ++) {
    *(buffer ++) = number;
    number >>= 8;
  }
}
