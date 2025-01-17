#include "main.h" //contains most undefined stuff here
#include "cpuDef.h"
#include "exp.h"

static inline void Setup(struct CPU * pCPU) {
  memset(pCPU, 0, sizeof(struct CPU));
}


static inline int ReadGPR(unsigned char GPR_SELECT, struct CPU * pCPU) {
  return pCPU -> GPRS[GPR_SELECT]; 
}


static inline int FetchByte(unsigned char ADDRESS, struct CPU * pCPU) {
  return pCPU -> I_ROM[ADDRESS]; 
}

static inline int WriteGPR(unsigned char GPR_SELECT, unsigned char VAL, struct CPU * pCPU) {
  pCPU -> GPRS[GPR_SELECT] = VAL; 
  return 0;
}

int Execute(size_t ticks, struct CPU * pCPU) {
  for (size_t i = 0; i < ticks; i++) {
    if (pCPU->halted == true) {
      return -1;
    }
    switch (pCPU -> I_ROM[pCPU -> PC]) {
    case HLT:
      pCPU -> halted = true;
      break;
    case UIN:
      printf("Enter user input: ");
      scanf("%c", & pCPU -> UI);
      break;
    case LDI:
      WriteGPR(FetchByte(pCPU -> PC + REGD_OFFSET, pCPU), 
      FetchByte(pCPU -> PC + IMMA_OFFSET, pCPU), pCPU);
      break;
    case MOV:
      WriteGPR(FetchByte(pCPU -> PC + REGD_OFFSET, pCPU),
      ReadGPR(pCPU -> PC + REGA_OFFSET, pCPU), pCPU);
      break;
    case STO:
      pCPU -> RAM[FetchByte(pCPU -> PC + IMMA_OFFSET, pCPU)] =
      FetchByte(pCPU -> PC + IMMB_OFFSET, pCPU);
      break;
    case LOAD:
      WriteGPR(FetchByte(pCPU -> PC + REGD_OFFSET, pCPU),
      pCPU -> RAM[FetchByte(pCPU -> PC + IMMA_OFFSET, pCPU)], pCPU);
      break;
    case ADD:
      WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU), 
      (ReadGPR(FetchByte(pCPU->PC + REGA_OFFSET, pCPU), pCPU) + 
      ReadGPR(FetchByte(pCPU->PC + REGB_OFFSET, pCPU), pCPU)), pCPU);
      break;
    case SUB:
      WriteGPR(FetchByte(pCPU -> PC + REGD_OFFSET, pCPU),
      (FetchByte(pCPU -> PC + REGA_OFFSET, pCPU) + FetchByte(pCPU -> PC - REGB_OFFSET, pCPU)), pCPU);
      break;
    case SWC:
      WriteGPR(FetchByte(pCPU -> PC + REGD_OFFSET, pCPU),
      (FetchByte(pCPU -> PC + REGA_OFFSET, pCPU) + FetchByte(pCPU -> PC + REGB_OFFSET, pCPU) + 1), pCPU);
      break;
    case AND:
      WriteGPR(FetchByte(pCPU -> PC + REGD_OFFSET, pCPU),
        (FetchByte(pCPU -> PC + REGA_OFFSET, pCPU) & FetchByte(pCPU -> PC + REGB_OFFSET, pCPU)), pCPU);
      break;
    case ORR:
      WriteGPR(FetchByte(pCPU -> PC + REGD_OFFSET, pCPU),
        (FetchByte(pCPU -> PC + REGA_OFFSET, pCPU) | FetchByte(pCPU -> PC + REGB_OFFSET, pCPU)), pCPU);
      break;
    case XOR:
      WriteGPR(FetchByte(pCPU -> PC + REGD_OFFSET, pCPU),
        (FetchByte(pCPU -> PC + REGA_OFFSET, pCPU) ^ FetchByte(pCPU -> PC + REGB_OFFSET, pCPU)), pCPU);
      break;
    case NOR:
      WriteGPR(FetchByte(pCPU -> PC + REGD_OFFSET, pCPU),
        ~(FetchByte(pCPU -> PC + REGA_OFFSET, pCPU) | FetchByte(pCPU -> PC + REGB_OFFSET, pCPU)), pCPU);
      break;
    //We don't care if the SP overflows or underflows.
    case PUSH:
      pCPU -> SP++;
      pCPU -> RAM[pCPU -> SP] = pCPU->GPRS[FetchByte(pCPU -> PC + REGA_OFFSET, pCPU)];
      break;
    case POP:
      WriteGPR(FetchByte(pCPU -> PC + REGD_OFFSET, pCPU),
      pCPU -> RAM[pCPU -> SP], pCPU);
      pCPU -> RAM[pCPU -> SP] = 0;
      pCPU -> SP--; 
      break;
    //
    case JMP:
      pCPU->PC = FetchByte(pCPU -> PC + IMMA_OFFSET, pCPU);
      break;
    case CJM:
      if (FetchByte(pCPU -> PC + IMMA_OFFSET, pCPU) == pCPU -> FR) {
        pCPU -> PC = (FetchByte(pCPU -> PC + IMMB_OFFSET, pCPU) - 1);
      }
      break;
    case SJM:
        pCPU->PC = (FetchByte(pCPU->PC + IMMB_OFFSET, pCPU) - NEXT_INS);
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
    default:
      printf("Unrecognized opcode at %d\n", (pCPU -> PC));
      break;
    }
    pCPU -> PC += NEXT_INS;
  }
  return 0;
}

int main() {
  struct CPU newCPU;
  Setup(& newCPU);
  //Push 0xf9 and pop to GPR2
  int nextInstruction = 0;
  newCPU.I_ROM[nextInstruction] = UIN;
  newCPU.I_ROM[nextInstruction+REGD_OFFSET] = 0x1;
  nextInstruction+=NEXT_INS;
  newCPU.I_ROM[nextInstruction] = PUSH;
  newCPU.I_ROM[nextInstruction+REGA_OFFSET] = 0x1;
  nextInstruction+=NEXT_INS;
  newCPU.I_ROM[nextInstruction] = POP;
  newCPU.I_ROM[nextInstruction+REGD_OFFSET] = 0x2;
  //
  Execute(3, & newCPU);
  EXP_GPR(2, 0xf9, & newCPU, true);
  int garbage; scanf("%d", & garbage);
  return 0;
}