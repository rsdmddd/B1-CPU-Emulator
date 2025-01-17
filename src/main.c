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
int Execute(size_t ticks, struct CPU *pCPU) {
    for (size_t i = 0; i < ticks; i++) {
      if (pCPU->halted == true) {
          return -1;
      }
      unsigned char result;
      switch (pCPU->I_ROM[pCPU->PC]) {
          case HLT:
              pCPU->halted = true;
              break;
          case UIN:
              printf("Enter user input: ");
              scanf("%c", &pCPU->UI);
              break;
          case LDI:
              // Load Immediate: Write an immediate value to GPR
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU), 
                      FetchByte(pCPU->PC + IMMA_OFFSET, pCPU), pCPU);
              break;
          case MOV:
              // Move: Copy value from one register to another
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU),
                      ReadGPR(FetchByte(pCPU->PC + REGA_OFFSET, pCPU), pCPU), pCPU);
              break;
          case STO:
              // Store: Store value from register into RAM
              pCPU->RAM[FetchByte(pCPU->PC + IMMA_OFFSET, pCPU)] =
                  FetchByte(pCPU->PC + IMMB_OFFSET, pCPU);
              break;
          case LOAD:
              // Load: Load value from RAM into register
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU),
                      pCPU->RAM[FetchByte(pCPU->PC + IMMA_OFFSET, pCPU)], pCPU);
              break;
          case ADD:
              // Add: Perform addition and store the result in destination register
              result = (ReadGPR(FetchByte(pCPU->PC + REGA_OFFSET, pCPU), pCPU) +
                        ReadGPR(FetchByte(pCPU->PC + REGB_OFFSET, pCPU), pCPU));
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU), result, pCPU);
              break;
          case SUB:
              // Subtract: Perform subtraction and store the result in destination register
              result = (ReadGPR(FetchByte(pCPU->PC + REGA_OFFSET, pCPU), pCPU) -
                        ReadGPR(FetchByte(pCPU->PC + REGB_OFFSET, pCPU), pCPU));
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU), result, pCPU);
              break;
          case SWC:
              // SWC (not defined, assuming it’s some form of modified subtraction operation)
              result = (ReadGPR(FetchByte(pCPU->PC + REGA_OFFSET, pCPU), pCPU) -
                        ReadGPR(FetchByte(pCPU->PC + REGB_OFFSET, pCPU), pCPU)) + 1;
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU), result, pCPU);
              break;
          case AND:
              // AND: Perform bitwise AND and store the result in destination register
              result = (ReadGPR(FetchByte(pCPU->PC + REGA_OFFSET, pCPU), pCPU) &
                        ReadGPR(FetchByte(pCPU->PC + REGB_OFFSET, pCPU), pCPU));
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU), result, pCPU);
              break;
          case ORR:
              // OR: Perform bitwise OR and store the result in destination register
              result = (ReadGPR(FetchByte(pCPU->PC + REGA_OFFSET, pCPU), pCPU) |
                        ReadGPR(FetchByte(pCPU->PC + REGB_OFFSET, pCPU), pCPU));
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU), result, pCPU);
              break;
          case XOR:
              // XOR: Perform bitwise XOR and store the result in destination register
              result = (ReadGPR(FetchByte(pCPU->PC + REGA_OFFSET, pCPU), pCPU) ^
                        ReadGPR(FetchByte(pCPU->PC + REGB_OFFSET, pCPU), pCPU));
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU), result, pCPU);
              break;
          case NOR:
              // NOR: Perform bitwise NOR and store the result in destination register
              result = ~(ReadGPR(FetchByte(pCPU->PC + REGA_OFFSET, pCPU), pCPU) |
                        ReadGPR(FetchByte(pCPU->PC + REGB_OFFSET, pCPU), pCPU));
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU), result, pCPU);
              break;
          case PUSH:
              // Push: Push value from register onto the stack (RAM)
              pCPU->SP++;
              pCPU->RAM[pCPU->SP] = pCPU->GPRS[FetchByte(pCPU->PC + REGA_OFFSET, pCPU)];
              break;
          case POP:
              // Pop: Pop value from stack (RAM) into a register
              WriteGPR(FetchByte(pCPU->PC + REGD_OFFSET, pCPU),
                      pCPU->RAM[pCPU->SP], pCPU);
              pCPU->RAM[pCPU->SP] = 0;
              pCPU->SP--;
              break;
          case JMP:
              // Jump: Set PC to immediate value
              pCPU->PC = FetchByte(pCPU->PC + IMMA_OFFSET, pCPU);
              break;
          case CJM:
              // Conditional Jump: Jump if flags match
              if (FetchByte(pCPU->PC + IMMA_OFFSET, pCPU) == pCPU->FR) {
                  pCPU->PC = (FetchByte(pCPU->PC + IMMB_OFFSET, pCPU) - 1);
              }
              break;
          case SJM:
              // Some form of jump modification (adjustment)
              pCPU->PC = (FetchByte(pCPU->PC + IMMB_OFFSET, pCPU) - NEXT_INS);
              break;
          case CALL:
              // Call: Push return address onto stack and jump
              pCPU->SP++;
              pCPU->RAM[pCPU->SP] = pCPU->PC + NEXT_INS;
              pCPU->PC = FetchByte(pCPU->PC + IMMA_OFFSET, pCPU) - NEXT_INS;
              break;
          case RET:
              // Return: Pop return address from stack and return to that point
              pCPU->PC = pCPU->RAM[pCPU->SP];
              pCPU->RAM[pCPU->SP] = 0;
              pCPU->SP--;
              pCPU->PC -= NEXT_INS;
              break;
          default:
              printf("Unrecognized opcode at %d\n", pCPU->PC);
              break;
        }
        printf("Result: %d\n", result);
        pCPU->PC += NEXT_INS;
    }
    return 0;
}

int main() {
  struct CPU newCPU;
  Setup(& newCPU);
  //Push 0xf9 and pop to GPR2
  int nextInstruction = 0;
  newCPU.I_ROM[nextInstruction] = LDI;
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