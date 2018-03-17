#include "../codegen/pubtypes.h"
#include "../util/pubtypes.h"
#include "pubtypes.h"

#include "patchgen.h"

const char * const patching_method_names[] = {
  "xor",
  "xor-rle",
  "ips"
};

char * (* const patch_generator_functions[])(CodeFile, Buffer, Buffer, const struct patching_flags *) = {
  &write_xor_patch_data,
  &write_xor_rle_patch_data,
  &write_ips_patch_data
};
