#include "proto.h"

void generate_forward_fragment_permutation_table_from_costs (struct fragment_permutation_table * table, unsigned ** cost_matrix) {
  if (!table -> target_fragments) return;
  unsigned * best_rows = calloc(table -> target_fragments, sizeof(unsigned));
  unsigned * best_costs = malloc(table -> target_fragments * sizeof(unsigned));
  memcpy(best_costs, *cost_matrix, table -> target_fragments * sizeof(unsigned));
  unsigned source_fragment, target_fragment;
  for (source_fragment = 1; source_fragment <= table -> source_fragments; source_fragment ++)
    for (target_fragment = 0; target_fragment < table -> target_fragments; target_fragment ++)
      if (cost_matrix[source_fragment][target_fragment] < best_costs[target_fragment]) {
        best_rows[target_fragment] = source_fragment;
        best_costs[target_fragment] = cost_matrix[source_fragment][target_fragment];
      }
  free(best_costs);
  for (target_fragment = 0; target_fragment < table -> target_fragments; target_fragment ++)
    table -> source_to_target_fragments[target_fragment] = (best_rows[target_fragment] == table -> source_fragments) ? -1 : best_rows[target_fragment];
  free(best_rows);
}

void generate_reversible_fragment_permutation_table_from_costs (struct fragment_permutation_table * table, unsigned ** cost_matrix) {
  // ...
}
