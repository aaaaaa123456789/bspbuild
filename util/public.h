#ifndef ___UTIL

#define ___UTIL

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#include "pubtypes.h"

// buffer.c
Buffer create_buffer(void);
Buffer create_buffer_from_size(unsigned size);
void append_data_to_buffer(Buffer * buffer, const void * data, unsigned length);
void resize_buffer(Buffer * buffer, unsigned new_size);
void append_big_endian_number_to_buffer(Buffer * buffer, uintmax_t number, unsigned char length);
void write_halfword_to_buffer(void * buffer, unsigned short number);
void write_word_to_buffer(void * buffer, unsigned number);
void write_number_to_buffer(void * buffer, uintmax_t number, unsigned char length);

// file.c
FILE * open_text_file(const char * file, char ** error);
FILE * open_binary_file(const char * file, char ** error);
FILE * open_binary_file_for_writing(const char * file, char ** error);
char * read_line(FILE * fp);
char ** read_file_as_lines(FILE * fp, unsigned * line_count);
int write_data_to_file(FILE * fp, const void * data, unsigned length);
long get_file_length(FILE * fp);

// generic.c
unsigned value_in_list(unsigned value, ...);
void destroy_string_array(char ** array, unsigned count);
unsigned check_nulls(const void * buffer, unsigned length);
void sort_number_array(unsigned * array, unsigned size);

// mem.c
void * create_memory_region(void);
void destroy_memory_region(void * region);
void * mr_malloc(void * region, unsigned size);
void * mr_calloc(void * region, unsigned size);
void * mr_realloc(void * region, void * buffer, unsigned new_size);
void mr_free(void * region, void * buffer);

// sha1.c
unsigned char * calculate_sha1(const void * data, unsigned length);

// string.c
char * trim_string(char * string);
char * duplicate_string(const char * string);
char * mr_duplicate_string(void * region, const char * string);
unsigned convert_digit_string_to_number(const char * string, char ** error);
unsigned convert_string_to_number(const char * string, char ** error);
unsigned char get_hex_digit(char digit);
char * generate_string(const char * fmt, ...);
char * generate_string_from_varargs(const char * fmt, va_list varargs);
int validate_UTF8(const unsigned char * string);
void string_to_uppercase(char * string);
void string_to_lowercase(char * string);

#endif
