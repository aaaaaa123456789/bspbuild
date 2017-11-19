#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "public.h"
#include "struct.h"

#include "../util/public.h"

// data.c
const char * find_next_invalid_string_character(const char *);
unsigned count_invalid_string_characters(const char *);

// labels.c
char * generate_prefixed_label(CodeFile, const char *);

// namedobj.c
char * generate_prefixed_register(CodeFile, const char *);
char * generate_prefixed_constant(CodeFile, const char *);
char * generate_named_object(const char *, const char *);
int validate_named_object(const char *);
