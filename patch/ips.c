#include "proto.h"

/*
   4 bytes: initial file pointer
   4 bytes: pointer to IPS data
   repeat until ifp = -1, then 4 bytes: length
*/

char * write_ips_patch_data (CodeFile codefile, Buffer source, Buffer target, const struct patching_flags * flags) {
  if (!(flags -> reversible_patch)) return write_ips_patch_data_for_buffers(codefile, source, target);
  char * result = write_patch_data_to_codefile(codefile, 0, NULL, "reverse");
  if (result) return result;
  add_blank_line_to_codefile(codefile);
  if (result = write_ips_patch_data_for_buffers(codefile, source, target)) return result;
  add_blank_line_to_codefile(codefile);
  if (add_local_label_to_codefile(codefile, "reverse") < 0) return duplicate_string("could not declare local label '.reverse'");
  return write_ips_patch_data_for_buffers(codefile, target, source);
}

char * write_ips_patch_data_for_buffers (CodeFile codefile, Buffer source, Buffer target) {
  Buffer patch_block;
  char * result;
  if ((source -> length <= MAX_IPS_PATCH_SIZE) && (target -> length <= MAX_IPS_PATCH_SIZE)) {
    if (result = write_patch_data_to_codefile(codefile, 1, (unsigned []) {0}, NULL)) return result;
    if (source -> length > target -> length)
      patch_block = generate_ips_patch(source -> data, target -> data, target -> length);
    else
      patch_block = generate_ips_patch_from_buffers(source, target);
    add_data_to_codefile(codefile, patch_block -> data, patch_block -> length);
    free(patch_block);
  } else {
    const char * source_cursor = source -> data;
    const char * target_cursor = target -> data;
    unsigned remaining_scan = (source -> length > target -> length) ? target -> length : source -> length;
    unsigned current_block, position = 0;
    while (remaining_scan) {
      for (current_block = 0; (current_block < remaining_scan) && (source_cursor[current_block] == target_cursor[current_block]); current_block ++);
      source_cursor += current_block;
      target_cursor += current_block;
      remaining_scan -= current_block;
      position += current_block;
      if (!remaining_scan) break;
      current_block = (remaining_scan > MAX_IPS_PATCH_SIZE) ? MAX_IPS_PATCH_SIZE : remaining_scan;
      if (result = write_patch_data_to_codefile(codefile, 1, &position, NULL)) return result;
      patch_block = generate_ips_patch(source_cursor, target_cursor, current_block);
      add_data_to_codefile(codefile, patch_block -> data, patch_block -> length);
      free(patch_block);
      source_cursor += current_block;
      target_cursor += current_block;
      position += current_block;
      remaining_scan -= current_block;
    }
    if (source -> length < target -> length) {
      position = source -> length;
      remaining_scan = target -> length - source -> length;
      while (remaining_scan) {
        current_block = (remaining_scan > MAX_IPS_PATCH_SIZE) ? MAX_IPS_PATCH_SIZE : remaining_scan;
        if (result = write_patch_data_to_codefile(codefile, 1, &position, NULL)) return result;
        patch_block = generate_ips_for_data(target -> data + position, current_block);
        add_data_to_codefile(codefile, patch_block -> data, patch_block -> length);
        free(patch_block);
        remaining_scan -= current_block;
        position += current_block;
      }
    }
  }
  return write_patch_data_to_codefile(codefile, 2, (unsigned []) {-1, target -> length}, NULL);
}
