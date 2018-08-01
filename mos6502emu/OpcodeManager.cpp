#include "stdafx.h"
#include "OpcodeManager.h"

namespace mos6502emu {
	static inline cycles_t ExecuteOpcode(opcodes_t opcode) {
		switch (opcode) {

		// ADC
		case ADC_IMM: {
			return 2;
		}break;
		case ADC_ZERO: {
			return 3;
		}break;
		case ADC_ZERO_X: {
			return 4;
		}break;
		case ADC_ABS: {
			return 4;
		}break;
		case ADC_ABS_X: {
			// 4+
		}break;
		case ADC_ABS_Y: {
			// 4+
		}break;
		case ADC_IND_X: {
			return 6;
		}break;
		case ADC_IND_Y: {
			// 5+
		}break;


		// AND
		case AND_IMM: {
				   return 2;
		}break;
		case AND_ZERO: {
				 return 3;
		}break;
		case AND_ZERO_X: {
				  return 4;
		}break;
		case AND_ABS: {
				  return 4;
		}break;
		case AND_ABS_X: {
				 // 4+
		}break;
		case AND_ABS_Y: {
				 // 4+
		}break;
		case AND_IND_X: {
				   return 6;
		}break;
		case AND_IND_Y: {
				   // 5+
		}break;


		// ASL
		case ASL_ACC: {
				   return 2;
		}break;
		case ASL_ZERO: {
				 return 5;
		}break;
		case ASL_ZERO_X: {
				  return 6;
		}break;
		case ASL_ABS: {
					return 6;
		}break;
		case ASL_ABS_X: {
				 return 7;
		}break;


		// BIT
		case BIT_ZERO: {
				   return 3;
		}break;
		case BIT_ABS: {
				  return 4;
		}break;


		// Branch instructions
				 // 2 cycles + 1 if true, + 1 if crosses page boundary
		case BPL: {

		}break;
		case BMI: {

		}break;
		case BVC: {

		}break;
		case BVS: {

		}break;
		case BCC: {

		}break;
		case BCS: {

		}break;
		case BNE: {

		}break;
		case BEQ: {

		}break;


		// BREAK
		case BRK: {
			 return 7;
		}break;


		// CMP
		case CMP_IMM: {
			 return 2;
		}break;
		case CMP_ZERO: {
				 return 3;
		}break;
		case CMP_ZERO_X: {
				  return 4;
		}break;
		case CMP_ABS: {
					return 4;
		}break;
		case CMP_ABS_X: {
				 // 4+
		}break;
		case CMP_ABS_Y: {
				// 4+
		}break;
		case CMP_IND_X: {
				   return 6;
		}break;
		case CMP_IND_Y: {
				   // 5+
		}break;


		// CPX
		case CPX_IMM: {
				   return 2;
		}break;
		case CPX_ZERO: {
				 return 3;
		}break;
		case CPX_ABS: {
				  return 4;
		}break;



		// CPY
		case CPY_IMM: {
				 return 2;
		}break;
		case CPY_ZERO: {
				 return 3;
		}break;
		case CPY_ABS: {
				  return 4;
		}break;


		// DEC
		case DEC_ZERO: {
				 return 5;
		}break;
		case DEC_ZERO_X: {
				  return 6;
		}break;
		case DEC_ABS: {
					return 6;
		}break;
		case DEC_ABS_X: {
				 return 7;
		}break;


		// EOR
		case EOR_IMM: {
				   return 2;
		}break;
		case EOR_ZERO: {
				 return 3;
		}break;
		case EOR_ZERO_X: {
				  return 4;
		}break;
		case EOR_ABS: {
					return 4;
		}break;
		case EOR_ABS_X: {
				 // 4+
		}break;
		case EOR_ABS_Y: {
				   // 4+
		}break;
		case EOR_IND_X: {
				   return 6;
		}break;
		case EOR_IND_Y: {
				   // 5+
		}break;


		// Flag (Processor Status) Instructions
		case CLC: {
			return 2;
		}break;
		case SEC: {
			return 2;
		}break;
		case CLI: {
			return 2;
		}break;
		case SEI: {
			return 2;
		}break;
		case CLV: {
			return 2;
		}break;
		case CLD: {
			return 2;
		}break;
		case SED: {
			return 2;
		}break;


		// INC
		case INC_ZERO: {
			return 5;
		}break;
		case INC_ZERO_X: {
			return 6;
		}break;
		case INC_ABS: {
					return 6;
		}break;
		case INC_ABS_X: {
				 return 7;
		}break;


		// JMP
		case JMP_ABS: {
				   return 3;
		}break;
		case JMP_IND: {
				 return 5;
		}break;


		// JSR
		case JSR_ABS: {
				 return 6;
		}break;


		// LDA
		case LDA_IMM: {
				 return 2;
		}break;
		case LDA_ZERO: {
				 return 3;
		}break;
		case LDA_ZERO_X: {
				  return 4;
		}break;
		case LDA_ABS: {
					return 4;
		}break;
		case LDA_ABS_X: {
				 // 4+
		}break;
		case LDA_ABS_Y: {
				// 4+
		}break;
		case LDA_IND_X: {
				   return 6;
		}break;
		case LDA_IND_Y: {
				// 5+
		}break;


		// LDX
		case LDX_IMM: {
				   return 2;
		}break;
		case LDX_ZERO: {
				 return 3;
		}break;
		case LDX_ZERO_Y: {
				  return 4;
		}break;
		case LDX_ABS: {
					return 4;
		}break;
		case LDX_ABS_Y: {
				 // 4+
		}break;


		// LDY
		case LDY_IMM: {
				   return 2;
		}break;
		case LDY_ZERO: {
				 return 3;
		}break;
		case LDY_ZERO_X: {
				  return 4;
		}break;
		case LDY_ABS: {
					return 4;
		}break;
		case LDY_ABS_X: {
				// 4+
		}break;


		// LSR
		case LSR_ACC: {
				   return 2;
		}break;
		case LSR_ZERO: {
				 return 5;
		}break;
		case LSR_ZERO_X: {
				  return 6;
		}break;
		case LSR_ABS: {
					return 6;
		}break;
		case LSR_ABS_X: {
				 return 7;
		}break;


		// NOP
		case NOP: {
				   return 2;
		}break;


		// ORA
		case ORA_IMM: {
			 return 2;
		}break;
		case ORA_ZERO: {
				 return 3;
		}break;
		case ORA_ZERO_X: {
				  return 4;
		}break;
		case ORA_ABS: {
					return 4;
		}break;
		case ORA_ABS_X: {
				 // 4+
		}break;
		case ORA_ABS_Y: {
				   // 4+
		}break;
		case ORA_IND_X: {
				   return 6;
		}break;
		case ORA_IND_Y: {
				   // 5+
		}break;


		// Register Instructions 
		case TAX: {
				   return 2;
		}break;
		case TXA: {
			 return 2;
		}break;
		case DEX: {
			 return 2;
		}break;
		case INX: {
			 return 2;
		}break;
		case TAY: {
			 return 2;
		}break;
		case TYA: {
			 return 2;
		}break;
		case DEY: {
			 return 2;
		}break;
		case INY: {
			 return 2;
		}break;


		// ROL
		case ROL_ACC: {
			 return 2;
		}break;
		case ROL_ZERO: {
				 return 5;
		}break;
		case ROL_ZERO_X: {
				  return 6;
		}break;
		case ROL_ABS: {
					return 6;
		}break;
		case ROL_ABS_X: {
				 return 7;
		}break;


		// ROR
		case ROR_ACC: {
				   return 2;
		}break;
		case ROR_ZERO: {
				 return 5;
		}break;
		case ROR_ZERO_X: {
				  return 6;
		}break;
		case ROR_ABS: {
					return 6;
		}break;
		case ROR_ABS_X: {
				 return 7;
		}break;


		// RTI
		case RTI_IMP: {
				   return 6;
		}break;


		// RTS
		case RTS: {
				 return 6;
		}break;


		// SBC
		case SBC_IMM: {
			 return 2;
		}break;
		case SBC_ZERO: {
				 return 3;
		}break;
		case SBC_ZERO_X: {
				  return 4;
		}break;
		case SBC_ABS: {
					return 4;
		}break;
		case SBC_ABS_X: {
				 // 4+
		}break;
		case SBC_ABS_Y: {
				   // 4+
		}break;
		case SBC_IND_X: {
				   return 6;
		}break;
		case SBC_IND_Y: {
				   // 5+
		}break;


		// STA
		case STA_ZERO: {
				   return 3;
		}break;
		case STA_ZERO_X: {
				  return 4;
		}break;
		case STA_ABS: {
					return 4;
		}break;
		case STA_ABS_X: {
				 return 5;
		}break;
		case STA_ABS_Y: {
				   return 5;
		}break;
		case STA_IND_X: {
				   return 6;
		}break;
		case STA_IND_Y: {
				   return 6;
		}break;


		// Stack Instructions
		case TXS: {
				   return 2;
		}break;
		case TSX: {
			 return 2;
		}break;
		case PHA: {
			 return 3;
		}break;
		case PLA: {
			 return 4;
		}break;
		case PHP: {
			 return 3;
		}break;
		case PLP: {
			 return 4;
		}break;



		// STX
		case STX_ZERO: {
			 return 3;
		}break;
		case STX_ZERO_Y: {
				  return 4;
		}break;
		case STX_ABS: {
					return 4;
		}break;


		// STY
		case STY_ZERO: {
				 return 3;
		}break;
		case STY_ZERO_X: {
				  return 4;
		}break;
		case STY_ABS: {
					return 4;
		}break;


		// illegal instructions, for now, do nothing.
		default: {
				
		}break;
		}


		LOG("Error, incorrect opcode cycle number returned!");
		return 0;
	}

	


}






