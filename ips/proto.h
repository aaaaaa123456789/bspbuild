#include <stdlib.h>
#include <string.h>

#include "../headers/const.h"
#include "../util/public.h"

#include "public.h"

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

// main.c
void generate_ips_patch_from_diff(unsigned);
