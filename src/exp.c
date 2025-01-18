#include "main.h"

int EXP_GPR(unsigned char GPR_SELECT, signed char VAL, struct CPU *pCPU) {
    if (pCPU->GPRS[GPR_SELECT] == VAL) {
        return 0;
    }
    return -1;
}
int EXP_RAM(unsigned char ADDRESS, signed char VAL, struct CPU *pCPU) {
    if (pCPU->RAM[ADDRESS] == VAL) {
        return 0;
    }
    return -1;
}