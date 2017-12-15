#ifndef ___OPTPARSE_PUBTYPES

#define ___OPTPARSE_PUBTYPES

typedef struct options {
  void * memory_region;
  char * error_text;
  struct option_file {
    char * name;
    char * label;
    unsigned direction:  2; // 0: source, 1: source + target, 2: target
    unsigned method:     6;
    unsigned reference: 24;
  } * input_files;
  struct {
    char * opening;
    char * success;
    char * error;
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
  unsigned current_conversion_direction:   2; // initialize to 1
  unsigned current_conversion_method:      6;
  unsigned current_conversion_reference:  24; // initialize to 0xffffff
  unsigned input_bsp_source:               1;
  unsigned detect_fragment_permutation:    1;
  unsigned disable_output_validations:     2; // 0: none, 1: sha1, 2: sha1 + length
  unsigned fragmentation_parameters_given: 1; // validation flag
} * Options;

#endif
