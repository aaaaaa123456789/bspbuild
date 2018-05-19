#include "proto.h"

Options special_option_response (const char * response) {
  Options result = new_options_object();
  unsigned length = strlen(response) + 1;
  result -> response = mr_malloc(result -> memory_region, length);
  memcpy(result -> response, response, length);
  return result;
}

Options no_arguments_response (void) {
  return special_option_response(
    "bspbuild - version " VERSION_STRING "\n"
    "Use \"bspbuild --help\" for help."
  );
}

Options version_response (void) {
  return special_option_response(
    "bspbuild - version " VERSION_STRING "\n"
    "\n"
    // boring legal disclaimer time!
    "This is public domain software; no copyright is claimed. (See\n"
    "http://unlicense.org/UNLICENSE for details.) No warranty, implied or otherwise,\n"
    "is given via distribution of this software in any form."
  );
}

Options help_response (void) {
  return special_option_response(
    "bspbuild {-s <source files> | -st <source+target files> | -t <target files>}\n"
    "         [-m <method>] [-tp | -tl | -ts | --target-from <reference file>]\n"
    "         {-o <output> | -ob <source code output>} [-f <fragment size>]\n"
    "         [-p[b|h|w] <padding value>] [--check-fragment-swap]\n"
    "         [--targets-per-page <value>] [--force-output-menu]\n"
    "         [--sort-output-menu] [--titles <file> | --titles-from-stdin]\n"
    "         [--opening-banner <message> | --opening-banner-from-file <file>]\n"
    "         [--success-message <message> | --success-message-from-file <file>]\n"
    "         [--source-detection-message <message> | --suppress-source-message]\n"
    "         [--error-message <message> | --error-message-from-file <file> |\n"
    "          --suppress-errors] [--error-text-substitute <placeholder>]\n"
    "         [--no-output-validation | --no-output-sha1-validation]\n"
    "         [--no-source-to-source] [--initial-register <value>]\n"
    "         [--prefix <prefix> | [--constant-prefix <prefix>]\n"
    "          [--register-prefix <prefix>] [--label-prefix <prefix>]]\n"
    "\n"
    "bspbuild --ips <source file> <target file> -o <output>\n"
    "\n"
    "bspbuild --bsp <files> -o <output>\n"
    "\n"
    "Check the online documentation available at\n"
    "https://github.com/aaaaaa123456789/bspbuild/blob/version-" VERSION_STRING "/docs.md\n"
    "for further information and explanations about the command-line options."
  );
}
