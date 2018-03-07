#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <setjmp.h>

#include "../headers/const.h"
#include "../headers/enum.h"
#include "../codegen/public.h"
#include "../optparse/pubtypes.h"
#include "../util/public.h"

#include "struct.h"
#include "public.h"

#define get_label(member_name, label_name) get_defined_label(&(builder_state -> labels.member_name), label_name)

#define inst(...) do {                                                                                    \
  char * ___result = add_instruction_to_codefile(builder_state -> codefile, __VA_ARGS__, ARGTYPE_END);    \
  if (!___result) break;                                                                                  \
  char * ___copy = mr_duplicate_string(builder_memory_region, ___result);                                 \
  free(___result);                                                                                        \
  builder_throw("could not generate instruction: %s", ___copy);                                           \
} while (0)

#define reg(name) ARGTYPE_NAMED_REGISTER, builder_state -> registers.name
#define imm(...) ARGTYPE_IMMEDIATE, (unsigned) (__VA_ARGS__)
#define lbl(...) ARGTYPE_NAMED_LABEL, get_label(__VA_ARGS__)
#define loc(...) ARGTYPE_LOCAL_LABEL, (__VA_ARGS__)
#define cnst(name) ARGTYPE_NAMED_CONSTANT, builder_state -> constants.name
#define err(name) ARGTYPE_NAMED_CONSTANT, builder_state -> constants.errors[name]
#define nloc(...) ARGTYPE_NUMERIC_LOCAL, (unsigned) (__VA_ARGS__)

// errorfn.c
void define_error_function(void);
void simple_error_message_function(char **, unsigned);
void descriptive_error_message_function(char **, unsigned);
int add_string_to_printed_error_messages(int **, char ***, unsigned *, char *);

// global.c
extern void * builder_memory_region;
extern struct code_generation_state * builder_state;
extern jmp_buf builder_return_point;
extern char * builder_error;
extern const char * const code_error_messages[];

// init.c
void initialize_builder_state(Options, CodeFile);
void initialize_code_generator(void);
int declare_register(const char *, unsigned char);
int declare_constant(const char *, unsigned);

// main.c
void builder_throw(const char *, ...);

// mainfn.c
void define_main_function(void);

// runtime.c
void generate_runtime(void);
int get_defined_label(int *, const char *);
int generate_banner_lines(const char *, char ***);
void destroy_banner_lines(char **, unsigned);

// selectfn.c
void define_select_output_function(void);
int check_for_single_output(unsigned);
void show_output_selection(const unsigned *, unsigned);
void show_paged_output_selection(const unsigned *, unsigned);
int compare_target_names(const void *, const void *);
void generate_filename_menu(const unsigned *, unsigned);
void generate_file_numbers_for_menu(const unsigned *, unsigned, int);

// utilfn.c
void define_detect_input_function(void);
void define_print_detected_input_function(void);
void define_validate_output_function(void);
void define_get_nth_string_function(void);
