#ifndef ___CODEGEN_PUBTYPES

#define ___CODEGEN_PUBTYPES

typedef struct code_file_info * CodeFile;

enum {
  // no parameter
  ARGTYPE_NONE, // end of argument list
  // unsigned parameter
  ARGTYPE_IMMEDIATE,
  ARGTYPE_NAMED_CONSTANT,
  ARGTYPE_REGISTER,
  ARGTYPE_NAMED_REGISTER,
  ARGTYPE_NAMED_LABEL,
  ARGTYPE_NUMERIC_LOCAL,
  ARGTYPE_NUMERIC_DATA,
  // const char * parameter
  ARGTYPE_LOCAL_LABEL,
  ARGTYPE_GLOBAL_LABEL,
  ARGTYPE_PASSTHROUGH // no validation, copy exactly
};

enum {
  INST_ADD,
  INST_ADD2,
  INST_ADDCARRY,
  INST_AND,
  INST_AND2,
  INST_BSPPATCH,
  INST_BUFCHAR,
  INST_BUFNUMBER,
  INST_BUFSTRING,
  INST_CALL,
  INST_CALLNZ,
  INST_CALLZ,
  INST_CHECKSHA1,
  INST_CLEARBUF,
  INST_DECREMENT,
  INST_DIVIDE,
  INST_DIVIDE2,
  INST_EXIT,
  INST_FILLBYTE,
  INST_FILLHALFWORD,
  INST_FILLWORD,
  INST_GETBYTE,
  INST_GETBYTEDEC,
  INST_GETBYTEINC,
  INST_GETFILEBYTE,
  INST_GETFILEHALFWORD,
  INST_GETFILEWORD,
  INST_GETHALFWORD,
  INST_GETHALFWORDDEC,
  INST_GETHALFWORDINC,
  INST_GETSTACKSIZE,
  INST_GETVARIABLE,
  INST_GETWORD,
  INST_GETWORDDEC,
  INST_GETWORDINC,
  INST_IFEQ,
  INST_IFGE,
  INST_IFGT,
  INST_IFLE,
  INST_IFLT,
  INST_IFNE,
  INST_INCREMENT,
  INST_IPSPATCH,
  INST_JUMP,
  INST_JUMPNZ,
  INST_JUMPTABLE,
  INST_JUMPZ,
  INST_LENGTH,
  INST_LOCKPOS,
  INST_LONGMUL,
  INST_LONGMULACUM,
  INST_MULACUM,
  INST_MENU,
  INST_MULTIPLY,
  INST_MULTIPLY2,
  INST_NOP,
  INST_OR,
  INST_OR2,
  INST_POP,
  INST_POPPOS,
  INST_POS,
  INST_PRINT,
  INST_PRINTBUF,
  INST_PUSH,
  INST_PUSHPOS,
  INST_READBYTE,
  INST_READHALFWORD,
  INST_READWORD,
  INST_REMAINDER,
  INST_REMAINDER2,
  INST_RETNZ,
  INST_RETURN,
  INST_RETZ,
  INST_ROTATELEFT,
  INST_ROTATELEFT2,
  INST_SEEK,
  INST_SEEKBACK,
  INST_SEEKEND,
  INST_SEEKFWD,
  INST_SET,
  INST_SETSTACKSIZE,
  INST_SHIFTLEFT,
  INST_SHIFTLEFT2,
  INST_SHIFTRIGHT,
  INST_SHIFTRIGHT2,
  INST_SHIFTRIGHTARITH,
  INST_SHIFTRIGHTARITH2,
  INST_STACKREAD,
  INST_STACKSHIFT,
  INST_STACKWRITE,
  INST_SUBBORROW,
  INST_SUBTRACT,
  INST_SUBTRACT2,
  INST_TRUNCATE,
  INST_TRUNCATEPOS,
  INST_UNLOCKPOS,
  INST_WRITEBYTE,
  INST_WRITEDATA,
  INST_WRITEHALFWORD,
  INST_WRITEWORD,
  INST_XOR,
  INST_XOR2,
  INST_XORDATA
};

#endif
