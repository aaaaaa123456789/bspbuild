#include "struct.h"
#include "options.h"

const struct option_parser option_parsers[] = {
  {.option = "-o",                  .argument_callback =    &set_compiled_output_file,    .has_argument = 1},
  {.option = "-s",                  .no_argument_callback = &set_next_input_file_mode,    .has_argument = 0, .parameter = 0},
  {.option = "-st",                 .no_argument_callback = &set_next_input_file_mode,    .has_argument = 0, .parameter = 1},
  {.option = "-t",                  .no_argument_callback = &set_next_input_file_mode,    .has_argument = 0, .parameter = 2},
  {.option = "--bsp",               .no_argument_callback = &set_input_bsp_source_option, .has_argument = 0},
  {.option = "--constant-prefix",   .argument_callback =    &constant_prefix_option,      .has_argument = 1},
  {.option = "--label-prefix",      .argument_callback =    &label_prefix_option,         .has_argument = 1},
  {.option = "--prefix",            .argument_callback =    &prefix_option,               .has_argument = 1},
  {.option = "--register-prefix",   .argument_callback =    &variable_prefix_option,      .has_argument = 1},
  {.option = "--source",            .no_argument_callback = &set_next_input_file_mode,    .has_argument = 0, .parameter = 0},
  {.option = "--source-target",     .no_argument_callback = &set_next_input_file_mode,    .has_argument = 0, .parameter = 1},
  {.option = "--target",            .no_argument_callback = &set_next_input_file_mode,    .has_argument = 0, .parameter = 2},
  // ...
  {.option = NULL}
};
