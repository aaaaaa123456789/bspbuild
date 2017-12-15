#include "pubtypes.h"

#ifndef NULL
  #define NULL ((void *) 0)
#endif

struct option_parser {
  const char * option;
  union {
    char * (* no_argument_callback) (Options, int);
    char * (* argument_callback) (Options, const char *, int);
  };
  unsigned argument_required: 1;
  signed parameter:          31;
};
