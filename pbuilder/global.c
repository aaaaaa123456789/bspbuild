#include <setjmp.h>

#include "../headers/null.h"
#include "struct.h"
#include "engines.h"

void * builder_memory_region = NULL;

struct code_generation_state * builder_state = NULL;

// not part of the memory region
jmp_buf builder_return_point;
char * builder_error;

const char * const code_error_messages[] = {
  "Invalid source file",
  "Selected target matches source file",
  "Output file size mismatch",
  "Output file hash mismatch",
  "Patch cannot be applied in reverse",
  "Padding block size not multiple of padding value size"
};

void (* const patch_engine_generator_functions[])(void) = {
  &define_apply_xor_patch_function,
  &define_apply_xor_rle_patch_function,
  &define_apply_ips_patch_function
};
