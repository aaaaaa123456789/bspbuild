#include "proto.h"

Buffer generate_ips_from_data (const void * first, const void * second, unsigned length) {
  if (length > 0x1000000) return NULL;
  ips_buffer = create_buffer();
  ips_source = first;
  ips_target = second;
  ips_data_length = length;
  append_data_to_buffer(&ips_buffer, (unsigned char []) {0x50, 0x41, 0x54, 0x43, 0x48}, 5); // "PATCH"
  (second ? &generate_ips_patch_from_diff : &generate_ips_patch_for_data)();
  append_data_to_buffer(&ips_buffer, (unsigned char []) {0x45, 0x4F, 0x46}, 3); // "EOF"
  Buffer result = ips_buffer;
  ips_source = ips_target = ips_buffer = NULL;
  ips_data_length = 0;
  return result;
}

void generate_ips_patch_from_diff (void) {
  // ...
}

void generate_ips_patch_for_data (void) {
  // ...
}
