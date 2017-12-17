#include "struct.h"
#include "options.h"

const struct option_parser option_parsers[] = {
  {.option = "-o",                          .argument_callback =    &set_compiled_output_file,          .has_argument = 1},
  {.option = "-s",                          .no_argument_callback = &set_next_input_file_mode,          .has_argument = 0, .parameter = 0},
  {.option = "-st",                         .no_argument_callback = &set_next_input_file_mode,          .has_argument = 0, .parameter = 1},
  {.option = "-t",                          .no_argument_callback = &set_next_input_file_mode,          .has_argument = 0, .parameter = 2},
  {.option = "--bsp",                       .no_argument_callback = &set_input_bsp_source_option,       .has_argument = 0},
  {.option = "--constant-prefix",           .argument_callback =    &constant_prefix_option,            .has_argument = 1},
  {.option = "--force-output-menu",         .no_argument_callback = &set_force_output_menu_option,      .has_argument = 0},
  {.option = "--hide-errors",               .no_argument_callback = &set_hide_errors_option,            .has_argument = 0},
  {.option = "--label-prefix",              .argument_callback =    &label_prefix_option,               .has_argument = 1},
  {.option = "--no-output-sha1-validation", .no_argument_callback = &disable_output_validation_option,  .has_argument = 0, .parameter = 1},
  {.option = "--no-output-validation",      .no_argument_callback = &disable_output_validation_option,  .has_argument = 0, .parameter = 2},
  {.option = "--no-source-to-source",       .no_argument_callback = &no_source_to_source_option,        .has_argument = 0},
  {.option = "--prefix",                    .argument_callback =    &prefix_option,                     .has_argument = 1},
  {.option = "--register-prefix",           .argument_callback =    &variable_prefix_option,            .has_argument = 1},
  {.option = "--sort-output-menu",          .no_argument_callback = &sort_target_list_option,           .has_argument = 0},
  {.option = "--source",                    .no_argument_callback = &set_next_input_file_mode,          .has_argument = 0, .parameter = 0},
  {.option = "--source-target",             .no_argument_callback = &set_next_input_file_mode,          .has_argument = 0, .parameter = 1},
  {.option = "--target",                    .no_argument_callback = &set_next_input_file_mode,          .has_argument = 0, .parameter = 2},
  // ...
  {.option = NULL}
};
