#include "proto.h"

int add_label_to_codefile (CodeFile file, const char * label) {
  unsigned p;
  if (!validate_new_label(label)) return -1;
  char * prefixed_label = generate_prefixed_label(file, label);
  for (p = 0; p < file -> label_count; p ++) if (!strcmp(prefixed_label, file -> labels[p])) {
    free(prefixed_label);
    return -1;
  }
  file -> labels = realloc(file -> labels, sizeof(char *) * (file -> label_count + 1));
  file -> labels[file -> label_count] = prefixed_label;
  fprintf(file -> fp, "%s:\n", prefixed_label);
  return file -> label_count ++;
}

int add_local_label_to_codefile (CodeFile file, const char * label) {
  if (!validate_new_label(label)) return -1;
  fprintf(file -> fp, ".%s\n", label);
  return 0;
}

int add_numeric_local_to_codefile (CodeFile file) {
  char buffer[16];
  sprintf(buffer, "L%u", ++ (file -> next_numeric_local));
  if (add_local_label_to_codefile(file, buffer) < 0) return -1;
  return file -> next_numeric_local;
}

int add_numeric_data_label_to_codefile (CodeFile file) {
  char buffer[16];
  sprintf(buffer, "Data%u", ++ (file -> next_numeric_data));
  char * prefixed = generate_prefixed_label(file, buffer);
  fprintf(file -> fp, "%s:\n", prefixed);
  free(prefixed);
  return file -> next_numeric_data;
}

char * generate_prefixed_label (CodeFile file, const char * label) {
  return file -> label_prefix ? generate_string("%s%s", file -> label_prefix, label) : duplicate_string(label);
}

int validate_new_label (const char * label) {
  if (!(label && *label)) return 0;
  if (strspn(label, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_") != strlen(label)) return 0;
  return !strchr("0123456789", *label);
}
