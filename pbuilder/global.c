#include <setjmp.h>

#include "struct.h"
#include "../headers/null.h"

struct code_generation_state * builder_state = NULL;

jmp_buf builder_return_point;
char * builder_error;
