#ifndef ___IPS

#define ___IPS

#include "../util/pubtypes.h"

// main.c
Buffer generate_ips_patch(const void * first, const void * second, unsigned length);
Buffer generate_ips_for_data(const void * data, unsigned length);

#endif
