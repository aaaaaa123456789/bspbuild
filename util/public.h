#ifndef ___UTIL

#define ___UTIL

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

#endif
