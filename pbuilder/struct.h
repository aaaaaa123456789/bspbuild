#include "../codegen/pubtypes.h"
#include "../optparse/pubtypes.h"

struct code_generation_state {
  Options options;
  CodeFile codefile;
  struct {
    unsigned char file;
    unsigned char result;
    unsigned char temp;
    unsigned char argument;
  } registers;
  struct {
    // ...
  } functions;
  // ...
};
