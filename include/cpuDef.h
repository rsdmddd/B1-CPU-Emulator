#ifndef CPUDEF_H
#define CPUDEF_H

#define INS_LEN 5
#define NEXT_INS 6
#define IMMA_OFFSET 1
#define IMMB_OFFSET 2
#define REGA_OFFSET 3
#define REGB_OFFSET 4
#define REGD_OFFSET 5

typedef enum {

    HLT = 1,
    UIN = 2,
    LDI = 3,
    MOV = 4,
    STO = 5,
    LOAD = 6,
    PUSH = 7,
    POP = 8, 
    ADD = 9,
    SUB = 10,
    SWC = 11,
    AND = 12,
    ORR = 13,
    XOR = 14,
    NAND = 15,
    NOR = 16,
    XNOR = 17,
    JMP = 18,
    CJM = 19,
    SJM = 20,
    CALL = 21,
    RET = 22,
    PTS = 23,
    PTL = 24

    //might seem unclean to define it here
    #define ALL   0b1111 
    #define NALL  0b0000
    #define NEG   0b1000
    #define EVEN  0b0100
    #define ZERO  0b0010
    #define CARRY 0b0001
} Instruction;

struct CPU
{
    /*Programme Counter, Stack Pointer, Flags Register, 
    User Input Register, Port Out*/
    
    unsigned char *pPIN; //pointer for port in for other created CPUS
    unsigned char PC, SP, FR;
    signed char UI, PIN, POU;
    signed char AC;
    const unsigned char R0; //Initalized in Setup
    signed char GPRS[8]; //wei proposed 16
    bool halted;
    unsigned char I_ROM[255], RAM[255];
};

#endif
