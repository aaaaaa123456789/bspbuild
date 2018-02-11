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
    int select_output; // TODO
    int error;
    int generate_output; // TODO
    int validate_output; // TODO
    int get_nth_string;
    int file_sizes; // TODO
    int file_hashes; // TODO
    int file_names; // TODO
    // ...
  } labels;
  struct {
    int errors[NUM_CODE_ERRORS + 1];
    int num_input_files;
    int hash_size;
    // ...
  } constants;
  struct {
    unsigned get_nth_string:       1;
    unsigned print_detected_input: 1;
    unsigned validate_output:      1;
    unsigned file_names:           1; // not strictly a function, but...
  } needed_functions;
  // ...
};
