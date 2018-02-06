#include "proto.h"

int get_defined_label (int * ID, const char * name) {
  if (*ID >= 0) return *ID;
  *ID = declare_label_for_codefile(builder_state -> codefile, name);
  if (*ID < 0) builder_throw("could not declare label '%s'", name);
  return *ID;
}
