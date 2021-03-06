#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include "public.h"
#include "struct.h"

#include "../headers/const.h"
#include "../util/public.h"

// data.c
const char * find_next_invalid_string_character(const char *);
unsigned count_invalid_string_characters(const char *);

// file.c
void add_line_to_codefile(CodeFile, const char *);
void add_formatted_line_to_codefile(CodeFile, const char *, ...);

// format.c
char * generate_formatted_number_for_file(unsigned);
char * generate_formatted_argument(CodeFile, struct instruction_argument, char **);

// insttab.c
extern const struct code_generation_instruction_entry codegen_instruction_table[];
extern const char * const codegen_directive_names[];
extern const unsigned codegen_instruction_count;
extern const unsigned codegen_directive_count;

// labels.c
char * generate_prefixed_label(CodeFile, const char *);
void flush_declared_local_labels(CodeFile);

// namedobj.c
char * generate_prefixed_register(CodeFile, const char *);
char * generate_prefixed_constant(CodeFile, const char *);
char * generate_named_object(const char *, const char *);
int validate_named_object(const char *);
char * convert_label_prefix_to_register_prefix(const char *);
