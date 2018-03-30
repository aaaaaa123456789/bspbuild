#include "proto.h"

char * write_xor_rle_patch_data (CodeFile codefile, Buffer source, Buffer target, const struct patching_flags * flags) {
  return write_xor_like_patch(codefile, source, target, flags, &write_xor_rle_patch_buffer_data);
}

char * write_xor_rle_patch_buffer_data (CodeFile codefile, const unsigned char * data, unsigned length) {
  // ...
}
