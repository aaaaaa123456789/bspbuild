#include "../codegen/pubtypes.h"
#include "../util/pubtypes.h"

#include "pubtypes.h"

struct fragment_permutation_table {
  unsigned source_fragment_count;
  unsigned target_fragment_count;
  int * source_to_target_fragments;
  int * target_to_source_fragments;
  // ...
};
