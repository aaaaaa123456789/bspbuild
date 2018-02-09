#include <setjmp.h>

#include "struct.h"
#include "../headers/null.h"

struct code_generation_state * builder_state = NULL;

jmp_buf builder_return_point;
char * builder_error;

const char * const code_error_messages[] = {
  "Invalid source file",
  "Selected target matches source file"
};
