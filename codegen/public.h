#ifndef ___CODEGEN

#define ___CODEGEN

#include "pubtypes.h"

// data.c
void add_data_to_codefile(CodeFile file, const void * data, unsigned length);
int add_string_to_codefile(CodeFile file, const char * string);

// file.c
void add_blank_line_to_codefile(CodeFile file);
void add_comment_to_codefile(CodeFile file, const char * comment, int indented);

// inst.c
char * add_instruction_to_codefile(CodeFile file, unsigned instruction, ...);
char * add_instruction_with_arguments_to_codefile(CodeFile file, unsigned instruction, unsigned argument_count, const struct instruction_argument * arguments);

// labels.c
int declare_label_for_codefile(CodeFile file, const char * label);
int add_label_to_codefile(CodeFile file, const char * label);
int add_declared_label_to_codefile(CodeFile file, unsigned label_number);
int add_local_label_to_codefile(CodeFile file, const char * label);
int declare_numeric_local_for_codefile(CodeFile file);
int add_numeric_local_to_codefile(CodeFile file);
int add_declared_numeric_local_to_codefile(CodeFile file, unsigned numeric_local);
int add_numeric_data_label_to_codefile(CodeFile file);

// namedobj.c
int add_register_definition_to_codefile(CodeFile file, const char * register_name, unsigned char register_number);
int add_constant_to_codefile(CodeFile file, const char * constant_name, unsigned constant_value);

#endif
