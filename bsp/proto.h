#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <setjmp.h>

#include "../headers/const.h"
#include "../util/public.h"

#include "struct.h"
#include "commands.h"
#include "public.h"

// args.c
struct bsp_argument * get_argument(const char *);
char * get_string_argument(const char *);

// cmdtable.c
extern struct bsp_command script_commands[];

// fstack.c
void bsp_push_file(const char *);
void bsp_pop_file(void);
char * get_line_from_bsp_input(void);

// globals.c
extern char * current_file;
extern unsigned current_line;
extern struct file_stack_entry * file_stack;
extern unsigned file_stack_length;
extern struct script_data * script_data;
extern jmp_buf bsp_return_point;
extern char * volatile bsp_error;
extern void * bsp_memory_region;
extern FILE * bsp_temporary_file;

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

// symbol.c
struct bsp_symbol * new_symbol(const char *, unsigned);
struct bsp_symbol * find_symbol(const char *);
struct bsp_symbol * find_definition(const char *);
struct bsp_symbol * find_identifier_by_name(const char *, struct bsp_symbol **, unsigned);
void create_symbol(const char *, unsigned);
void create_definition(const char *, unsigned);
void create_reference(const char *, unsigned);
void resolve(const struct bsp_symbol *);
