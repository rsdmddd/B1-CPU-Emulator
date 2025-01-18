#include "main.h"

int EXP_GPR(unsigned char GPR_SELECT, signed char VAL, struct CPU *pCPU) {
    if (pCPU->GPRS[GPR_SELECT] == VAL) {
        printf("[EXP_GPR]: 0")
        return 0;
    }
    printf("[EXP_RAM]: 0")
    return -1;
}
int EXP_RAM(unsigned char ADDRESS, signed char VAL, struct CPU *pCPU) {
    if (pCPU->RAM[ADDRESS] == VAL) {
        printf("[EXP_RAM]: 0")
        return 0;
    }
    printf("[EXP_RAM]: -1")
    return -1;
}