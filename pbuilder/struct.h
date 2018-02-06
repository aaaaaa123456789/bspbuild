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
    int detect_input;
    int print_detected_input;
    int select_output;
    int error;
    int generate_output;
    int validate_output;
    // ...
  } labels;
  struct {
    int errors[NUM_CODE_ERRORS + 1];
    // ...
  } constants;
  // ...
};
