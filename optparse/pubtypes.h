#ifndef ___OPTPARSE_PUBTYPES

#define ___OPTPARSE_PUBTYPES

typedef struct options {
  void * memory_region;
  char * error_text;
  char * response;
  struct option_file {
    char * name;
    char * label;
    unsigned reference: 24;
    unsigned method:     6;
    unsigned direction:  2; // 0: source, 1: source + target, 2: target
  } * input_files;
  struct {
    char * opening;
    char * success;
    char * error;
    char * error_replacement;
  } messages;
  struct {
    char * compiled;
    char * source;
  } output_files;
  struct {
    char * global;
    char * label;
    char * variable;
    char * constant;
  } prefixes;
  char * label_file;
  unsigned input_file_count;
  unsigned fragment_size;
  unsigned padding_value;
  unsigned initial_register_number:                  8;
  unsigned current_conversion_reference:            24; // initialize to 0xffffff
  unsigned current_conversion_method:                6;
  unsigned current_conversion_direction:             2; // initialize to 1
  unsigned operation_mode:                           2; // 0: normal, 1: BSP input, 2: IPS output
  unsigned disable_output_validations:               2; // 0: none, 1: sha1, 2: sha1 + length
  unsigned sort_targets_alphabetically:              1;
  unsigned output_selection_on_single_option:        1;
  unsigned suppress_error_messages:                  1;
  unsigned no_source_patches:                        1;
  unsigned targets_per_page:                         4; // in output menu, 2-15, 0 = disabled
  unsigned padding_size:                             2; // 3 means 4, 0 means not given
  unsigned detect_fragment_permutation:              1;
  unsigned fragmentation_parameters_given:           1; // validation flag
  unsigned direction_options_given:                  1; // validation flag
  unsigned patch_method_options_given:               1; // validation flag
  unsigned initial_register_number_given:            1; // validation flag
  unsigned opening_message_from_file:                1;
  unsigned success_message_from_file:                1;
  unsigned error_message_from_file:                  1;
} * Options;

#endif
