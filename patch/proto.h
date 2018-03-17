#include <stdlib.h>
#include <string.h>

#include "../headers/const.h"
#include "../codegen/public.h"
#include "../util/public.h"

#include "public.h"

// global.c
extern const char * const patching_method_names[];
extern char * (* const patch_generator_functions[])(CodeFile, Buffer, Buffer, const struct patching_flags *);
