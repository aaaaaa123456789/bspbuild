#include "proto.h"

void process_input_file_options (Options options) {
  reorder_input_files(options) // note that this is just one statement; function calls are chained by &&
  && (options -> opening_message_from_file ? convert_options_filename_to_text(options, &(options -> messages.opening)) : 1)
  && (options -> success_message_from_file ? convert_options_filename_to_text(options, &(options -> messages.success)) : 1)
  && (options -> error_message_from_file ? convert_options_filename_to_text(options, &(options -> messages.error)) : 1)
  && (options -> label_file ? process_label_file(options) : 1);
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

int process_label_file (Options options) {
  /*
     Label file format:
     filename=label: apply a label to a specific file
     label: apply a label to the next unlabelled file
     =label: same as above, but ensure that = characters are escaped
     blank lines are ignored; to assign a blank (null) label, enter a = sign on a line of its own
     lines starting with // are comments; the // must come at the beginning of the line (use =// to create a label beginning with //)
     extra lines and mismatched filenames are ignored; missing lines become null labels
  */
  char * error;
  FILE * fp = open_text_file(options -> label_file, &error);
  if (error) {
    options -> error_text = copy_string_for_options(options, error);
    free(error);
    return 0;
  }
  unsigned line_count;
  char ** lines = read_file_as_lines(fp, &line_count);
  fclose(fp);
  unsigned * line_types = malloc(sizeof(unsigned) * line_count);
  unsigned line, p;
  for (line = 0; line < line_count; line ++) line_types[line] = line | (get_label_file_line_type(lines[line]) << 30);
  sort_number_array(line_types, line_count);
  for (line = 0; (line < line_count) && !(line_types[line] & (3U << 30)); line ++);
  for (; (line < line_count) && ((line_types[line] & (3U << 30)) == 1); line ++) assign_named_file_label(options, lines[line_types[line] & 0x3fffffffU]);
  if (line != line_count) {
    for (p = line; p < line_count; p ++) line_types[p] &= 0x3fffffffU;
    assign_unnamed_file_labels(options, lines, line_types + line, line_count - line);
  }
  free(line_types);
  destroy_string_array(lines, line_count);
  return 1;
}

unsigned get_label_file_line_type (const char * line) {
  // 0: empty/ignored, 1: named, 2: unnamed
  switch (*line) {
    case 0:
      return 0;
    case '=':
      return 2;
    case '/':
      if (line[1] == '/') return 0;
  }
  return strchr(line, '=') ? 1 : 2;
}

void assign_named_file_label (Options options, const char * line) {
  char * label_part = strchr(line, '=');
  if (!label_part) return; // should not happen
  unsigned file, filename_length = (label_part ++) - line;
  for (file = 0; file < options -> input_file_count; file ++)
    if (!(strncmp(line, options -> input_files[file].name, filename_length) || options -> input_files[file].name[filename_length]))
      options -> input_files[file].label = copy_string_for_options(options, label_part);
}

void assign_unnamed_file_labels (Options options, char ** lines, const unsigned * line_numbers, unsigned line_count) {
  unsigned file, line = 0;
  const char * label;
  for (file = 0; (file < options -> input_file_count) && (line < line_count); file ++) {
    if (options -> input_files[file].label) continue;
    label = lines[line_numbers[line ++]];
    if (*label == '=') label ++;
    options -> input_files[file].label = copy_string_for_options(options, label);
  }
}
