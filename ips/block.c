#include "proto.h"

void write_ips_blocks_for_data (const void * data, unsigned length, unsigned offset) {
  if (!(data && length)) return;
  unsigned block_size;
  while (length) {
    if (offset == EOF_MARKER) {
      char * buf = malloc(length + 1);
      *buf = ips_target[EOF_MARKER - 1];
      memcpy(buf + 1, data, length);
      block_size = write_next_ips_block(buf, length + 1, offset - 1)
      free(buf);
    } else
      block_size = write_next_ips_block(data, length, offset);
    data = ((const char *) data) + block_size;
    length -= block_size;
    offset += block_size;
  }
}

unsigned write_next_ips_block (const char * data, unsigned length, unsigned offset) {
  unsigned remaining = length;
  unsigned short last_run;
  do {
    last_run = write_next_run_block(data, remaining, offset);
    data += last_run;
    remaining -= last_run;
    offset += last_run;
  } while (last_run >= MAXIMUM_BLOCK_SIZE);
  if (!remaining) return length;
  unsigned current = remaining;
  if (current > MAXIMUM_BLOCK_SIZE) {
    current = MAXIMUM_BLOCK_SIZE;
    remaining -= MAXIMUM_BLOCK_SIZE;
  } else
    remaining = 0;
  int run_pos = check_runs(data, current);
  if (run_pos > 0) {
    remaining += current - run_pos;
    current = run_pos;
    if ((offset + current) == EOF_MARKER) {
      remaining --;
      current ++;
    }
  }
  write_next_data_block(data, current, offset);
  return length - remaining;
}

void write_next_data_block (const char * data, unsigned short length, unsigned offset) {
  if (!length) return;
  append_big_endian_number_to_buffer(&ips_buffer, offset, 3);
  append_big_endian_number_to_buffer(&ips_buffer, length, 2);
  append_data_to_buffer(&ips_buffer, data, length);
}

unsigned short write_next_run_block (const char * data, unsigned length, unsigned offset) {
  unsigned short run;
  if (length > MAXIMUM_BLOCK_SIZE) length = MAXIMUM_BLOCK_SIZE;
  for (run = 1; (run < length) && (data[run] == *data); run ++);
  if (run < MINIMUM_RUN) return 0;
  append_big_endian_number_to_buffer(&ips_buffer, offset, 3);
  append_data_to_buffer(&ips_buffer, (unsigned char []) {0, 0}, 2); // just a zero, but this is faster
  append_big_endian_number_to_buffer(&ips_buffer, run, 2);
  append_data_to_buffer(&ips_buffer, data, 1);
  return run;
}

unsigned get_segment_length (const char * first_buffer, const char * second_buffer, unsigned length, int kind) {
  // 0: differing, 1: equal
  unsigned pos;
  for (pos = 0; (pos < length) && ((first_buffer[pos] == second_buffer[pos]) == kind); pos ++);
  return pos;
}

int check_runs (const char * data, unsigned length) {
  unsigned pos, cmp;
  if (length < MINIMUM_RUN) return -1;
  for (pos = 0; pos <= (length - MINIMUM_RUN); pos ++) {
    for (cmp = 0; (cmp < MINIMUM_RUN) && (data[pos] == data[pos + cmp]); cmp ++);
    if (cmp >= MINIMUM_RUN) return pos;
  }
  return -1;
}
