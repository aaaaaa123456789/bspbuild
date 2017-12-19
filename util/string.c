#include "proto.h"

char * trim_string (char * string) {
  while ((*string == ' ') || (*string == '\t')) string ++;
  unsigned effective_length = strlen(string);
  while (effective_length && ((string[effective_length - 1] == ' ') || (string[effective_length - 1] == '\t'))) effective_length --;
  char * result = malloc(effective_length + 1);
  memcpy(result, string, effective_length);
  result[effective_length] = 0;
  return result;
}

char * duplicate_string (const char * string) {
  return strcpy(malloc(strlen(string) + 1), string);
}

unsigned convert_digit_string_to_number (const char * string, char ** error) {
  if (strspn(string, "0123456789") != strlen(string)) {
    *error = generate_string("invalid number: %s", string);
    return 0;
  }
  while ((*string == '0') && string[1]) string ++;
  return convert_string_to_number(string, error);
}

unsigned convert_string_to_number (const char * string, char ** error) {
  if (!*string) {
    *error = duplicate_string("argument is empty");
    return 0;
  }
  *error = NULL;
  char * error_pointer;
  unsigned long long value = strtoull(string, &error_pointer, 0);
  if (*error_pointer) *error = generate_string("invalid number: %s", string);
  return value;
}

unsigned char get_hex_digit (char digit) {
  if ((digit >= '0') && (digit <= '9')) return digit - '0';
  if ((digit >= 'A') && (digit <= 'F')) return digit - ('A' - 10);
  if ((digit >= 'a') && (digit <= 'f')) return digit - ('a' - 10);
  return 0;
}

char * generate_string (const char * fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  char * result = generate_string_from_varargs(fmt, ap);
  va_end(ap);
  return result;
}

char * generate_string_from_varargs (const char * fmt, va_list varargs) {
  char testbuf[2];
  va_list temp;
  va_copy(temp, varargs);
  int rv = vsnprintf(testbuf, 1, fmt, temp);
  va_end(temp);
  if (rv < 0) return NULL;
  char * result = malloc(rv + 1);
  vsnprintf(result, rv + 1, fmt, varargs);
  return result;
}

int validate_UTF8 (const unsigned char * string) {
  unsigned char check;
  for (; *string; string ++) {
    if (*string < 0x80) continue;
    if (
      (*string < 0xc2) ||
      (*string > 0xf4) ||
      ((*string == 0xe0) && (string[1] < 0xa0)) ||
      ((*string == 0xed) && (string[1] >= 0xa0)) ||
      ((*string == 0xf0) && (string[1] < 0x90)) ||
      ((*string == 0xf4) && (string[1] >= 0x90))
    ) return 0;
    if (*string < 0xe0)
      check = 1;
    else if (*string < 0xf0)
      check = 2;
    else
      check = 3;
    while (check) {
      if ((*(++ string) & 0xc0) != 0x80) return 0;
      check --;
    }
  }
  return 1;
}

void string_to_uppercase (char * string) {
  for (; *string; string ++) *string = toupper(*string);
}

void string_to_lowercase (char * string) {
  for (; *string; string ++) *string = tolower(*string);
}
