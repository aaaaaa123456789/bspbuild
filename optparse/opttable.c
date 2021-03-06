#include "struct.h"
#include "options.h"
#include "../headers/enum.h"
#include "../headers/null.h"

const struct option_parser option_parsers[] = {
  {.option = "-f",                          .argument_callback =    &set_fragment_size,                 .has_argument = 1},
  {.option = "-m",                          .argument_callback =    &set_patch_method_option,           .has_argument = 1},
  {.option = "-o",                          .argument_callback =    &set_compiled_output_file,          .has_argument = 1},
  {.option = "-ob",                         .argument_callback =    &set_bsp_output_file,               .has_argument = 1},
  {.option = "-p",                          .argument_callback =    &set_padding_value,                 .has_argument = 1, .parameter = 3},
  {.option = "-pb",                         .argument_callback =    &set_padding_value,                 .has_argument = 1, .parameter = 1},
  {.option = "-ph",                         .argument_callback =    &set_padding_value,                 .has_argument = 1, .parameter = 2},
  {.option = "-pw",                         .argument_callback =    &set_padding_value,                 .has_argument = 1, .parameter = 3},
  {.option = "-s",                          .no_argument_callback = &set_next_input_file_mode,          .has_argument = 0, .parameter = DIRECTION_SOURCE},
  {.option = "-st",                         .no_argument_callback = &set_next_input_file_mode,          .has_argument = 0, .parameter = DIRECTION_SOURCE_TARGET},
  {.option = "-t",                          .no_argument_callback = &set_next_input_file_mode,          .has_argument = 0, .parameter = DIRECTION_TARGET},
  {.option = "-tl",                         .no_argument_callback = &set_target_and_reference_option,   .has_argument = 0, .parameter = -1},
  {.option = "-tp",                         .no_argument_callback = &set_target_and_reference_option,   .has_argument = 0, .parameter = TARGET_TYPE_PREVIOUS},
  {.option = "-ts",                         .no_argument_callback = &set_target_and_reference_option,   .has_argument = 0, .parameter = TARGET_TYPE_FROM_SOURCE},
  {.option = "--bsp",                       .no_argument_callback = &set_operation_mode_option,         .has_argument = 0, .parameter = OPERATION_MODE_BSP_INPUT},
  {.option = "--check-fragment-swap",       .no_argument_callback = &check_fragment_permutation_option, .has_argument = 0},
  {.option = "--constant-prefix",           .argument_callback =    &constant_prefix_option,            .has_argument = 1},
  {.option = "--error-message",             .argument_callback =    &error_message_option,              .has_argument = 1},
  {.option = "--error-message-from-file",   .argument_callback =    &error_message_from_file_option,    .has_argument = 1},
  {.option = "--error-text-substitute",     .argument_callback =    &error_text_substitute_option,      .has_argument = 1},
  {.option = "--force-output-menu",         .no_argument_callback = &set_force_output_menu_option,      .has_argument = 0},
  {.option = "--hide-errors",               .no_argument_callback = &set_hide_errors_option,            .has_argument = 0},
  {.option = "--initial-register",          .argument_callback =    &set_initial_register_option,       .has_argument = 1},
  {.option = "--ips",                       .no_argument_callback = &set_operation_mode_option,         .has_argument = 0, .parameter = OPERATION_MODE_IPS_OUTPUT},
  {.option = "--label-prefix",              .argument_callback =    &label_prefix_option,               .has_argument = 1},
  {.option = "--patch-method",              .argument_callback =    &set_patch_method_option,           .has_argument = 1},
  {.option = "--no-output-sha1-validation", .no_argument_callback = &disable_output_validation_option,  .has_argument = 0, .parameter = VALIDATE_LENGTH},
  {.option = "--no-output-validation",      .no_argument_callback = &disable_output_validation_option,  .has_argument = 0, .parameter = VALIDATE_NONE},
  {.option = "--no-source-to-source",       .no_argument_callback = &no_source_to_source_option,        .has_argument = 0},
  {.option = "--opening-banner",            .argument_callback =    &opening_banner_option,             .has_argument = 1},
  {.option = "--opening-banner-from-file",  .argument_callback =    &opening_banner_from_file_option,   .has_argument = 1},
  {.option = "--prefix",                    .argument_callback =    &prefix_option,                     .has_argument = 1},
  {.option = "--register-prefix",           .argument_callback =    &variable_prefix_option,            .has_argument = 1},
  {.option = "--sort-output-menu",          .no_argument_callback = &sort_target_list_option,           .has_argument = 0},
  {.option = "--source",                    .no_argument_callback = &set_next_input_file_mode,          .has_argument = 0, .parameter = DIRECTION_SOURCE},
  {.option = "--source-detection-message",  .argument_callback =    &source_detection_message_option,   .has_argument = 1},
  {.option = "--source-target",             .no_argument_callback = &set_next_input_file_mode,          .has_argument = 0, .parameter = DIRECTION_SOURCE_TARGET},
  {.option = "--success-message",           .argument_callback =    &success_message_option,            .has_argument = 1},
  {.option = "--success-message-from-file", .argument_callback =    &success_message_from_file_option,  .has_argument = 1},
  {.option = "--suppress-errors",           .no_argument_callback = &suppress_errors_option,            .has_argument = 0},
  {.option = "--suppress-source-message",   .no_argument_callback = &suppress_source_message_option,    .has_argument = 0},
  {.option = "--target",                    .no_argument_callback = &set_next_input_file_mode,          .has_argument = 0, .parameter = DIRECTION_TARGET},
  {.option = "--target-from",               .argument_callback    = &set_named_reference_target_option, .has_argument = 1},
  {.option = "--target-from-last",          .no_argument_callback = &set_target_and_reference_option,   .has_argument = 0, .parameter = -1},
  {.option = "--target-from-previous",      .no_argument_callback = &set_target_and_reference_option,   .has_argument = 0, .parameter = TARGET_TYPE_PREVIOUS},
  {.option = "--target-from-source",        .no_argument_callback = &set_target_and_reference_option,   .has_argument = 0, .parameter = TARGET_TYPE_FROM_SOURCE},
  {.option = "--targets-per-page",          .argument_callback =    &targets_per_page_option,           .has_argument = 1},
  {.option = "--titles",                    .argument_callback =    &title_file_option,                 .has_argument = 1},
  {.option = "--titles-from-stdin",         .no_argument_callback = &titles_from_stdin_option,          .has_argument = 0},
  {.option = NULL}
};
