#include <stdlib.h>
#include <string.h>

#include "../headers/const.h"
#include "../codegen/public.h"
#include "../ips/public.h"
#include "../util/public.h"

#include "patchgen.h"
#include "public.h"

// global.c
extern const char * const patching_method_names[];
extern char * (* const patch_generator_functions[])(CodeFile, Buffer, Buffer, const struct patching_flags *);

// ips.c
char * write_ips_patch_data_for_buffers(CodeFile, Buffer, Buffer);

// main.c
char * write_patch_data_to_codefile(CodeFile, unsigned, const unsigned *, const char *);
