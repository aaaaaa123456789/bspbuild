#include "../codegen/pubtypes.h"
#include "../optparse/pubtypes.h"
#include "../headers/const.h"

struct code_generation_state {
  Options options;
  CodeFile codefile;
  int * file_name_labels; // outside of the labels struct so it doesn't get initialized to -1
  struct builder_file_data {
    unsigned size;
    unsigned char hash[20];
    unsigned data_label;
    int reference_file;
  } * file_data;
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
    int generate_reverse_output;
    int apply_patch;
    int apply_reverse_patch;
    int validate_output;
    int get_nth_string;
    int file_sizes;
    int file_hashes;
    int file_names;
    int patch_list;
    int execute_patch;
    // patch engine auxiliary functions
    int apply_xor_rle_patch;
    int pad_to_length;
    int apply_fragmented_patch;
    int handle_fragment_permutation;
  } labels;
  struct {
    int errors[NUM_CODE_ERRORS + 1];
    int num_input_files;
    int first_output_file;
    int hash_size;
    int padding_value;
    int fragment_size;
  } constants;
  struct {
    unsigned get_nth_string:              1;
    unsigned print_detected_input:        1;
    unsigned generate_reverse_output:     1;
    unsigned apply_reverse_patch:         1;
    unsigned validate_output:             1;
    unsigned file_names:                  1; // not strictly a function, but...
    unsigned full_file_sizes:             1; // include pure targets' file sizes
    unsigned full_file_hashes:            1; // include pure targets' file hashes
    unsigned full_patch_list:             1; // patch list starts at file #1, instead of the first output file
    // patch engine auxiliary functions
    unsigned apply_xor_rle_patch:         1;
    unsigned pad_to_length:               1;
    unsigned apply_fragmented_patch:      1;
    unsigned handle_fragment_permutation: 1;
  } needed_functions;
  struct {
    int labels[NUM_PATCHING_METHODS];
    signed char methods[NUM_PATCHING_METHODS];
    signed char IDs[NUM_PATCHING_METHODS];
    unsigned char count;
  } patch_engines;
};
