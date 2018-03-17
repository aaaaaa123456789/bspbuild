#ifndef ___PATCH

#define ___PATCH

#include "../codegen/pubtypes.h"
#include "../util/pubtypes.h"

#include "pubtypes.h"

// main.c
char * write_patch_to_codefile(CodeFile codefile, Buffer source, Buffer target, struct patching_flags * flags);

// misc.c
signed char find_patch_method_by_name(const char * method_name);

#endif
