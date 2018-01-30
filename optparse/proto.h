#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "../util/public.h"
#include "../headers/version.h"
#include "../headers/const.h"
#include "../patch/public.h"

#include "public.h"
#include "struct.h"
#include "options.h"

// files.c
void process_input_file_options(Options);
int reorder_input_files(Options);
int convert_options_filename_to_text(Options, char **);
int process_label_file(Options);
unsigned get_label_file_line_type(const char *);
void assign_named_file_label(Options, const char *);
void assign_unnamed_file_labels(Options, char **, const unsigned *, unsigned);

// main.c
Options new_options_object(void);
int parse_option(Options, const char *, const char *);

// misc.c
char * multiple_option_response(const char *);
char * generate_string_for_options(Options, const char *, ...);
char * copy_string_for_options(Options, const char *);
void parse_naked_argument(Options, const char *);

// opttable.c
extern const struct option_parser option_parsers[];

// special.c
Options special_option_response(const char *);
Options no_arguments_response(void);
Options version_response(void);
Options help_response(void);

// validate.c
void validate_options(Options);
char * validate_input_file_options(Options);
char * validate_output_file_options(Options);
char * validate_nonsensical_option_combinations(Options);
char * validate_options_in_bsp_mode(Options);
char * validate_options_in_ips_mode(Options);
char * validate_options_in_nonstandard_modes(Options, const char *);
