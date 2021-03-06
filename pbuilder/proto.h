#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <setjmp.h>

#include "../headers/const.h"
#include "../headers/enum.h"
#include "../headers/version.h"
#include "../codegen/public.h"
#include "../optparse/pubtypes.h"
#include "../patch/public.h"
#include "../util/public.h"

#include "struct.h"
#include "public.h"
#include "engines.h"

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
#define flbl(...) ARGTYPE_NAMED_LABEL, builder_state -> file_name_labels[__VA_ARGS__]
#define loc(...) ARGTYPE_LOCAL_LABEL, (__VA_ARGS__)
#define cnst(name) ARGTYPE_NAMED_CONSTANT, builder_state -> constants.name
#define err(name) ARGTYPE_NAMED_CONSTANT, builder_state -> constants.errors[name]
#define nloc(...) ARGTYPE_NUMERIC_LOCAL, (unsigned) (__VA_ARGS__)
#define dat(...) ARGTYPE_NUMERIC_DATA, (unsigned) (__VA_ARGS__)

// data.c
void generate_patch_data(void);
void define_file_names_data(void);
void check_for_unopened_files(void);
void define_patch_list(void);
void define_file_sizes(void);
void define_file_hashes(void);

// errorfn.c
void define_error_function(void);
void simple_error_message_function(char **, unsigned);
void descriptive_error_message_function(char **, unsigned);
int add_string_to_printed_error_messages(int **, char ***, unsigned *, char *);

// file.c
void write_file_patch(int, unsigned);
unsigned write_rainbow_file_patch(unsigned);
void write_file_patch_header_comment(const char *, unsigned);
unsigned write_individual_file_patch(unsigned, unsigned);
int get_file_length_and_hash(unsigned, Buffer);

// genoutfn.c
void define_generate_output_function(void);
void define_generate_reverse_output_function(void);

// global.c
extern void * builder_memory_region;
extern struct code_generation_state * builder_state;
extern jmp_buf builder_return_point;
extern char * builder_error;
extern const char * const code_error_messages[];
extern void (* const patch_engine_generator_functions[])(void);

// init.c
void initialize_builder_state(Options, CodeFile);
void initialize_code_generator(void);
int declare_register(const char *, unsigned char);
int declare_constant(const char *, unsigned);
void declare_filename_labels(void);
void declare_patch_engine_labels(void);

// main.c
void builder_throw(const char *, ...);
void add_version_header_comments(void);
void destroy_builder_state(void);

// mainfn.c
void define_main_function(void);

// patcheng.c
void define_apply_xor_like_patch_function(int);
void define_apply_xor_like_fragmented_patch_function(int);
void define_pad_to_length_function(void);

// patches.c
void write_file_patches(void);

// patchfn.c
void define_apply_patch_function(void);
void define_apply_reverse_patch_function(void);
void define_execute_patch_function(void);
void define_patch_engine_functions(void);

// pfrag.c
void define_apply_fragmented_patch_function(void);

// pfrperm.c
void define_handle_fragment_permutation_function(void);
void write_fragment_copy_code(int, int, const char *);

// pxorrle.c
void define_apply_xor_rle_patch_helper_function(void);

// runtime.c
void generate_runtime(void);
int get_defined_label(int *, const char *);
int generate_banner_lines(const char *, char ***);
void destroy_banner_lines(char **, unsigned);
void builder_declare_local(const char *);

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
