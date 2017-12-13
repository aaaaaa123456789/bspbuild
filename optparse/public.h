#ifndef ___OPTPARSE

#define ___OPTPARSE

#include "pubtypes.h"

// ctordtor.c
void destroy_options_object(Options options);

// main.c
Options parse_options(char ** arguments, unsigned argument_count);

#endif
