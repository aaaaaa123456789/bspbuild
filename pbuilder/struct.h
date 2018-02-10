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
    int error;
    int generate_output; // TODO
    int validate_output; // TODO
    int get_nth_string;
    // ...
  } labels;
  struct {
    int errors[NUM_CODE_ERRORS + 1];
    // ...
  } constants;
  struct {
    unsigned get_nth_string:       1;
    unsigned print_detected_input: 1;
    unsigned validate_output:      1;
  } needed_functions;
  // ...
};
