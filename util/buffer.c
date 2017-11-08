#include "proto.h"

void write_halfword_to_buffer (void * buffer, unsigned short number) {
  write_number_to_buffer(buffer, number, 2);
}

void write_word_to_buffer (void * buffer, unsigned number) {
  write_number_to_buffer(buffer, number, 4);
}

void write_number_to_buffer (void * buffer, uintmax_t number, unsigned char length) {
  unsigned char pos;
  unsigned char * bytebuf = buffer;
  for (pos = 0; pos < length; pos ++) {
    *(bytebuf ++) = number;
    number >>= 8;
  }
}
