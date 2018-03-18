#include "proto.h"

char * write_ips_patch_data (CodeFile codefile, Buffer source, Buffer target, const struct patching_flags * flags) {
  if (!(flags -> reversible_patch)) return write_ips_patch_data_for_buffers(codefile, source, target);
  char * result = write_patch_data_to_codefile(codefile, 0, NULL, "reverse");
  if (result) return result;
  add_blank_line_to_codefile(codefile);
  if (result = write_ips_patch_data_for_buffers(codefile, source, target)) return result;
  add_blank_line_to_codefile(codefile);
  if (add_local_label_to_codefile(codefile, "reverse") < 0) return duplicate_string("could not declare local label '.reverse'");
  return write_ips_patch_data_for_buffers(codefile, target, source);
}

char * write_ips_patch_data_for_buffers (CodeFile codefile, Buffer source, Buffer target) {
  // ...
}
