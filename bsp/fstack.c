#include "proto.h"

void bsp_push_file (const char * file) {
  current_file = NULL;
  char * error;
  FILE * fp = open_text_file(file, &error);
  if (error) bsp_throw_error("%s", error);
  char * filename = duplicate_string(file);
  if (file_stack_length) file_stack -> line = current_line;
  file_stack = mr_realloc(bsp_memory_region, file_stack, sizeof(struct file_stack_entry) * (file_stack_length + 1));
  memmove(file_stack + 1, file_stack, file_stack_length * sizeof(struct file_stack_entry));
  file_stack_length ++;
  *file_stack = (struct file_stack_entry) {.fp = fp, .name = filename, .line = 0};
  current_file = filename;
  current_line = 0;
}

void bsp_pop_file (void) {
  fclose(file_stack -> fp);
  mr_free(bsp_memory_region, file_stack -> name);
  file_stack_length --;
  memmove(file_stack, file_stack + 1, file_stack_length * sizeof(struct file_stack_entry));
  if (file_stack_length) {
    file_stack = mr_realloc(bsp_memory_region, file_stack, sizeof(struct file_stack_entry) * file_stack_length);
    current_file = file_stack -> name;
    current_line = file_stack -> line;
  } else {
    mr_free(bsp_memory_region, file_stack);
    file_stack = NULL;
    current_file = NULL;
    current_line = 0;
  }
}

char * get_line_from_input (void) {
  return read_line(file_stack -> fp);
}
