#include <stdio.h> 
#include <stdlib.h>
#include "decode.h"
#include "loader.h"

int main(int argc, char **argv) {
  unsigned short int memory[65536];
  int z;
  for (z = 0; z < 65536; z++) {
    memory[z] = 0;
  } 
  int f;
  FILE *outputFile;
  if (argc < 3) {
    exit(EXIT_FAILURE);
  }
  for (f = 2; f < argc; f++) {
    loadToMemory(&argv[f], memory);  
  }
  outputFile = fopen(argv[1], "w");
  int i;
  for (i = 0x0; i < 65536; i++) {
    if (memory[i] != 0 && ((i<=0x1FFF)||(i>=0x8000 && i<=0x9FFF))) {
      INSN currInstruction;
      currInstruction = decode_insn(memory[i]);
      fprintf(outputFile, "%04X : %04X -> ", i, memory[i]);
      print_insn(outputFile, currInstruction);
    }
    if (memory[i] != 0 && !((i<=0x1FFF)||(i>=0x8000 && i<=0x9FFF))) {
      fprintf(outputFile, "%04X : %04X\n", i, memory[i]);
    }
  }
  fclose(outputFile);
}
