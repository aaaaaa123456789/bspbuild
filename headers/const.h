#ifndef ___CONSTANTS

#define ___CONSTANTS

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
