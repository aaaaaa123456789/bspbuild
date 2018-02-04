#include "pubtypes.h"

struct option_parser {
  const char * option;
  union {
    char * (* no_argument_callback) (Options, int);
    char * (* argument_callback) (Options, const char *, int);
  };
  unsigned has_argument: 1;
  signed parameter:     31;
};
