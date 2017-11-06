#include <setjmp.h>

#include "struct.h"

char * current_file = NULL;
unsigned current_line = 0;

struct file_stack_entry * file_stack;
unsigned file_stack_length;

struct script_data * script_data = NULL;

jmp_buf bsp_return_point;
char * volatile bsp_error;

void * bsp_memory_region = NULL;
