#include "../codegen/pubtypes.h"
#include "../util/pubtypes.h"

#include "pubtypes.h"

// ips.c
char * write_ips_patch_data(CodeFile codefile, Buffer source, Buffer target, const struct patching_flags * flags);

// xor.c
char * write_xor_patch_data(CodeFile codefile, Buffer source, Buffer target, const struct patching_flags * flags);

// xorrle.c
char * write_xor_rle_patch_data(CodeFile codefile, Buffer source, Buffer target, const struct patching_flags * flags);
