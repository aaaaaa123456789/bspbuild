#include "../codegen/pubtypes.h"
#include "../util/pubtypes.h"
#include "pubtypes.h"

const char * const patching_method_names[] = {
  "xor",
  "xor-rle",
  "ips"
};

char * (* const patch_generator_functions[])(CodeFile, Buffer, Buffer, const struct patching_flags *) = {
  // ...
};
