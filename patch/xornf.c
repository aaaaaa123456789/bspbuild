#include "proto.h"

unsigned calculate_xor_like_unfragmented_data_length (Buffer source, Buffer target, const struct patching_flags * flags) {
  unsigned data_length;
  if (flags -> padding_length) {
    data_length = calculate_unpadded_data_length(target -> data, target -> length, flags);
    if (flags -> reversible_patch) {
      unsigned temp = calculate_unpadded_data_length(source -> data, source -> length, flags);
      if (temp > data_length) data_length = temp;
    }
  } else if (flags -> reversible_patch)
    data_length = (source -> length > target -> length) ? source -> length : target -> length;
  else
    data_length = target -> length;
  return data_length;
}

char * write_xor_like_unfragmented_patch_data (CodeFile codefile, Buffer source, Buffer target, unsigned data_length, const char * label,
                                               char * (* data_writer) (CodeFile, const unsigned char *, unsigned)) {
  if (add_local_label_to_codefile(codefile, label) < 0) return generate_string("could not declare local label '.%s'", label);
  unsigned char * data;
  if ((data_length <= source -> length) && (data_length <= target -> length))
    data = generate_xor_data_buffer(source -> data, target -> data, data_length);
  else {
    const void * extra;
    unsigned original_length;
    if (data_length > source -> length) {
      original_length = source -> length;
      extra = target -> data + original_length;
    } else {
      original_length = target -> length;
      extra = source -> data + original_length;
    }
    data = generate_xor_data_buffer(source -> data, target -> data, original_length);
    data = realloc(data, data_length);
    memcpy(data + original_length, extra, data_length - original_length);
  }
  char * result = data_writer(codefile, data, data_length);
  free(data);
  if (!result) add_blank_line_to_codefile(codefile);
  return result;
}
