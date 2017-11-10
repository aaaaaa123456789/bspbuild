#include "proto.h"

Buffer generate_ips_patch (const void * source, const void * target, unsigned length) {
  if (length > 0x1000000) return NULL;
  ips_buffer = create_buffer();
  ips_source = source;
  ips_target = target;
  append_data_to_buffer(&ips_buffer, (unsigned char []) {0x50, 0x41, 0x54, 0x43, 0x48}, 5); // "PATCH"
  if (source)
    generate_ips_patch_from_diff(length);
  else
    write_ips_blocks_for_data(target, length, 0);
  append_data_to_buffer(&ips_buffer, (unsigned char []) {0x45, 0x4F, 0x46}, 3); // "EOF"
  Buffer result = ips_buffer;
  ips_source = ips_target = ips_buffer = NULL;
  return result;
}

Buffer generate_ips_for_data (const void * data, unsigned length) {
  return generate_ips_patch(NULL, data, length);
}

void generate_ips_patch_from_diff (unsigned length) {
  // ...
}
