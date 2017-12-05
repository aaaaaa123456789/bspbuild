#include "proto.h"

unsigned char * calculate_sha1 (const void * data, unsigned length) {
  uint32_t state[5] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0};
  const char * current;
  unsigned remaining;
  for (current = data, remaining = length; remaining >= 64; current += 64, remaining -= 64) sha1_process_block(current, state);
  // technically only {0} is necessary, but better safe than sorry
  unsigned char last_block[64] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  memcpy(last_block, current, remaining);
  last_block[remaining] = 0x80;
  if (remaining >= 56) {
    sha1_process_block(last_block, state);
    memset(last_block, 0, 64);
  }
  unsigned long long bit_length = ((unsigned long long) length) << 3;
  for (remaining = 5; remaining; remaining --) {
    last_block[58 + remaining] = bit_length;
    bit_length >>= 8;
  }
  sha1_process_block(last_block, state);
  unsigned char * result = malloc(20);
  for (remaining = 0; remaining < 20; remaining ++) result[remaining] = state[remaining >> 2] >> ((~remaining & 3) << 3);
  return result;
}

void sha1_process_block (const unsigned char * block, uint32_t * state) {
  // ...
}
