#include "struct.h"
#include "options.h"

const struct option_parser option_parsers[] = {
  {.option = "--constant-prefix",   .argument_callback =   &constant_prefix_option,      .has_argument = 1},
  {.option = "--label-prefix",      .argument_callback =   &label_prefix_option,         .has_argument = 1},
  {.option = "--prefix",            .argument_callback =   &prefix_option,               .has_argument = 1},
  {.option = "--register-prefix",   .argument_callback =   &variable_prefix_option,      .has_argument = 1},
  // ...
  {.option = NULL}
};
