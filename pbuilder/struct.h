#include "../codegen/pubtypes.h"
#include "../optparse/pubtypes.h"
#include "../headers/const.h"

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
    int main;
    int detect_input; // TODO
    int print_detected_input; // TODO
    int select_output; // TODO
    int error; // TODO
    int generate_output; // TODO
    int validate_output; // TODO
    // ...
  } labels;
  struct {
    int errors[NUM_CODE_ERRORS + 1];
    // ...
  } constants;
  // ...
};
