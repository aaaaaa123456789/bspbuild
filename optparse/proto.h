#include <string.h>

#include "public.h"
#include "struct.h"

#include "../util/public.h"
#include "../headers/version.h"

// ctordtor.c
Options new_options_object(void);

// main.c
int parse_option(Options, const char *, const char *);
void parse_naked_argument(Options, const char *);

// special.c
Options special_option_response(const char *);
Options no_arguments_response(void);
Options version_response(void);
Options help_response(void);

// validate.c
void validate_options(Options);
