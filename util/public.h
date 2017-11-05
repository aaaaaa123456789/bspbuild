#ifndef ___UTIL

#define ___UTIL

#include <stdarg.h>

// file.c
FILE * open_text_file(const char * file, char ** error);
FILE * open_binary_file(const char * file, char ** error);
FILE * open_binary_file_for_writing(const char * file, char ** error);
char * read_line(FILE * fp);
int write_data_to_file(FILE * fp, const void * data, unsigned length);

// string.c
char * trim_string(char * string);
char * duplicate_string(const char * string);
unsigned convert_string_to_number(const char * string, char ** error);
char * generate_string(const char * fmt, ...);
char * generate_string_from_varargs(const char * fmt, va_list varargs);

#endif
