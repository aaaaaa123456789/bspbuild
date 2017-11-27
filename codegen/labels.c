#include "proto.h"

int declare_label_for_codefile (CodeFile file, const char * label) {
  unsigned p;
  char * prefixed_label;
  if (*label == '.') {
    if (!validate_named_object(label + 1)) return -1;
    prefixed_label = duplicate_string(label);
  } else {
    if (!validate_named_object(label)) return -1;
    prefixed_label = generate_prefixed_label(file, label);
  }
  for (p = 0; p < file -> label_count; p ++) if (file -> labels[p] && !strcmp(prefixed_label, file -> labels[p])) {
    free(prefixed_label);
    return -1;
  }
  file -> labels = realloc(file -> labels, sizeof(char *) * (file -> label_count + 1));
  file -> labels[file -> label_count] = prefixed_label;
  return file -> label_count ++;
}

int add_label_to_codefile (CodeFile file, const char * label) {
  int result = declare_label_for_codefile(file, label);
  if (result < 0) return result;
  add_declared_label_to_codefile(file, result);
  return result;
}

int add_declared_label_to_codefile (CodeFile file, unsigned label_number) {
  if (label_number >= file -> label_count) return -1;
  flush_declared_local_labels(file);
  add_formatted_line_to_codefile(file, "%s:", file -> labels[label_number]);
  return 0;
}

int add_local_label_to_codefile (CodeFile file, const char * label) {
  if (!validate_named_object(label)) return -1;
  add_formatted_line_to_codefile(file, ".%s", label);
  return 0;
}

int declare_numeric_local_for_codefile (CodeFile file) {
  return ++ (file -> next_numeric_local);
}

int add_numeric_local_to_codefile (CodeFile file) {
  add_declared_numeric_local_to_codefile(file, ++ (file -> next_numeric_local));
  return file -> next_numeric_local;
}

int add_declared_numeric_local_to_codefile (CodeFile file, unsigned numeric_local) {
  if (!numeric_local || (numeric_local > file -> next_numeric_local)) return -1;
  char buffer[16];
  sprintf(buffer, "L%u", numeric_local);
  if (add_local_label_to_codefile(file, buffer) < 0) return -1;
  return 0;
}

int declare_numeric_data_label_for_codefile (CodeFile file) {
  return ++ (file -> next_numeric_data);
}

int add_numeric_data_label_to_codefile (CodeFile file) {
  add_declared_numeric_data_label_to_codefile(file, ++ (file -> next_numeric_data));
  return file -> next_numeric_data;
}

int add_declared_numeric_data_label_to_codefile (CodeFile file, unsigned number) {
  if (!number || (number > file -> next_numeric_data)) return -1;
  char buffer[16];
  sprintf(buffer, "Data%u", number);
  char * prefixed = generate_prefixed_label(file, buffer);
  flush_declared_local_labels(file);
  add_formatted_line_to_codefile(file, "%s:", prefixed);
  free(prefixed);
  return 0;
}

char * generate_prefixed_label (CodeFile file, const char * label) {
  return generate_named_object(label, file -> label_prefix);
}

void flush_declared_local_labels (CodeFile file) {
  file -> next_numeric_local = 0;
  unsigned p;
  int last = -1;
  for (p = 0; p < file -> label_count; p ++) if (file -> labels[p])
    if (*(file -> labels[p]) == '.') {
      if (last < 0) last = p;
      free(file -> labels[p]);
      file -> labels[p] = NULL;
    } else
      last = -1;
  if (last < 0) return;
  file -> label_count = last;
  file -> labels = realloc(file -> labels, sizeof(char *) * last);
}
