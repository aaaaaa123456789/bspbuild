#ifndef ___CONSTANTS

#define ___CONSTANTS

#define LETTERS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define LETTERS_OR_UNDERSCORE LETTERS "_"
#define DIGITS "0123456789"
#define VALID_ID_CHARACTERS LETTERS_OR_UNDERSCORE DIGITS
#define NUMERIC_CHARACTERS DIGITS "+-"

#define NUM_PATCHING_METHODS 3 // FIXME: update with real value after patching methods have been defined
#define NUM_CODE_ERRORS 10 // FIXME: update with real value after error constants have been defined

#define MAX_INPUT_FILES 0xFF0000
#define MAX_PATCHING_METHODS 64 // maximum allowable due to the number of bits allocated to this value

#define IPS_EOF_MARKER 0x454F46 /* 4,542,278 (0x454F46) is the end of file marker, and thus not a valid block offset */
#define MAX_IPS_PATCH_SIZE 0x1000000 /* maximum size for the input files to IPS */

// configurable parameters
#define MINIMUM_IPS_RUN 8
#define MAXIMUM_IPS_REDUNDANCY 6
#define MAXIMUM_IPS_BLOCK_SIZE 0x7FFF

#define MINIMUM_FRAGMENT_SIZE 64

#endif
