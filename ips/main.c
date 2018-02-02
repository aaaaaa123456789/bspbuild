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
  ips_source = ips_target = NULL;
  ips_buffer = NULL;
  return result;
}

Buffer generate_ips_patch_from_buffers (const Buffer source, const Buffer target) {
  if (source -> length > target -> length) return NULL;
  if (!(source -> length)) return generate_ips_for_data(target -> data, target -> length);
  Buffer result = generate_ips_patch(source -> data, target -> data, source -> length);
  if (source -> length == target -> length) return result;
  resize_buffer(&result, result -> length - 3); // remove the EOF
  ips_buffer = result;
  ips_target = target -> data;
  write_ips_blocks_for_data(target -> data + source -> length, target -> length - source -> length, source -> length);
  append_data_to_buffer(&ips_buffer, (unsigned char []) {0x45, 0x4F, 0x46}, 3); // "EOF"
  ips_target = NULL;
  result = ips_buffer;
  ips_buffer = NULL;
  return result;
}

Buffer generate_ips_for_data (const void * data, unsigned length) {
  return generate_ips_patch(NULL, data, length);
}

void generate_ips_patch_from_diff (unsigned length) {
  unsigned block, next, pos;
  for (pos = 0; pos < length; pos += block)
    if (ips_source[pos] == ips_target[pos])
      block = get_segment_length(ips_source + pos, ips_target + pos, length - pos, 1);
    else {
      block = next = 0;
      do {
        block += next + get_segment_length(ips_source + pos + block, ips_target + pos + block, length - pos - block, 0);
        next = get_segment_length(ips_source + pos + block, ips_target + pos + block, length - pos - block, 1);
      } while (((pos + block) < length) && (next < MAXIMUM_IPS_REDUNDANCY));
      write_ips_blocks_for_data(ips_target + pos, block, pos);
    }
}
