#include "proto.h"

Options new_options_object (void) {
  void * mr = create_memory_region();
  Options result = mr_calloc(mr, sizeof(struct options));
  result -> memory_region = mr;
  result -> current_conversion_direction = 1;
  result -> current_conversion_method = 0; // optimization hint; not otherwise needed
  result -> current_conversion_reference = 0xffffff;
  return result;
}

void destroy_options_object (Options options) {
  destroy_memory_region(options -> memory_region);
}
