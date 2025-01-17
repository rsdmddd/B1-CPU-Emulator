#include "main.h"

int EXP_GPR(unsigned char GPR_SELECT, signed char VAL, struct CPU *pCPU, bool PRINT) {
    if (pCPU->GPRS[GPR_SELECT] == VAL) {
        if (PRINT == true) {
        printf("[EXP_GPR]: will return 0\n");
        return 0;
        }
    }
    if (PRINT == true) {
    printf("[EXP_GPR]: will return -1\n");
    return -1;
    }
}

int EXP_RAM(unsigned char ADDRESS, signed char VAL, struct CPU *pCPU, bool PRINT) {
    if (pCPU->RAM[ADDRESS] == VAL) {
        if (PRINT == true) {
        printf("[EXP_RAM]: will return 0\n");
        }
        return 0;
    }
    if (PRINT == true) {
    printf("[EXP_RAM]: will return -1\n");
    }
    return -1;
}