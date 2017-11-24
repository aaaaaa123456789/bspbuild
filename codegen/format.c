#include "proto.h"

char * generate_formatted_number_for_file (unsigned number) {
  // rules:
  // 1) anything between -255 and 255 is printed in decimal (123, -45)
  // 2) anything that fits in 16 bits and sign is printed in four hex digits and sign (0x0123, -0x4567)
  // 3) anything else is printed as a full 32-bit unsigned value in hex (0x00012345, 0x89abcdef, 0xfffedcba)
  char * result = malloc(16);
  unsigned negated = 0x100000000 - number; // technically this is just -number, but let's be standards-compliant
  if (number < 0x100)
    sprintf(result, "%u", number);
  else if (number < 0x10000)
    sprintf(result, "0x%04x", number);
  else if (negated < 0x100)
    sprintf(result, "-%u", negated);
  else if (negated < 0x10000)
    sprintf(result, "-0x%04x", negated);
  else
    sprintf(result, "0x%08x", number);
  return result;
}

char * generate_formatted_argument (struct instruction_argument argument, char ** error) {
  // ...
}
