#include "proto.h"

void process_input_file_options (Options options) {
  reorder_input_files(options)
  && (options -> opening_message_from_file ? convert_options_filename_to_text(options, &(options -> messages.opening)) : 1)
  && (options -> success_message_from_file ? convert_options_filename_to_text(options, &(options -> messages.success)) : 1)
  && (options -> error_message_from_file ? convert_options_filename_to_text(options, &(options -> messages.error)) : 1);
  // ...
}

int reorder_input_files (Options options) {
  unsigned * new_file_numbers = malloc(sizeof(unsigned) * options -> input_file_count);
  unsigned pos, total = 0;
  for (pos = 0; pos < options -> input_file_count; pos ++) options -> file_count_per_direction[options -> input_files[pos].direction] ++;
  unsigned current_numbers[3];
  for (pos = 0; pos < 3; pos ++) {
    current_numbers[pos] = total;
    total += options -> file_count_per_direction[pos];
  }
  if (total != options -> input_file_count) {
    options -> error_text = copy_string_for_options(options, "[internal] input file count mismatch");
    return 0;
  }
  for (pos = 0; pos < options -> input_file_count; pos ++) new_file_numbers[pos] = current_numbers[options -> input_files[pos].direction] ++;
  struct option_file * files = malloc(sizeof(struct option_file) * options -> input_file_count);
  for (pos = 0; pos < options -> input_file_count; pos ++) {
    files[new_file_numbers[pos]] = options -> input_files[pos];
    if (files[new_file_numbers[pos]].reference < 0xff0000) files[new_file_numbers[pos]].reference = new_file_numbers[files[new_file_numbers[pos]].reference];
  }
  free(new_file_numbers);
  memcpy(options -> input_files, files, sizeof(struct option_file) * options -> input_file_count);
  free(files);
  return 1;
}

int convert_options_filename_to_text (Options options, char ** text) {
  char * error;
  char * original_filename = *text;
  *text = NULL;
  FILE * fp = open_text_file(original_filename, &error);
  if (error) {
    mr_free(options -> memory_region, original_filename);
    options -> error_text = copy_string_for_options(options, error);
    free(error);
    return 0;
  }
  int rv;
  char buffer[16384];
  unsigned length = 0;
  error = NULL;
  while (!feof(fp)) {
    rv = fread(buffer, 1, 16384, fp);
    if (ferror(fp))
      error = "error reading file";
    else if (check_nulls(buffer, rv))
      error = "null characters in text file:";
    if (error) {
      fclose(fp);
      options -> error_text = generate_string_for_options(options, "%s %s", error, original_filename);
      mr_free(options -> memory_region, original_filename);
      return 0;
    }
    *text = mr_realloc(options -> memory_region, *text, length + rv + 1);
    memcpy(*text + length, buffer, rv);
    length += rv;
    length[*text] = 0;
  }
  fclose(fp);
  mr_free(options -> memory_region, original_filename);
  return 1;
}
