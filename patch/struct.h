#include "../codegen/pubtypes.h"
#include "../util/pubtypes.h"

#include "pubtypes.h"

struct fragment_permutation_table {
  int * source_to_target_fragments;
  int * target_to_source_fragments;
  // ...
};
