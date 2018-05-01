#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <limits.h>
#include <ctype.h>

#include "../headers/const.h"

#include "struct.h"
#include "public.h"

// file.c
FILE * open_file(const char *, const char *, const char *, char **);

// generic.c
int compare_numbers(const void *, const void *);

// mem.c
void * mr_create_node(struct memory_region **, struct memory_region *);

// sha1.c
void sha1_process_block(const unsigned char *, uint32_t *);
