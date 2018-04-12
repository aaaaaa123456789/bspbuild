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
  unsigned fragment;
  for (fragment = 0; fragment < table -> target_fragments; fragment ++) table -> source_to_target_fragments[fragment] = -1;
  for (fragment = 0; fragment < table -> source_fragments; fragment ++) table -> target_to_source_fragments[fragment] = -1;
  if (!(table -> source_fragments && table -> target_fragments)) return;
  unsigned * minimum_subset = malloc((((table -> source_fragments + table -> target_fragments) << 1) + 3) * sizeof(unsigned));
  determine_minimum_cost_subset(cost_matrix, table -> source_fragments + 1, table -> target_fragments + 1, minimum_subset);
  unsigned * current;
  for (current = minimum_subset; *current != -1U; current += 2) {
    if (*current != table -> source_fragments)
      if (
        (table -> target_to_source_fragments[*current] == -1) ||
        (cost_matrix[*current][current[1]] < cost_matrix[*current][table -> target_to_source_fragments[*current]])
      ) table -> target_to_source_fragments[*current] = current[1];
    if (current[1] != table -> target_fragments)
      if (
        (table -> source_to_target_fragments[current[1]] == -1) ||
        (cost_matrix[*current][current[1]] < cost_matrix[table -> source_to_target_fragments[current[1]]][current[1]])
      ) table -> source_to_target_fragments[current[1]] = *current;
  }
  free(minimum_subset);
  for (fragment = 0; fragment < table -> target_fragments; fragment ++)
    if (table -> source_to_target_fragments[fragment] == table -> source_fragments) table -> source_to_target_fragments[fragment] = -1;
  for (fragment = 0; fragment < table -> source_fragments; fragment ++)
    if (table -> target_to_source_fragments[fragment] == table -> target_fragments) table -> target_to_source_fragments[fragment] = -1;
}
