#define SET_OPCODE 0x84
#define SHIFT_OPCODE 0xab

// commands.c
void calculation_command(int, char **);
void mulacum_command(int, char **);
void bit_shift_command(int, char **);

// dircmd.c
void data_command(int, char **);
void hexdata_command(int, char **);
void define_command(int, char **);
void include_command(int, char **);
void string_command(int, char **);
void align_command(int, char **);

// stdcmd.c
void no_arguments_command(int, char **);
void one_argument_command(int, char **);
void two_arguments_command(int, char **);
void one_variable_command(int, char **);
void one_variable_one_argument_command(int, char **);
void one_variable_two_arguments_command(int, char **);
void two_variables_command(int, char **);
void two_variables_two_arguments_command(int, char **);
void standard_command(unsigned char, unsigned char, unsigned char, char **);
void one_byte_argument_command(int, char **);
void one_halfword_argument_command(int, char **);
void one_argument_one_byte_argument(int, char **);
void one_argument_one_halfword_argument(int, char **);
