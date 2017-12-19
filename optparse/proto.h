#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "../util/public.h"
#include "../headers/version.h"
#include "../headers/const.h"

#include "public.h"
#include "struct.h"
#include "options.h"

// ctordtor.c
Options new_options_object(void);

// main.c
int parse_option(Options, const char *, const char *);
void parse_naked_argument(Options, const char *);

// misc.c
char * multiple_option_response(const char *);
char * generate_string_for_options(Options, const char *, ...);
char * copy_string_for_options(Options, const char *);

// opttable.c
extern const struct option_parser option_parsers[];

// special.c
Options special_option_response(const char *);
Options no_arguments_response(void);
Options version_response(void);
Options help_response(void);

// validate.c
void validate_options(Options);
