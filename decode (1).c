#include <stdio.h> 
#include <stdlib.h>
#include "decode.h"

const char* instructions[] = {"ILLEGAL_INSN","NOP", "BRp", "BRz", "BRzp", "BRn", "BRnp", "BRnz", "BRnzp", "ADD", "MUL", "SUB", "DIV", "ADD", "MOD", "AND", "NOT", "OR", "XOR", "AND", "LDR", "STR", "CONST", "HICONST", "CMP", "CMPU", "CMPI", "CMPIU", "SLL", "SRA", "SRL", "JSRR", "JSR", "JMPR", "JMP", "TRAP", "RTI"};

short int sign_extend(short int value, int bits) {
  int msb;
  msb = value >> (bits - 1);
  msb = msb & 1;
  if (msb == 0) {
    return value;
  }
  if (msb == 1) {
    int andNum = 0;
    int i;
    for (i = bits; i < 16; i++) {
      int curr, j;
      curr = 2;
      for (j = 1; j < i; j++) {
        curr = curr * 2;
      }
      andNum = andNum + curr; 
    }
    return value | andNum;
  }
  else {
    return value;
  }
}

INSN decode_insn (unsigned short int insn) {
  INSN ret;
  int op, subop;
  op = insn >> 12;
  subop = insn >> 3;
  subop = subop & 7;
  if (op == 0) {
    int nzp, immed;
    immed = insn & 511;
    ret.immediate = sign_extend(immed, 9);
    nzp = insn >> 9;
    if (nzp == 0) {
      ret.type = NOP;
    }
    if (nzp == 1) {
      ret.type = BRp;
    }
    if (nzp == 2) {
      ret.type = BRz;
    }
    if (nzp == 3) {
      ret.type = BRzp;
    }
    if (nzp == 4) {
      ret.type = BRn;
    }
    if (nzp == 5) {
      ret.type = BRnp;
    }
    if (nzp == 6) {
      ret.type = BRnz;
    }
    if (nzp == 7) {
      ret.type = BRnzp;
    }
  }
  
  if (op == 1) {
    int rd, rs, rt;
    rd = insn >> 9;
    rd = rd & 7;
    rs = insn >> 6;
    rs = rs & 7;
    rt = insn & 7;
    if (subop == 0) {
      ret.type = ADD;
      ret.Rd = rd;
      ret.Rs = rs;
      ret.Rt = rt;
    }
    if (subop == 1) {
      ret.type = MUL;
      ret.Rd = rd;
      ret.Rs = rs;
      ret.Rt = rt;
    }
    if (subop == 2) {
      ret.type = SUB;
      ret.Rd = rd;
      ret.Rs = rs;
      ret.Rt = rt;
    }
    if (subop == 3) {
      ret.type = DIV;
      ret.Rd = rd;
      ret.Rs = rs;
      ret.Rt = rt;
    }
    if ((subop >> 2) == 1) {
      ret.type = ADDI;
      ret.Rd = rd;
      ret.Rs = rs;
      int immed;
      immed = insn & 31;
      ret.immediate = sign_extend(immed, 5);
    }
  }
    
  if (op == 5) {
    int rd, rs, rt;
    rd = insn >> 9;
    rd = rd & 7;
    rs = insn >> 6;
    rs = rs & 7;
    rt = insn & 7;
    if (subop == 0) {
      ret.type = AND;
      ret.Rd = rd;
      ret.Rs = rs;
      ret.Rt = rt;
    }
    if (subop == 1) {
      ret.type = NOT;
      ret.Rd = rd;
      ret.Rs = rs;
    }
    if (subop == 2) {
      ret.type = OR;
      ret.Rd = rd;
      ret.Rs = rs;
      ret.Rt = rt;
    }
    if (subop == 3) {
      ret.type = XOR;
      ret.Rd = rd;
      ret.Rs = rs;
      ret.Rt = rt;
    }
    if ((subop >> 2) == 1) {
      ret.type = ANDI;
      ret.Rd = rd;
      ret.Rs = rs;
      int immed;
      immed = insn & 31;
      ret.immediate = sign_extend(immed, 5);
    }
  }
    
  if (op == 2) {
    int rs, rt, comp;
    rs = insn >> 9;
    rs = rs & 7;
    rt = insn & 7;
    comp = insn >> 7;
    comp = comp & 3;
    if (comp == 0) {
      ret.type = CMP;
      ret.Rs = rs;
      ret.Rt = rt;
    }
    if (comp == 1) {
      ret.type = CMPU;
      ret.Rt = rt;
      ret.Rs = rs;
    }
    if (comp == 2) {
      ret.type = CMPI;
      ret.Rs = rs;
      int immed;
      immed = insn & 127;
      ret.immediate = sign_extend(immed, 7);
    }
    if (comp == 3) {
      ret.type = CMPIU;
      ret.Rs = rs;
      unsigned short int uuuuuuu;
      uuuuuuu = insn & 127;
      ret.immediate = uuuuuuu;
    }
  }
    
  if (op == 10) {
    int rd, rs, rt, shift;
    rd = insn >> 9;
    rd = rd & 7;
    rs = insn >> 6;
    rs = rs & 7;
    rt = insn & 7;
    shift = insn >> 4;
    shift = shift & 3;
    unsigned short int uuuu;
    uuuu = insn & 15;

    if (shift == 0) {
      ret.type = SLL;
      ret.Rd = rd;
      ret.Rs = rs;
      ret.immediate = uuuu;
    }
    if (shift == 1) {
      ret.type = SRA;
      ret.Rd = rd;
      ret.Rs = rs;
      ret.immediate = uuuu;
    }
    if (shift == 2) {
      ret.type = SRL;
      ret.Rd = rd;
      ret.Rs = rs;
      ret.immediate = uuuu;
    }
    if (shift == 3) {
      ret.type = MOD;
      ret.Rd = rd;
      ret.Rs = rs;
      ret.Rt = rt;
    }
  }
  
  if (op == 4) {
    int rs, jump;
    rs = insn >> 6;
    rs = rs & 7;
    jump = insn >> 11;
    jump = jump & 1;
    if (jump == 0) {
      ret.type = JSRR;
      ret.Rs = rs;
    }
    if (jump == 1) {
      ret.type = JSR;
      ret.Rs = rs;
      int immed;
      immed = insn & 2047;
      ret.immediate = sign_extend(immed, 11);
    }
  }
  
  if (op == 12) {
    int rs, jump;
    rs = insn >> 6;
    rs = rs & 7;
    jump = insn >> 11;
    jump = jump & 1;
    if (jump == 0) {
      ret.type = JMPR;
      ret.Rs = rs;
    }
    if (jump == 1) {
      ret.type = JMP;
      ret.Rs = rs;
      int immed;
      immed = insn & 2047;
      ret.immediate = sign_extend(immed, 11);
    }
  }
  
  if (op == 6) {
    int rd, rs;
    rd = insn >> 9;
    rd = rd & 7;
    rs = insn >> 6;
    rs = rs & 7;
    ret.type = LDR;
    ret.Rd = rd;
    ret.Rs = rs;
    int immed;
    immed = insn & 63;
    ret.immediate = sign_extend(immed, 6);
  }
  
  if (op == 7) {
    int rt, rs;
    rt = insn >> 9;
    rt = rt & 7;
    rs = insn >> 6;
    rs = rs & 7;
    ret.type = STR;
    ret.Rt = rt;
    ret.Rs = rs;
    int immed;
    immed = insn & 63;
    ret.immediate = sign_extend(immed, 6);
  }
  
  if (op == 9) {
    int rd;
    rd = insn >> 9;
    rd = rd & 7;
    ret.type = CONST;
    ret.Rd = rd;
    int immed;
    immed = insn & 511;
    ret.immediate = sign_extend(immed, 9);
  }
  
  if (op == 13) {
    int rd;
    rd = insn >> 9;
    rd = rd & 7;
    ret.type = HICONST;
    ret.Rd = rd;
    unsigned short int eightbitunsigned;
    eightbitunsigned = insn & 255;
    ret.immediate = eightbitunsigned;
  }
  
  if (op == 15) {
    ret.type = TRAP;
    unsigned short int eightbitunsigned;
    eightbitunsigned = insn & 255;
    ret.immediate = eightbitunsigned;
  }
  
  if (op == 8) {
    ret.type = RTI;
  }
  if (op != 0 && op != 1 && op != 2 && op != 4 && op != 5 && op != 6 && op != 7 && op != 8 && op != 9 && op != 10 && op != 12 && op != 13 && op != 15) {
    ret.type = ILLEGAL_INSN;
  }
  return ret;
}

void print_insn (FILE *theFile, INSN theINSN) {
  if (theINSN.type == ADDI || theINSN.type == ANDI || theINSN.type == LDR || theINSN.type == SLL || theINSN.type == SRA || theINSN.type == SRL) {
    fprintf(theFile, "%s R%d, R%d, #%hd\n", instructions[theINSN.type], theINSN.Rd, theINSN.Rs, theINSN.immediate);
  }
  if (theINSN.type == CONST || theINSN.type == HICONST) {
    fprintf(theFile, "%s R%d, #%hd\n", instructions[theINSN.type], theINSN.Rd, theINSN.immediate);
  }
  if (theINSN.type == CMP || theINSN.type == CMPU) {
    fprintf(theFile, "%s R%d, R%d\n", instructions[theINSN.type], theINSN.Rs, theINSN.Rt);
  }
  if (theINSN.type == CMPI || theINSN.type == CMPIU) {
    fprintf(theFile, "%s R%d, #%hd\n", instructions[theINSN.type], theINSN.Rs, theINSN.immediate);
  }
  if (theINSN.type == JSRR || theINSN.type == JMPR) {
    fprintf(theFile, "%s R%d\n", instructions[theINSN.type], theINSN.Rs);
  }
  if (theINSN.type == JSR || theINSN.type == JMP || theINSN.type == TRAP || theINSN.type == BRp || theINSN.type == BRz || theINSN.type == BRzp || theINSN.type == BRn || theINSN.type == BRnp || theINSN.type == BRnz || theINSN.type == BRnzp) {
    fprintf(theFile, "%s #%hd\n", instructions[theINSN.type], theINSN.immediate);
  }
  if (theINSN.type == NOT) {
    fprintf(theFile, "%s R%d, R%d\n", instructions[theINSN.type], theINSN.Rd, theINSN.Rs);
  }
  if (theINSN.type == STR) {
    fprintf(theFile, "%s R%d, R%d, #%hd\n", instructions[theINSN.type], theINSN.Rt, theINSN.Rs, theINSN.immediate);
  }
  if (theINSN.type == RTI || theINSN.type == NOP) {
    fprintf(theFile, "%s\n", instructions[theINSN.type]);
  }
  if (theINSN.type == ILLEGAL_INSN) {
    fprintf(theFile, "INVALID INSTRUCTION\n");
  }
  if (theINSN.type == ADD || theINSN.type == MUL || theINSN.type == SUB || theINSN.type == DIV || theINSN.type == MOD || theINSN.type == AND || theINSN.type == OR || theINSN.type == XOR) {
    fprintf(theFile, "%s R%d, R%d, R%d\n", instructions[theINSN.type], theINSN.Rd, theINSN.Rs, theINSN.Rt);
  }
}
