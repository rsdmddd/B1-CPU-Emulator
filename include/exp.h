#ifndef EXP_H
#define EXP_H
//int EXP_REG(char REG_SEL[3], unsigned char VAL, struct CPU *pCPU, bool PRINT);
int EXP_GPR(unsigned char GPR_SELECT, unsigned char VAL, struct CPU *pCPU, bool PRINT);
int EXP_RAM(unsigned char ADDRESS, unsigned char VAL, struct CPU *pCPU, bool PRINT);
#endif