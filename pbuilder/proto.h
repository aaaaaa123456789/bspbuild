#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>

#include "../codegen/public.h"
#include "../optparse/pubtypes.h"
#include "../util/public.h"

#include "struct.h"
#include "public.h"

// global.c
extern struct code_generation_state * builder_state;
extern jmp_buf builder_return_point;
extern char * builder_error;

// init.c
void initialize_code_generator(void);
int declare_register(const char *, unsigned char);

// main.c
void builder_throw(const char *, ...);
