#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../headers/const.h"
#include "../codegen/public.h"
#include "../ips/public.h"
#include "../util/public.h"

#include "public.h"
#include "struct.h"
#include "patchgen.h"

// fragcost.c
unsigned ** generate_fragment_cost_matrix(Buffer, Buffer, unsigned);
unsigned calculate_estimated_fragment_cost(const unsigned char *, const unsigned char *, unsigned);
void determine_minimum_cost_subset(unsigned **, unsigned, unsigned, unsigned *);

// fragperm.c
char * generate_fragment_permutation_table(Buffer, Buffer, const struct patching_flags *, struct fragment_permutation_table **);
char * generate_identity_fragment_permutation_table(unsigned, unsigned, const struct patching_flags *, struct fragment_permutation_table **);
struct fragment_permutation_table * create_fragment_permutation_table(unsigned, unsigned, int);
void destroy_fragment_permutation_table(struct fragment_permutation_table *);

// ftbuild.c
void generate_forward_fragment_permutation_table_from_costs(struct fragment_permutation_table *, unsigned **);
void generate_reversible_fragment_permutation_table_from_costs(struct fragment_permutation_table *, unsigned **);

// ftwriter.c
char * write_fragment_permutation_table_to_codefile(CodeFile, const struct fragment_permutation_table *, int);

// global.c
extern const char * const patching_method_names[];
extern char * (* const patch_generator_functions[])(CodeFile, Buffer, Buffer, const struct patching_flags *);

// ips.c
char * write_ips_patch_data_for_buffers(CodeFile, Buffer, Buffer);

// main.c
char * write_patch_data_to_codefile(CodeFile, unsigned, const unsigned *, const char *);

// xor.c
char * write_xor_patch_buffer_data(CodeFile, const unsigned char *, unsigned);

// xordata.c
char * write_xor_like_patch(CodeFile, Buffer, Buffer, const struct patching_flags *, char * (*) (CodeFile, const unsigned char *, unsigned));
void * generate_xor_data_buffer(const unsigned char *, const unsigned char *, unsigned);
unsigned calculate_unpadded_data_length(const unsigned char *, unsigned, const struct patching_flags *);

// xorfrag.c
char * write_xor_like_fragmented_patch_data(CodeFile, Buffer, Buffer, const struct patching_flags *, char * (*) (CodeFile, const unsigned char *, unsigned));
char * write_xor_like_patch_data_with_fragments(CodeFile, Buffer, Buffer, const struct patching_flags *, const struct fragment_permutation_table *,
                                                char * (*) (CodeFile, const unsigned char *, unsigned));
char * write_xor_like_fragmented_header(CodeFile, unsigned, int, const int *, const unsigned *, const struct patching_flags *,
                                        const struct fragment_permutation_table *);
char * write_xor_like_fragment(CodeFile, const unsigned char *, const unsigned char *, unsigned, int, char * (*) (CodeFile, const unsigned char *, unsigned));
unsigned calculate_fragment_length(const unsigned char *, const unsigned char *, const struct patching_flags *);
void * generate_last_fragment_data(Buffer, unsigned);
const void * select_fragment_data(int, Buffer, const void *, const void *, unsigned);

// xornf.c
char * write_xor_like_unfragmented_header(CodeFile, Buffer, Buffer, const struct patching_flags *, const char *, unsigned);
unsigned calculate_xor_like_unfragmented_data_length(Buffer, Buffer, const struct patching_flags *);
char * write_xor_like_unfragmented_patch_data(CodeFile, Buffer, Buffer, unsigned, const char *, char * (*) (CodeFile, const unsigned char *, unsigned));

// xorrle.c
char * write_xor_rle_patch_buffer_data(CodeFile, const unsigned char *, unsigned);
unsigned generate_rle_data(const unsigned char *, unsigned, unsigned char *);
struct rle_run_data find_next_rle_run(const unsigned char *, unsigned);
void find_rle_byte_run(struct rle_run_data *, const unsigned char *, unsigned);
void find_rle_multibyte_run(struct rle_run_data *, const unsigned char *, unsigned, unsigned char, unsigned char);
unsigned write_rle_data_to_buffer(unsigned char *, const unsigned char *, unsigned);
unsigned write_rle_run_to_buffer(unsigned char *, struct rle_run_data);
