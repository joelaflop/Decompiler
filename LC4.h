#include "loader.h"
 
 void decode();
 void code(inst *instr);
 void data(inst *instr);
 void symbol(inst *instr);
 void fileName(inst *instr);
 void lineNumber(inst *instr);
 char** getMem();
 void freeMem();
