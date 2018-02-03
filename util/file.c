#include "proto.h"

FILE * open_text_file (const char * file, char ** error) {
  return open_file(file, "r", "reading", error);
}

FILE * open_binary_file (const char * file, char ** error) {
  return open_file(file, "rb", "reading", error);
}

FILE * open_binary_file_for_writing (const char * file, char ** error) {
  return open_file(file, "wb", "writing", error);
}

FILE * open_file (const char * file, const char * mode, const char * mode_description, char ** error) {
  FILE * fp = fopen(file, mode);
  if (!fp)
    *error = generate_string("could not open file %s for %s", file, mode_description);
  else
    *error = NULL;
  return fp;
}

char * read_line (FILE * fp) {
  char * line = NULL;
  unsigned length = 0;
  int c;
  while (1) {
    c = getc(fp);
    if ((c == EOF) || (c == '\n')) break;
    if (c == '\r') continue;
    line = realloc(line, length + 1);
    line[length ++] = c;
  }
  line = realloc(line, length + 1);
  line[length] = 0;
  return line;
}

char ** read_file_as_lines (FILE * fp, unsigned * line_count) {
  char ** lines = NULL;
  unsigned count = 0;
  char * line;
  while (!feof(fp)) {
    line = read_line(fp);
    lines = realloc(lines, sizeof(char *) * (count + 1));
    lines[count ++] = line;
  }
  lines = realloc(lines, sizeof(char *) * (count + 1));
  lines[count] = NULL;
  if (line_count) *line_count = count;
  return lines;
}

int write_data_to_file (FILE * fp, const void * data, unsigned length) {
  unsigned rv;
  const char * current = data;
  while (length) {
    rv = fwrite(current, 1, (length > 65536) ? 65536 : length, fp);
    if (!rv) return 0;
    current += rv;
    length -= rv;
  }
  return 1;
}

long get_file_length (FILE * fp) {
  fpos_t saved_pos;
  if (fgetpos(fp, &saved_pos) < 0) return -1;
  long result = -1;
  if (fseek(fp, 0, 2) >= 0) result = ftell(fp);
  fsetpos(fp, &saved_pos);
  return result;
}

char * write_buffer_to_new_file (const char * filename, Buffer buffer) {
  char * error;
  FILE * fp = open_binary_file_for_writing(filename, &error);
  if (error) return error;
  int rv = write_data_to_file(fp, buffer -> data, buffer -> length);
  fclose(fp);
  if (!rv) return generate_string("could not write data to %s", filename);
  return NULL;
}
