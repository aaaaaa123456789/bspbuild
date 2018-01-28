#ifndef ___OPTPARSE

#define ___OPTPARSE

#include "pubtypes.h"

// main.c
Options parse_options(char ** arguments, unsigned argument_count);
void destroy_options_object(Options options);

#endif
