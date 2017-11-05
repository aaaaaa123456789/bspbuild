#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "struct.h"
#include "public.h"

// file.c
FILE * open_file(const char *, const char *, const char *, char **);

// mem.c
void * mr_create_node(struct memory_region **, struct memory_region *);

// string.c
unsigned char get_hex_digit(char);
