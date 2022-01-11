#include <stdio.h> 
#include <stdlib.h>
#include "loader.h"
#include "execute.h"

int main(int argc, char **argv) {
	unsigned short int pc = 0x8200, psr = 0x8002;
    int z, regCount;
    MachineState intialState;
    for (z = 0; z < 65536; z++) {
    	intialState.memory[z] = 0;
    } 
    for (regCount = 0; regCount < 8; regCount++) {
    	intialState.R[regCount] = 0;
    }
  	
  	intialState.PC = pc;
  	intialState.PSR = psr;
  	int f;
  	FILE *outputFile;
  	if (argc < 3) {
   		exit(EXIT_FAILURE);
  	}
  	for (f = 2; f < argc; f++) {
    	loadToMemory(&argv[f], intialState.memory);  
  	}
  	outputFile = fopen(argv[1], "w");
  	int rwe = 0, rd = 0, regVal = 0, nwe = 0, nzpVal = 0, dwe = 0, dmem = 0, dval = 0; //check if int is correct int?
  	unsigned short int currPC;
  	while (intialState.PC != 0x80FF) {
  		short int mode;
  		mode = intialState.PSR >> 15;
  		mode = mode & 1;
  		if ((intialState.PC >= 0x2000 && intialState.PC <= 0x7FFF) || (intialState.PC >= 0xA000 && intialState.PC <= 0xFFFF)) {
  			printf("Cannot execute Data as code\n");
  			fclose(outputFile);
  			exit(EXIT_FAILURE);
  		}
  		if ((intialState.PC >= 0x8000 && intialState.PC <= 0xFFFF) && mode != 1) {
  			printf("Cannot access OS memory when in user mode\n");
  			fclose(outputFile);
  			exit(EXIT_FAILURE);
  		}
  		rwe = 0, rd = 0, regVal = 0, nwe = 0, nzpVal = 0, dwe = 0, dmem = 0, dval = 0;
		INSN currInstruction;
		currPC = intialState.PC;
        currInstruction = decode_insn(intialState.memory[intialState.PC]);

        if (currInstruction.type == LDR || currInstruction.type == STR) {
      		dmem = intialState.R[currInstruction.Rs] + currInstruction.immediate;
      	}

      	execute_insn(&intialState, currInstruction);

      	if (currInstruction.type == ADD || currInstruction.type == MUL || currInstruction.type == SUB || currInstruction.type == DIV || 
      	currInstruction.type == ADDI || currInstruction.type == MOD || currInstruction.type == AND || currInstruction.type == NOT || 
      	currInstruction.type == OR || currInstruction.type == XOR || currInstruction.type == ANDI || currInstruction.type == LDR ||
      	currInstruction.type == CONST || currInstruction.type == HICONST || currInstruction.type == SLL || currInstruction.type == SRA ||
      	currInstruction.type == SRL) {
      		rwe = 1;
        	rd = currInstruction.Rd;
        	regVal = intialState.R[rd];
      	}
    	if (currInstruction.type == JSRR || currInstruction.type == JSR || currInstruction.type == TRAP) {
      		rwe = 1;
      		rd = 7;
      		regVal = intialState.R[7];
    	}

		if (currInstruction.type == ADD || currInstruction.type == MUL || currInstruction.type == SUB || currInstruction.type == DIV || 
      	currInstruction.type == ADDI || currInstruction.type == MOD || currInstruction.type == AND || currInstruction.type == NOT || 
      	currInstruction.type == OR || currInstruction.type == XOR || currInstruction.type == ANDI || currInstruction.type == LDR ||
      	currInstruction.type == CONST || currInstruction.type == HICONST || currInstruction.type == CMP || currInstruction.type == CMPU || 
      	currInstruction.type == CMPI ||	currInstruction.type == CMPIU || currInstruction.type == SLL || currInstruction.type == SRA ||
      	currInstruction.type == SRL || currInstruction.type == JSRR || currInstruction.type == JSR || currInstruction.type == TRAP) {
      		nwe = 1;
      		nzpVal = intialState.PSR & 7;
    	}      

      if (currInstruction.type == STR) {
      	dwe = 1;
      }
      if (currInstruction.type == LDR || currInstruction.type == STR) {
      	dval = intialState.memory[dmem];
      }

      fprintf(outputFile, "%04X ", currPC);
      print_binary(outputFile, intialState.memory[currPC]);
      fprintf(outputFile, " %X %X %04X %X %X %X %04X %004X\n", rwe, rd, regVal, nwe, nzpVal, dwe, dmem, dval);
  }
  fclose(outputFile);
}
