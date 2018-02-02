#ifndef ___IPS

#define ___IPS

#include "../util/pubtypes.h"

// main.c
Buffer generate_ips_patch(const void * first, const void * second, unsigned length);
Buffer generate_ips_patch_from_buffers(const Buffer source, const Buffer target);
Buffer generate_ips_for_data(const void * data, unsigned length);

#endif
