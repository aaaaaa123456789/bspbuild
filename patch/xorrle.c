#include "proto.h"

/*
   Format is the same as for plain XOR patches
   RLE data format:
   1 byte command:
   bits 7-6: command (0: data, 1: repeat byte, 2: repeat halfword, 3: repeat word)
   bit 5: offset/long toggle (for data: 0: data, 1: repeat doubleword; for others: 0: repeat, 1: repeat and increment)
   bits 4-0: length (0: additional byte follows indicating length + 32)
   length for repeat commands is in number of values, not bytes; for repeat commands, length = 1, 2 indicates a byte follows for length + 0x120, 0x220
   for repeat and increment commands, the initial value follows, and then then increment
   valid length ranges: data: 1 .. 287, repeat: 3 .. 799
*/

char * write_xor_rle_patch_data (CodeFile codefile, Buffer source, Buffer target, const struct patching_flags * flags) {
  return write_xor_like_patch(codefile, source, target, flags, &write_xor_rle_patch_buffer_data);
}

char * write_xor_rle_patch_buffer_data (CodeFile codefile, const unsigned char * data, unsigned length) {
  void * buffer = malloc(16 + (length << 1)); // big enough to stay out of trouble
  unsigned buffer_length = generate_rle_data(data, length, buffer);
  add_data_to_codefile(codefile, buffer, buffer_length);
  free(buffer);
  return NULL;
}

unsigned generate_rle_data (const unsigned char * data, unsigned length, unsigned char * result) {
  unsigned result_length = 0;
  struct rle_run_data run;
  unsigned current;
  while (length) {
    run = find_next_rle_run(data, length);
    if (run.distance) {
      current = write_rle_data_to_buffer(result, data, length);
      result_length += current;
      result += current;
      data += run.distance;
      length -= run.distance;
    }
    if (run.length) {
      current = write_rle_run_to_buffer(result, run);
      result_length += current;
      result += current;
      data += run.length * run.data_length;
      length -= run.length * run.data_length;
    }
  }
  return result_length;
}

struct rle_run_data find_next_rle_run (const unsigned char * data, unsigned length) {
  struct rle_run_data run = {.distance = 0, .length = 0, .data_length = 0};
  while ((run.distance + 3) <= length) {
    find_rle_byte_run(&run, data + run.distance, length - run.distance);
    find_rle_multibyte_run(&run, data + run.distance, length - run.distance, 2, 1);
    find_rle_multibyte_run(&run, data + run.distance, length - run.distance, 4, 1);
    find_rle_multibyte_run(&run, data + run.distance, length - run.distance, 8, 0);
    if (run.length) return run;
    run.distance ++;
  }
  return (struct rle_run_data) {.distance = length, .length = 0, .data_length = 0};
}

void find_rle_byte_run (struct rle_run_data * run, const unsigned char * data, unsigned length) {
  if (length < 3) return;
  unsigned char offset = (data[1] - *data) & 0xff;
  if (((data[2] - data[1]) & 0xff) != offset) return;
  struct rle_run_data current_run = {.distance = run -> distance, .length = 3, .data_length = 1, .value = *data, .offset = offset};
  while (current_run.length < length) {
    if (((data[current_run.length] - data[current_run.length - 1]) & 0xff) != offset) break;
    current_run.length ++;
  }
  if (current_run.length > run -> length) *run = current_run;
}

void find_rle_multibyte_run (struct rle_run_data * run, const unsigned char * data, unsigned length, unsigned char byte_length, unsigned char allow_offsets) {
  length /= byte_length;
  if (length < 3) return;
  struct rle_run_data current_run = {.distance = run -> distance, .length = 3, .data_length = byte_length, .value = read_number_from_buffer(data, byte_length)};
  unsigned long long current = read_number_from_buffer(data + byte_length, byte_length);
  if ((current != current_run.value) && !allow_offsets) return;
  unsigned long long mask = ((1 << (byte_length << 2)) << (byte_length << 2)) - 1; // 1 << 64 is undefined, so splitting the shift into two side-steps the issue
  current_run.offset = (current - current_run.value) & mask;
  unsigned long long next = read_number_from_buffer(data + (byte_length << 1), byte_length);
  if (((next - current) & mask) != (unsigned long long) current_run.offset) return;
  while (current_run.length < length) {
    current = next;
    next = read_number_from_buffer(data + current_run.length * byte_length, byte_length);
    if (((next - current) & mask) != (unsigned long long) current_run.offset) break;
    current_run.length ++;
  }
  if ((current_run.length * byte_length) > run -> length) *run = current_run;
}

unsigned write_rle_data_to_buffer (unsigned char * buffer, const unsigned char * data, unsigned length) {
  unsigned short current_block;
  unsigned char * wp = buffer;
  while (length) {
    current_block = (length > MAX_RLE_DATA_BLOCK) ? MAX_RLE_DATA_BLOCK : length;
    if (current_block > 31) {
      *(wp ++) = 0;
      *(wp ++) = current_block - 32;
    } else
      *(wp ++) = current_block;
    memcpy(wp, data, current_block);
    wp += current_block;
    data += current_block;
    length -= current_block;
  }
  return wp - buffer;
}

unsigned write_rle_run_to_buffer (unsigned char * buffer, struct rle_run_data run) {
  // the function expects run.data_length and run.offset to be valid (run.data_length in {1, 2, 4, 8}, and run.offset = 0 if run.data_length = 8)
  unsigned short current_block;
  unsigned char * wp = buffer;
  unsigned char type;
  if (run.data_length == 8)
    type = 0x20;
  else if (run.data_length == 4)
    type = 0xc0;
  else
    type = run.data_length << 6;
  if (run.offset)
    type |= 0x20;
  while (run.length) {
    current_block = (run.length > MAX_RLE_RUN_BLOCK) ? MAX_RLE_RUN_BLOCK : run.length;
    if (current_block < 3) break;
    if ((run.length != current_block) && ((run.length - current_block) < 3)) current_block = run.length - 3;
    if (current_block > 31) {
      *(wp ++) = type + ((current_block - 32) >> 8);
      *(wp ++) = (current_block - 32) & 0xff;
    } else
      *(wp ++) = type + current_block;
    write_number_to_buffer(wp, run.value, run.data_length);
    if (run.offset) {
      write_number_to_buffer(wp + run.data_length, run.offset, run.data_length);
      run.value += run.offset * current_block;
      wp += run.data_length << 1;
    } else
      wp += run.data_length;
    run.length -= current_block;
  }
  return wp - buffer;
}
