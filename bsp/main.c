#include "proto.h"

char * bsp_build_file (const char * output, const char * input) {
  char * files[2];
  *files = (char *) input;
  files[1] = NULL;
  return bsp_build(output, files);
}

char * bsp_build (const char * output, char ** inputs) {
  if (!(inputs && *inputs && output)) return duplicate_string("error: BSP compilation requires at least one input file and one output file");
  bsp_memory_region = create_memory_region();
  bsp_error = NULL;
  int rv = setjmp(bsp_return_point);
  if (!rv) {
    initialize_script_data();
    file_stack = NULL;
    file_stack_length = 0;
    while (*inputs) bsp_parse_file(*(inputs ++));
    flush_all_symbols();
    bsp_write_output(output);
  }
  destroy_memory_region(bsp_memory_region);
  return bsp_error;
}

void bsp_parse_file (const char * file) {
  char * line;
  int comment_start;
  push_file(file);
  while (file_stack_length) {
    current_line ++;
    line = get_line_from_input();
    comment_start = find_unquoted_character(line, ';');
    if (comment_start >= 0) line[comment_start] = 0;
    process_input_line(line);
    free(line);
    if (feof(file_stack -> fp)) pop_file();
  }
  current_file = NULL;
}

void bsp_write_output (const char * output) {
  char * error;
  FILE * fp = open_binary_file_for_writing(output, &error);
  if (error) bsp_throw_error("%s", error);
  if (!write_data_to_file(fp, script_data -> data, script_data -> length)) bsp_throw_error("could not write data to output file");
  fclose(fp);
}

void bsp_throw_error (const char * fmtstring, ...) {
  va_list ap;
  va_start(ap, fmtstring);
  char * error = generate_string_from_varargs(fmtstring, ap);
  va_end(ap);
  bsp_error = malloc(strlen(error) + (current_file ? strlen(current_file) + 20 : 8));
  if (current_file)
    sprintf(bsp_error, "%s:%u: %s", current_file, current_line, error);
  else
    sprintf(bsp_error, "error: %s", error);
  free(error);
  longjmp(bsp_return_point, 1);
}
