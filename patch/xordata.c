#include "proto.h"

void * generate_xor_data_buffer (const unsigned char * source, const unsigned char * target, unsigned length) {
  uint_fast8_t * xor_data = malloc(length);
  unsigned count = length / sizeof(uint_fast8_t);
  const uint_fast8_t * fast_source = (const void *) source;
  const uint_fast8_t * fast_target = (const void *) target;
  while (count --) *(xor_data ++) = *(fast_source ++) ^ *(fast_target ++);
  count = length % sizeof(uint_fast8_t);
  if (count) {
    source = (const void *) fast_source;
    target = (const void *) fast_target;
    unsigned char * slow_xor_data = (void *) xor_data;
    while (count --) *(slow_xor_data ++) = *(source ++) ^ *(target ++);
  }
  return xor_data;
}

unsigned calculate_unpadded_data_length (const unsigned char * data, unsigned length, const struct patching_flags * flags) {
  unsigned char padding_value[4] = {0};
  switch (flags -> padding_length) {
    case 1:
      *padding_value = flags -> padding_value;
    case 0:
      while (length) {
        if (data[length - 1] != *padding_value) break;
        length --;
      }
      break;
    case 2:
      write_number_to_buffer(padding_value, flags -> padding_value, 2);
      while (length > 1) {
        if (data[length - 2] != *padding_value) break;
        if (data[length - 1] != padding_value[1]) break;
        length -= 2;
      }
      break;
    case 3:
      write_number_to_buffer(padding_value, flags -> padding_value, 4);
      while (length > 3) {
        if (memcmp(data + (length - 4), padding_value, 4)) break;
        length -= 4;
      }
  }
  return length;
}
