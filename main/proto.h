#include <stdio.h>
#include <stdlib.h>

#include "../headers/const.h"
#include "../bsp/public.h"
#include "../codegen/public.h"
#include "../ips/public.h"
#include "../optparse/public.h"
#include "../util/public.h"

// altmodes.c
int bsp_input_operation_mode(Options);
int ips_output_operation_mode(Options);

// generate.c
char * generate_code(Options, CodeFile);

// main.c
int main(int, char **);
int normal_operation_mode(Options);
