#include "main.h" //contains most undefined stuff here
#include "cpuDef.h"
#include "exp.h"

static inline void Setup(struct CPU * pCPU, * pPIN) {
  memset(pCPU, 0, sizeof(struct CPU));
  pCPU->pPIN = pPIN;
}
static inline int ReadGPR(unsigned char GPR_SELECT, struct CPU * pCPU) {
  return pCPU -> GPRS[GPR_SELECT]; 
}
static inline int FetchByte(unsigned char ADDRESS, struct CPU * pCPU) {
  return pCPU -> I_ROM[ADDRESS]; 
}
static inline int WriteGPR(unsigned char GPR_SELECT, signed char VAL, struct CPU * pCPU) {
  pCPU -> GPRS[GPR_SELECT] = VAL; 
  return 0;
}
int Execute(size_t ticks, struct CPU *pCPU) {
    for (size_t i = 0; i < ticks; i++) {
      if (pCPU->halted == true) {
          return -1;
      }
      signed char result;
      switch (pCPU->I_ROM[pCPU->PC]) {
          case HLT:
              pCPU->halted = true;
              break;
          case UIN:
              printf("Enter user input: ");
              scanf("%d", &pCPU->UI);
              break;
          case LDI:
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU), 
                      FetchByte(pCPU->PC + IMMA_OFFSET, pCPU), pCPU);
              break;
          case MOV:
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU),
                      ReadGPR(FetchByte(pCPU->PC + REGA_OFFSET, pCPU), pCPU), pCPU);
              break;
          case STO:
              pCPU->RAM[FetchByte(pCPU->PC + IMMA_OFFSET, pCPU)] =
                  FetchByte(pCPU->PC + IMMB_OFFSET, pCPU);
              break;
          case LOAD:
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU),
                      pCPU->RAM[FetchByte(pCPU->PC + IMMA_OFFSET, pCPU)], pCPU);
              break;
          case ADD:
              result = (ReadGPR(FetchByte(pCPU->PC + REGA_OFFSET, pCPU), pCPU) +
                        ReadGPR(FetchByte(pCPU->PC + REGB_OFFSET, pCPU), pCPU));
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU), result, pCPU);
              break;
          case SUB:
              result = (ReadGPR(FetchByte(pCPU->PC + REGA_OFFSET, pCPU), pCPU) -
                        ReadGPR(FetchByte(pCPU->PC + REGB_OFFSET, pCPU), pCPU));
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU), result, pCPU);
              break;
          case SWC:
              result = (ReadGPR(FetchByte(pCPU->PC + REGA_OFFSET, pCPU), pCPU) -
                        ReadGPR(FetchByte(pCPU->PC + REGB_OFFSET, pCPU), pCPU)) + 1;
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU), result, pCPU);
              break;
          case AND:
              result = (ReadGPR(FetchByte(pCPU->PC + REGA_OFFSET, pCPU), pCPU) &
                        ReadGPR(FetchByte(pCPU->PC + REGB_OFFSET, pCPU), pCPU));
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU), result, pCPU);
              break;
          case ORR:
              result = (ReadGPR(FetchByte(pCPU->PC + REGA_OFFSET, pCPU), pCPU) |
                        ReadGPR(FetchByte(pCPU->PC + REGB_OFFSET, pCPU), pCPU));
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU), result, pCPU);
              break;
          case XOR:
              result = (ReadGPR(FetchByte(pCPU->PC + REGA_OFFSET, pCPU), pCPU) ^
                        ReadGPR(FetchByte(pCPU->PC + REGB_OFFSET, pCPU), pCPU));
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU), result, pCPU);
              break;
          case NOR:
              result = ~(ReadGPR(FetchByte(pCPU->PC + REGA_OFFSET, pCPU), pCPU) |
                        ReadGPR(FetchByte(pCPU->PC + REGB_OFFSET, pCPU), pCPU));
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU), result, pCPU);
              break;
          case PUSH:
              pCPU->SP++;
              pCPU->RAM[pCPU->SP] = pCPU->GPRS[FetchByte(pCPU->PC + REGA_OFFSET, pCPU)];
              break;
          case POP: 
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU),
                      pCPU->RAM[pCPU->SP], pCPU);
              pCPU->RAM[pCPU->SP] = 0;
              pCPU->SP--;
              break;
          case JMP:
              pCPU->PC = FetchByte(pCPU->PC + IMMA_OFFSET, pCPU);
              break;
          case CJM:
              if (FetchByte(pCPU->PC + IMMA_OFFSET, pCPU) == pCPU->FR) {
                  pCPU->PC = (FetchByte(pCPU->PC + IMMB_OFFSET, pCPU) - 1);
              }
              break;
          case SJM:
              if (FetchByte(PC+IMMA_OFFSET, pCPU) ^ pCPU->FR > 0) {
                pCPU->PC = FetchByte(pCPU->PC + IMMB_OFFSET, pCPU);
              }
              break;
          case CALL:
              pCPU->SP++;
              pCPU->RAM[pCPU->SP] = pCPU->PC + NEXT_INS;
              pCPU->PC = FetchByte(pCPU->PC + IMMA_OFFSET, pCPU) - NEXT_INS;
              break;
          case RET:
              pCPU->PC = pCPU->RAM[pCPU->SP];
              pCPU->RAM[pCPU->SP] = 0;
              pCPU->SP--;
              pCPU->PC -= NEXT_INS;
              break;
          case PTS:
              WriteGPR(FetchByte(PC+REGD_OFFSET, pCPU),
              *(pCPU->pPIN), pCPU);
          case PTL:
              pCPU->PIN = *(pCPU->pPIN);  
          case PTS
              WriteGPR(FetchByte(PC+REGA_OFFSET, pCPU), *(pCPU->pPIN), pCPU)
          default:
              printf("Unrecognized opcode at %d\n", pCPU->PC);
              break;
        }
        pCPU->PC += NEXT_INS;
        pCPU->AC = result;
    }
    return 0;
}

int main() {
  struct CPU newCPU;
  struct CPU secondCPU;
  Setup(&newCPU);
  Setup(&secondCPU);
  newCPU.pPIN = &secondCPU.PIN
  secondCPU.pPIN = &newCPU.PIN
//Fib program
   int nextInstruction = 0;
   newCPU.I_ROM[nextInstruction] = LDI;
   newCPU.I_ROM[nextInstruction + IMMA_OFFSET] = 0x0; 
   newCPU.I_ROM[nextInstruction + REGD_OFFSET] = 0x1; 
   nextInstruction += NEXT_INS;
   newCPU.I_ROM[nextInstruction] = LDI;
   newCPU.I_ROM[nextInstruction + IMMA_OFFSET] = 0x1; 
   newCPU.I_ROM[nextInstruction + REGD_OFFSET] = 0x2; 
   nextInstruction += NEXT_INS;
   newCPU.I_ROM[nextInstruction] = ADD;
   newCPU.I_ROM[nextInstruction + REGA_OFFSET] = 0x1; 
   newCPU.I_ROM[nextInstruction + REGB_OFFSET] = 0x2; 
   newCPU.I_ROM[nextInstruction + REGD_OFFSET] = 0x3; 
   nextInstruction += NEXT_INS;
   newCPU.I_ROM[nextInstruction] = MOV;
   newCPU.I_ROM[nextInstruction + REGA_OFFSET] = 0x2; 
   newCPU.I_ROM[nextInstruction + REGD_OFFSET] = 0x1; 
   nextInstruction += NEXT_INS;
   newCPU.I_ROM[nextInstruction] = MOV;
   newCPU.I_ROM[nextInstruction + REGA_OFFSET] = 0x3; 
   newCPU.I_ROM[nextInstruction + REGD_OFFSET] = 0x2; 
   nextInstruction += NEXT_INS;
   newCPU.I_ROM[nextInstruction] = ADD;
   newCPU.I_ROM[nextInstruction + REGA_OFFSET] = 0x1; 
   newCPU.I_ROM[nextInstruction + REGB_OFFSET] = 0x2; 
   newCPU.I_ROM[nextInstruction + REGD_OFFSET] = 0x3;
   nextInstruction += NEXT_INS;
   newCPU.I_ROM[nextInstruction] = JMP;
   newCPU.I_ROM[nextInstruction + IMMA_OFFSET] = 12; 
   nextInstruction += NEXT_INS;
//
  Execute(50, & newCPU);
  EXP_GPR(4, (0x1 + 0x1), &newCPU, true);
  int garbage; scanf("%d", & garbage);
  return 0;
}