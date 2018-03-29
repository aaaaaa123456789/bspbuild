#include "../codegen/pubtypes.h"
#include "../util/pubtypes.h"

#include "pubtypes.h"

struct fragment_permutation_table {
  unsigned source_fragments;
  unsigned target_fragments;
  int * source_to_target_fragments; // index = target, value = source
  int * target_to_source_fragments; // index = source, value = target
};
