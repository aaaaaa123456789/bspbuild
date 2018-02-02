#ifndef ___CONSTANTS

#define ___CONSTANTS

#define LETTERS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define LETTERS_OR_UNDERSCORE LETTERS "_"
#define DIGITS "0123456789"
#define VALID_ID_CHARACTERS LETTERS_OR_UNDERSCORE DIGITS
#define NUMERIC_CHARACTERS DIGITS "+-"

// ...
#define NUM_PATCHING_METHODS 3 // FIXME: update with real value after patching methods have been defined

#define MAX_INPUT_FILES 0xff0000

// configurable parameters
#define MINIMUM_IPS_RUN 8
#define MAXIMUM_IPS_REDUNDANCY 6
#define MAXIMUM_IPS_BLOCK_SIZE 0x7FFF

#define IPS_EOF_MARKER 0x454F46 /* 4,542,278 (0x454F46) is the end of file marker, and thus not a valid block offset */
#define MAX_IPS_PATCH_SIZE 0x1000000 /* maximum size for the input files to IPS */

enum {
  OPERATION_MODE_NORMAL     = 0,
  OPERATION_MODE_BSP_INPUT  = 1,
  OPERATION_MODE_IPS_OUTPUT = 2
};

enum {
  DIRECTION_SOURCE        = 0,
  DIRECTION_SOURCE_TARGET = 1,
  DIRECTION_TARGET        = 2
};

enum {
  TARGET_TYPE_PREVIOUS    = 0xffffff,
  TARGET_TYPE_FROM_SOURCE = 0xfffffe
};

enum {
  VALIDATE_ALL    = 0,
  VALIDATE_LENGTH = 1,
  VALIDATE_NONE   = 2
};

#endif
