#ifndef EXP_H
#define EXP_H
//int EXP_REG(char REG_SEL[3], unsigned char VAL, struct CPU *pCPU, bool PRINT);
int EXP_GPR(unsigned char GPR_SELECT, signed char VAL, struct CPU *pCPU);
int EXP_RAM(unsigned char ADDRESS, signed char VAL, struct CPU *pCPU);
#endif