#ifndef ___PATCH_PUBTYPES

#define ___PATCH_PUBTYPES

struct patching_flags {
  unsigned fragment_size;
  unsigned padding_value;
  unsigned patching_method:       6;
  unsigned padding_length:        2;
  unsigned fragmentation_enabled: 1;
  unsigned fragment_reordering:   1;
  unsigned reversible_patch:      1;
};

#endif
