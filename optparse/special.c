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
    "Use \"bspbuild --help\" for help.\n"
  );
}

Options version_response (void) {
  return special_option_response(
    "bspbuild - version " VERSION_STRING "\n"
    "\n"
    // boring legal disclaimer time!
    "This is public domain software; no copyright is claimed. (See\n"
    "http://unlicense.org/UNLICENSE for details.) No warranty, implied or otherwise,\n"
    "is given via distribution of this software in any form.\n"
  );
}

Options help_response (void) {
  // ...
}
