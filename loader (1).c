#include <stdio.h> 
#include <stdlib.h>
#include "loader.h"

void loadToMemory(char* file[], unsigned short int mem[]) {
  FILE *object;
  object = fopen(*file, "r");
  if (object == NULL){
    printf("File not found.\n");
    exit(EXIT_FAILURE);
  }
  while(!feof(object)) {
    int next, next2;
    next = fgetc(object);
    next2 = fgetc(object);
    if (next == 0xCA && next2 == 0xDE) {
      unsigned short int addr, ess, te, rms, address, terms, i;
      addr = fgetc(object);
      ess = fgetc(object);
      address = ((addr & 0x00FF) << 8) | (ess & 0x00FF);
      te = fgetc(object);
      rms = fgetc(object);
      terms = ((te & 0x00FF) << 8) | (rms & 0x00FF);
      for (i = 0; i < terms; i++) {
        unsigned short int cur, rent, current;
        cur = fgetc(object);
        rent = fgetc(object) & 0xFFFF;
        current = ((cur & 0x00FF) << 8) | (rent & 0x00FF);
        mem[address] = current;
        address++;
      }
    }
    
    if (next == 0xDA && next2 == 0xDA) {
      unsigned short int addr, ess, te, rms, address, terms, i;
      addr = fgetc(object);
      ess = fgetc(object);
      address = ((addr & 0x00FF) << 8) | (ess & 0x00FF);
      te = fgetc(object);
      rms = fgetc(object);
      terms = ((te & 0x00FF) << 8) | (rms & 0x00FF);
      for (i = 0; i < terms; i++) {
        unsigned short int cur, rent, current, zero;
        zero = 0;
        cur = fgetc(object);
        cur = cur & 0xFFFF;
        rent = fgetc(object) & 0xFFFF;
        current = ((cur & 0x00FF) << 8) | (rent & 0x00FF);
        mem[address] = current;
        address++;
      }
    }
    
    if (next == 0xC3 && next2 == 0xB7) {
      unsigned short int te, rms, terms, i;
      fgetc(object);
      fgetc(object);
      te = fgetc(object);
      rms = fgetc(object);
      terms = ((te & 0x00FF) << 8) | (rms & 0x00FF);
      for (i = 0; i < terms; i++) {
        fgetc(object);
      }
    }
    if (next == 0xF1 && next2 == 0x7E) {
      unsigned short int te, rms, terms, i;
      te = fgetc(object);
      rms = fgetc(object);
      terms = ((te & 0x00FF) << 8) | (rms & 0x00FF);
      for (i = 0; i < terms; i++) {
        fgetc(object);
      }
    }
    if (next == 0x71 && next2 == 0x5E) {
      unsigned short int i;
      for (i = 0; i < 6; i++) {
        fgetc(object);
      }
    }
  }
  fclose(object);
}
