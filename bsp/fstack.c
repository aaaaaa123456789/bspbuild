#include "proto.h"

void push_file (const char * file) {
  current_file = NULL;
  char * error;
  FILE * fp = open_text_file(file, &error);
  if (error) error_exit("%s", error);
  char * filename = duplicate_string(file);
  if (file_stack_length) file_stack -> line = current_line;
  file_stack = realloc(file_stack, sizeof(struct file_stack_entry) * (file_stack_length + 1));
  memmove(file_stack + 1, file_stack, file_stack_length * sizeof(struct file_stack_entry));
  file_stack_length ++;
  *file_stack = (struct file_stack_entry) {.fp = fp, .name = filename, .line = 0};
  current_file = filename;
  current_line = 0;
}

void pop_file (void) {
  fclose(file_stack -> fp);
  free(file_stack -> name);
  file_stack_length --;
  memmove(file_stack, file_stack + 1, file_stack_length * sizeof(struct file_stack_entry));
  if (file_stack_length) {
    file_stack = realloc(file_stack, sizeof(struct file_stack_entry) * file_stack_length);
    current_file = file_stack -> name;
    current_line = file_stack -> line;
  } else {
    free(file_stack);
    file_stack = NULL;
    current_file = NULL;
    current_line = 0;
  }
}

char * get_line_from_input (void) {
  return read_line(file_stack -> fp);
}
