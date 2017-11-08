#ifndef ___UTIL_PUBTYPES

#define ___UTIL_PUBTYPES

typedef struct buffer {
  unsigned length;
  unsigned char data[];
} * Buffer;

#endif
