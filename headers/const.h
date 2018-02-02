#ifndef ___CONSTANTS

#define ___CONSTANTS

// ...
#define NUM_PATCHING_METHODS 3 // FIXME: update with real value after patching methods have been defined

#define MAX_INPUT_FILES 0xff0000

#define MAX_IPS_BLOCK_SIZE 0x1000000

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
