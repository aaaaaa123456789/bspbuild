#include "../codegen/pubtypes.h"
#include "../optparse/pubtypes.h"

struct code_generation_state {
  Options options;
  CodeFile codefile;
  struct {
    unsigned char result;
    unsigned char temp;
    unsigned char argument;
    unsigned char error;
    unsigned char stackframe;
  } registers;
  // ...
};
