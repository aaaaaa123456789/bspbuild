#include "../util/public.h"

#include "public.h"

#define MINIMUM_RUN 8
#define MAXIMUM_REDUNDANCY 6
#define MAXIMUM_BLOCK_SIZE 0x7FFF
// you cannot write a patch block with an offset of 4,542,278 (0x454F46) because it is the end of file marker
#define EOF_MARKER 4542278

// block.c
void write_ips_blocks_for_data(const void *, unsigned, unsigned);
unsigned write_next_ips_block(const char *, unsigned, unsigned);
void write_next_data_block(const char *, unsigned short, unsigned);
unsigned short write_next_run_block(const char *, unsigned, unsigned);
unsigned get_segment_length(const char *, const char *, unsigned, int);
int check_runs(const char *, unsigned);

// global.c
extern Buffer ips_buffer;
extern const char * ips_source;
extern const char * ips_target;
extern unsigned ips_data_length;

// main.c
void generate_ips_patch_from_diff(void);
void generate_ips_patch_for_data(void);
