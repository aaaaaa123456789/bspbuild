#include "proto.h"

char * write_xor_like_patch (CodeFile codefile, Buffer source, Buffer target, const struct patching_flags * flags,
                             char * (* data_writer) (CodeFile, const unsigned char *, unsigned)) {
  if (flags -> fragmentation_enabled) return write_xor_like_fragmented_patch_data(codefile, source, target, flags, data_writer);
  unsigned data_length = calculate_xor_like_unfragmented_data_length(source, target, flags);
  char * result = write_xor_like_unfragmented_header(codefile, source, target, flags, "data", data_length);
  if (result) return result;
  return write_xor_like_unfragmented_patch_data(codefile, source, target, data_length, "data", data_writer);
}

void * generate_xor_data_buffer (const unsigned char * source, const unsigned char * target, unsigned length) {
  void * result = malloc(length);
  uint_fast8_t * xor_data = result;
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
  return result;
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
