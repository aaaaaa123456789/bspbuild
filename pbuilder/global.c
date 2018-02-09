#include <setjmp.h>

#include "struct.h"
#include "../headers/null.h"

void * builder_memory_region = NULL;

struct code_generation_state * builder_state = NULL;

// not part of the memory region
jmp_buf builder_return_point;
char * builder_error;

const char * const code_error_messages[] = {
  "Invalid source file",
  "Selected target matches source file"
};
