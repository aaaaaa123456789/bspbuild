#include <stdlib.h>
#include <string.h>

#include "../headers/const.h"
#include "../codegen/public.h"
#include "../ips/public.h"
#include "../util/public.h"

#include "public.h"
#include "struct.h"
#include "patchgen.h"

// fragperm.c
char * generate_fragment_permutation_table(Buffer, Buffer, const struct patching_flags *, struct fragment_permutation_table **);
char * generate_identity_fragment_permutation_table(unsigned, unsigned, const struct patching_flags *, struct fragment_permutation_table **);
char * write_fragment_permutation_table_to_codefile(CodeFile, const struct fragment_permutation_table *, int);

// global.c
extern const char * const patching_method_names[];
extern char * (* const patch_generator_functions[])(CodeFile, Buffer, Buffer, const struct patching_flags *);

// ips.c
char * write_ips_patch_data_for_buffers(CodeFile, Buffer, Buffer);

// main.c
char * write_patch_data_to_codefile(CodeFile, unsigned, const unsigned *, const char *);

// xor.c
char * write_xor_patch_data_without_fragments(CodeFile, Buffer, Buffer, const struct patching_flags *);
