#include "struct.h"

char * current_file = NULL;
unsigned current_line = 0;

struct file_stack_entry * file_stack = NULL;
unsigned file_stack_length = 0;

struct script_data * script_data = NULL;
