#ifndef ___PATCH

#define ___PATCH

#include "../codegen/pubtypes.h"
#include "../util/pubtypes.h"

#include "pubtypes.h"

// main.c
char * write_patch_to_codefile(CodeFile codefile, Buffer source, Buffer target, const struct patching_flags * flags);
signed char find_patch_method_by_name(const char * method_name);
const char * get_patch_method_name(unsigned char method);

#endif
