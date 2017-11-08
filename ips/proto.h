#include "public.h"

#define MINIMUM_RUN 8
#define MAXIMUM_REDUNDANCY 6
// you cannot write a patch block with an offset of 4,542,278 (0x454F46) because it is the end of file marker
#define EOF_MARKER 4542278

// block.c
unsigned get_segment_length(const char *, const char *, unsigned, int);
int check_runs(const char *, unsigned);
