#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <setjmp.h>

#include "../util/public.h"

#include "struct.h"
#include "commands.h"
#include "public.h"

#define LETTERS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define LETTERS_OR_UNDERSCORE LETTERS "_"
#define DIGITS "0123456789"
#define VALID_ID_CHARACTERS LETTERS_OR_UNDERSCORE DIGITS
#define NUMERIC_CHARACTERS DIGITS "+-"

// args.c
struct bsp_argument * get_argument(const char *);
char * get_string_argument(const char *);

// cmdtable.c
extern struct bsp_command script_commands[];

// fstack.c
void push_file(const char *);
void pop_file(void);
char * get_line_from_input(void);

// globals.c
extern char * current_file;
extern unsigned current_line;
extern struct file_stack_entry * file_stack;
extern unsigned file_stack_length;
extern struct script_data * script_data;
extern jmp_buf bsp_return_point;
extern char * volatile bsp_error;
extern void * bsp_memory_region;

// labels.c
void declare_label(const char *);
void flush_locals(void);
void flush_all_symbols(void);
int validate_label(const char *);

// main.c
void bsp_parse_file(const char *);
void bsp_write_output(const char *);
void bsp_throw_error(const char *, ...);

// parse.c
void process_input_line(const char *);
char ** extract_components_from_line(const char *);
void destroy_component_array(char **);
unsigned count_parameters(char **);
int find_unquoted_character(const char *, char);
struct bsp_command * find_command(const char *);

// scrdata.c
void initialize_script_data(void);
void append_data_to_script(const char *, unsigned);
void append_binary_file_to_script(const char *);
void write_halfword_to_buffer(void *, unsigned short);
void write_word_to_buffer(void *, unsigned);
void write_number_to_buffer(unsigned char *, unsigned, unsigned char);

// symbol.c
struct bsp_symbol * new_symbol(const char *, unsigned);
struct bsp_symbol * find_symbol(const char *);
struct bsp_symbol * find_definition(const char *);
struct bsp_symbol * find_identifier_by_name(const char *, struct bsp_symbol **, unsigned);
void create_symbol(const char *, unsigned);
void create_definition(const char *, unsigned);
void create_reference(const char *, unsigned);
void resolve(const struct bsp_symbol *);
