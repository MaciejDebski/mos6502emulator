#include "stdafx.h"
#include "mos6502emu.h"
#include "Opcodes_mos6502.h"
#include "CPU.h"

namespace mos6502emu {
	static inline void SetFlagsNZ(Word8bit result) {
		CPU::Status.N = (result & 0x80) >> 7;
		CPU::Status.Z = (result == 0);
	}

	static inline void ADC(Word16bit data) {
		Word8bit A = CPU::Reg.A;
		Word16bit sum = (CPU::Reg.A = (A + (data & 0xFF)));
		
		CPU::Status.C = (sum > 255);
		CPU::Status.V = ((A^sum)&(data^sum) & 0x80) >> 7;
		
		SetFlagsNZ(sum);
	}

	static inline void ROL(Word8bit address) {
		Word8bit c = CPU::Status.C;
		CPU::Status.C = CPU::Memory[address] & 0x80 >> 7;
		CPU::Memory[address] = CPU::Memory[address] << 1;
		PasteBit(CPU::Memory[address], 0, c);
	}

	static inline void ROR(Word8bit address) {
		Word8bit c = CPU::Status.C;
		CPU::Status.C = CPU::Memory[address] & 0x1;
		CPU::Memory[address] = CPU::Memory[address] >> 1;
		PasteBit(CPU::Memory[address], 7, c);
	}

	static inline void SBC(Word8bit data) {
		Word8bit A = CPU::Reg.A;
		CPU::Status.C = (data <= A);

		Word8bit diff = (CPU::Reg.A = (A + ((~data) & 0xFF)));
		CPU::Status.V = ((A^diff)&((~data)^diff) & 0x80) >> 7;

		SetFlagsNZ(diff);
	}

	CyclesUsed ExecuteOpcode(Word8bit opcode) {
		switch (opcode) {

			// ADC
		case ADC_IMM: {
			ADC(CPU::Read_PC());
			return 2;
		}break;
		case ADC_ZERO: {
			ADC(CPU::Read_ZERO());
			return 3;
		}break;
		case ADC_ZERO_X: {
			ADC(CPU::Read_ZERO_X());
			return 4;
		}break;
		case ADC_ABS: {
			ADC(CPU::Read_ABS());
			return 4;
		}break;
		case ADC_ABS_X: {
			ADC(CPU::Read_ABS_X());
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case ADC_ABS_Y: {
			ADC(CPU::Read_ABS_Y());
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case ADC_IND_X: {
			ADC(CPU::Read_IND_X());
			return 6;
		}break;
		case ADC_IND_Y: {
			ADC(CPU::Read_IND_Y());
			return 5 + CPU::PageBoundaryCrossed();
		}break;


			// AND
		case AND_IMM: {
			SetFlagsNZ(CPU::Reg.A &= CPU::Memory[++CPU::Reg.PC]);
			return 2;
		}break;
		case AND_ZERO: {
			SetFlagsNZ(CPU::Reg.A &= CPU::Read_ZERO());
			return 3;
		}break;
		case AND_ZERO_X: {
			SetFlagsNZ(CPU::Reg.A &= CPU::Read_ZERO_X());
			return 4;
		}break;
		case AND_ABS: {
			SetFlagsNZ(CPU::Reg.A &= CPU::Read_ABS());
			return 4;
		}break;
		case AND_ABS_X: {
			SetFlagsNZ(CPU::Reg.A &= CPU::Read_ABS_X());
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case AND_ABS_Y: {
			SetFlagsNZ(CPU::Reg.A &= CPU::Read_ABS_Y());
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case AND_IND_X: {
			SetFlagsNZ(CPU::Reg.A &= CPU::Read_IND_X());
			return 6;
		}break;
		case AND_IND_Y: {
			SetFlagsNZ(CPU::Reg.A &= CPU::Read_IND_Y());
			return 5 + CPU::PageBoundaryCrossed();
		}break;


			// ASL
		case ASL_ACC: {
			CPU::Status.C = (CPU::Reg.A & 0x80) >> 7;
			SetFlagsNZ(CPU::Reg.A = CPU::Reg.A << 1);
			return 2;
		}break;
		case ASL_ZERO: {
			Word16bit address = CPU::GetAddr_ZERO();
			CPU::Status.C = (CPU::Memory[address] & 0x80) >> 7;
			SetFlagsNZ(CPU::Memory[address] = CPU::Memory[address] << 1);
			return 5;
		}break;
		case ASL_ZERO_X: {
			Word16bit address = CPU::GetAddr_ZERO_X();
			CPU::Status.C = (CPU::Memory[address] & 0x80) >> 7;
			SetFlagsNZ(CPU::Memory[address] = CPU::Memory[address] << 1);
			return 6;
		}break;
		case ASL_ABS: {
			Word16bit address = CPU::GetAddr_ABS();
			CPU::Status.C = (CPU::Memory[address] & 0x80) >> 7;
			SetFlagsNZ(CPU::Memory[address] = CPU::Memory[address] << 1);
			return 6;
		}break;
		case ASL_ABS_X: {
			Word16bit address = CPU::GetAddr_ABS_X();
			CPU::Status.C = (CPU::Memory[address] & 0x80) >> 7;
			SetFlagsNZ(CPU::Memory[address] = CPU::Memory[address] << 1);
			return 7;
		}break;


			// BIT
		case BIT_ZERO: {
			Word8bit result = CPU::Reg.A & CPU::Read_ZERO();
			SetFlagsNZ(result);
			CPU::Status.V = (result & 0x40) >> 6;
			return 3;
		}break;
		case BIT_ABS: {
			Word8bit result = CPU::Reg.A & CPU::Read_ABS();
			SetFlagsNZ(result);
			CPU::Status.V = (result & 0x40) >> 6;
			return 4;
		}break;


			// Branch instructions
			// 2 cycles + 1 if true, + 1 if crosses page boundary
		case BPL: {
			if (CPU::Status.N) {
				// branch false
				++CPU::Reg.PC;
				return 2;
			}
			// branch true
			CPU::Reg.PC = CPU::GetAddr_REL();
			return 3 + CPU::PageBoundaryCrossed();
		}break;
		case BMI: {
			if (CPU::Status.N) {
				// branch true
				CPU::Reg.PC = CPU::GetAddr_REL();
				return 3 + CPU::PageBoundaryCrossed();
			}
			// branch false
			++CPU::Reg.PC;
			return 2;
		}break;
		case BVC: {
			if (CPU::Status.V) {
				// branch false
				++CPU::Reg.PC;
				return 2;
			}
			// branch true
			CPU::Reg.PC = CPU::GetAddr_REL();
			return 3 + CPU::PageBoundaryCrossed();
		}break;
		case BVS: {
			if (CPU::Status.V) {
				// branch true
				CPU::Reg.PC = CPU::GetAddr_REL();
				return 3 + CPU::PageBoundaryCrossed();
			}
			// branch false
			++CPU::Reg.PC;
			return 2;
		}break;
		case BCC: {
			if (CPU::Status.C) {
				// branch false
				++CPU::Reg.PC;
				return 2;
			}
			// branch true
			CPU::Reg.PC = CPU::GetAddr_REL();
			return 3 + CPU::PageBoundaryCrossed();
		}break;
		case BCS: {
			if (CPU::Status.C) {
				// branch true
				CPU::Reg.PC = CPU::GetAddr_REL();
				return 3 + CPU::PageBoundaryCrossed();
			}
			// branch false
			++CPU::Reg.PC;
			return 2;
		}break;
		case BNE: {
			if (CPU::Status.Z) {
				// branch false
				++CPU::Reg.PC;
				return 2;
			}
			// branch true
			CPU::Reg.PC = CPU::GetAddr_REL();
			return 3 + CPU::PageBoundaryCrossed();
		}break;
		case BEQ: {
			if (CPU::Status.Z) {
				// branch true
				CPU::Reg.PC = CPU::GetAddr_REL();
				return 3 + CPU::PageBoundaryCrossed();
			}
			// branch false
			++CPU::Reg.PC;
			return 2;
		}break;


			// BREAK
		case BRK: {
			// TODO: BRK
			return 7;
		}break;


			// CMP
		case CMP_IMM: {
			//Word8bit result = CPU::ProcessorRegisters1.A - CPU::Memory[++CPU::ProcessorRegisters1.PC];
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
			SetFlagsNZ(--CPU::Memory[CPU::GetAddr_ZERO()]);
			return 5;
		}break;
		case DEC_ZERO_X: {
			SetFlagsNZ(--CPU::Memory[CPU::GetAddr_ZERO_X()]);
			return 6;
		}break;
		case DEC_ABS: {
			SetFlagsNZ(--CPU::Memory[CPU::GetAddr_ABS()]);
			return 6;
		}break;
		case DEC_ABS_X: {
			SetFlagsNZ(--CPU::Memory[CPU::GetAddr_ABS_X()]);
			return 7;
		}break;


			// EOR
		case EOR_IMM: {
			SetFlagsNZ( CPU::Reg.A ^= CPU::Read_PC() );
			return 2;
		}break;
		case EOR_ZERO: {
			SetFlagsNZ(CPU::Reg.A ^= CPU::Read_ZERO());
			return 3;
		}break;
		case EOR_ZERO_X: {
			SetFlagsNZ(CPU::Reg.A ^= CPU::Read_ZERO_X());
			return 4;
		}break;
		case EOR_ABS: {
			SetFlagsNZ(CPU::Reg.A ^= CPU::Read_ABS());
			return 4;
		}break;
		case EOR_ABS_X: {
			SetFlagsNZ(CPU::Reg.A ^= CPU::Read_ABS_X());
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case EOR_ABS_Y: {
			SetFlagsNZ(CPU::Reg.A ^= CPU::Read_ABS_Y());
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case EOR_IND_X: {
			SetFlagsNZ(CPU::Reg.A ^= CPU::Read_IND_X());
			return 6;
		}break;
		case EOR_IND_Y: {
			SetFlagsNZ(CPU::Reg.A ^= CPU::Read_IND_Y());
			return 5 + CPU::PageBoundaryCrossed();
		}break;


			// Flag (Processor Status) Instructions
		case CLC: {
			CPU::Status.C = 0;
			return 2;
		}break;
		case SEC: {
			CPU::Status.C = 1;
			return 2;
		}break;
		case CLI: {
			CPU::Status.I = 0;
			return 2;
		}break;
		case SEI: {
			CPU::Status.I = 1;
			return 2;
		}break;
		case CLV: {
			CPU::Status.V = 0;
			return 2;
		}break;
		case CLD: {
			CPU::Status.D = 0;
			return 2;
		}break;
		case SED: {
			CPU::Status.D = 1;
			return 2;
		}break;


			// INC
		case INC_ZERO: {
			SetFlagsNZ(++CPU::Memory[CPU::GetAddr_ZERO()]);
			return 5;
		}break;
		case INC_ZERO_X: {
			SetFlagsNZ(++CPU::Memory[CPU::GetAddr_ZERO_X()]);
			return 6;
		}break;
		case INC_ABS: {
			SetFlagsNZ(++CPU::Memory[CPU::GetAddr_ABS()]);
			return 6;
		}break;
		case INC_ABS_X: {
			SetFlagsNZ(++CPU::Memory[CPU::GetAddr_ABS_X()]);
			return 7;
		}break;


			// JMP
		case JMP_ABS: {
			CPU::Reg.PC = CPU::GetAddr_ABS();
			return 3;
		}break;
		case JMP_IND: {
			CPU::Reg.PC = CPU::GetAddr_IND();
			return 5;
		}break;


			// JSR
		case JSR_ABS: {
			// TODO: JSR
			return 6;
		}break;


			// LDA
		case LDA_IMM: {
			SetFlagsNZ(CPU::Reg.A = CPU::Read_PC());
			return 2;
		}break;
		case LDA_ZERO: {
			SetFlagsNZ(CPU::Reg.A = CPU::Read_ZERO());
			return 3;
		}break;
		case LDA_ZERO_X: {
			SetFlagsNZ(CPU::Reg.A = CPU::Read_ZERO_X());
			return 4;
		}break;
		case LDA_ABS: {
			SetFlagsNZ(CPU::Reg.A = CPU::Read_ABS());
			return 4;
		}break;
		case LDA_ABS_X: {
			SetFlagsNZ(CPU::Reg.A = CPU::Read_ABS_X());
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case LDA_ABS_Y: {
			SetFlagsNZ(CPU::Reg.A = CPU::Read_ABS_Y());
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case LDA_IND_X: {
			SetFlagsNZ(CPU::Reg.A = CPU::Read_IND_X());
			return 6;
		}break;
		case LDA_IND_Y: {
			SetFlagsNZ(CPU::Reg.A = CPU::Read_IND_Y());
			return 5 + CPU::PageBoundaryCrossed();
		}break;


			// LDX
		case LDX_IMM: {
			SetFlagsNZ(CPU::Reg.X = CPU::Read_PC());
			return 2;
		}break;
		case LDX_ZERO: {
			SetFlagsNZ(CPU::Reg.X = CPU::Read_ZERO());
			return 3;
		}break;
		case LDX_ZERO_Y: {
			SetFlagsNZ(CPU::Reg.X = CPU::Read_ZERO_Y());
			return 4;
		}break;
		case LDX_ABS: {
			SetFlagsNZ(CPU::Reg.X = CPU::Read_ABS());
			return 4;
		}break;
		case LDX_ABS_Y: {
			SetFlagsNZ(CPU::Reg.X = CPU::Read_ABS_Y());
			return 4 + CPU::PageBoundaryCrossed();
		}break;


			// LDY
		case LDY_IMM: {
			SetFlagsNZ(CPU::Reg.Y = CPU::Read_PC());
			return 2;
		}break;
		case LDY_ZERO: {
			SetFlagsNZ(CPU::Reg.Y = CPU::Read_ZERO());
			return 3;
		}break;
		case LDY_ZERO_X: {
			SetFlagsNZ(CPU::Reg.Y = CPU::Read_ZERO_X());
			return 4;
		}break;
		case LDY_ABS: {
			SetFlagsNZ(CPU::Reg.Y = CPU::Read_ABS());
			return 4;
		}break;
		case LDY_ABS_X: {
			SetFlagsNZ(CPU::Reg.Y = CPU::Read_ABS_X());
			return 4 + CPU::PageBoundaryCrossed();
		}break;


			// LSR
		case LSR_ACC: {
			CPU::Status.C = (CPU::Reg.A & 0x1);
			SetFlagsNZ(CPU::Reg.A = CPU::Reg.A >> 1);
			return 2;
		}break;
		case LSR_ZERO: {
			Word16bit address = CPU::GetAddr_ZERO();
			CPU::Status.C = (CPU::Memory[address] & 0x1);
			SetFlagsNZ(CPU::Memory[address] = CPU::Memory[address] >> 1);
			return 5;
		}break;
		case LSR_ZERO_X: {
			Word16bit address = CPU::GetAddr_ZERO_X();
			CPU::Status.C = (CPU::Memory[address] & 0x1);
			SetFlagsNZ(CPU::Memory[address] = CPU::Memory[address] >> 1);
			return 6;
		}break;
		case LSR_ABS: {
			Word16bit address = CPU::GetAddr_ABS();
			CPU::Status.C = (CPU::Memory[address] & 0x1);
			SetFlagsNZ(CPU::Memory[address] = CPU::Memory[address] >> 1);
			return 6;
		}break;
		case LSR_ABS_X: {
			Word16bit address = CPU::GetAddr_ABS_X();
			CPU::Status.C = (CPU::Memory[address] & 0x1);
			SetFlagsNZ(CPU::Memory[address] = CPU::Memory[address] >> 1);
			return 7;
		}break;


			// NOP
		case NOP: {
			// Do nothing. No need of incrementing the PC as NOP has no address bytes in front of it. The next Tick() will increment the PC to the next instruction.
			return 2;
		}break;


			// ORA
		case ORA_IMM: {
			SetFlagsNZ(CPU::Reg.A |= CPU::Read_PC());
			return 2;
		}break;
		case ORA_ZERO: {
			SetFlagsNZ(CPU::Reg.A |= CPU::Read_ZERO());
			return 3;
		}break;
		case ORA_ZERO_X: {
			SetFlagsNZ(CPU::Reg.A |= CPU::Read_ZERO_X());
			return 4;
		}break;
		case ORA_ABS: {
			SetFlagsNZ(CPU::Reg.A |= CPU::Read_ABS());
			return 4;
		}break;
		case ORA_ABS_X: {
			SetFlagsNZ(CPU::Reg.A |= CPU::Read_ABS_X());
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case ORA_ABS_Y: {
			SetFlagsNZ(CPU::Reg.A |= CPU::Read_ABS_Y());
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case ORA_IND_X: {
			SetFlagsNZ(CPU::Reg.A |= CPU::Read_IND_X());
			return 6;
		}break;
		case ORA_IND_Y: {
			SetFlagsNZ(CPU::Reg.A |= CPU::Read_IND_Y());
			return 5 + CPU::PageBoundaryCrossed();
		}break;


			// Register Instructions 
		case TAX: {
			SetFlagsNZ(CPU::Reg.X = CPU::Reg.A);
			return 2;
		}break;
		case TXA: {
			SetFlagsNZ(CPU::Reg.A = CPU::Reg.X);
			return 2;
		}break;
		case DEX: {
			SetFlagsNZ(--CPU::Reg.X);
			return 2;
		}break;
		case INX: {
			SetFlagsNZ(++CPU::Reg.X);
			return 2;
		}break;
		case TAY: {
			SetFlagsNZ(CPU::Reg.Y = CPU::Reg.A);
			return 2;
		}break;
		case TYA: {
			SetFlagsNZ(CPU::Reg.A = CPU::Reg.Y);
			return 2;
		}break;
		case DEY: {
			SetFlagsNZ(--CPU::Reg.Y);
			return 2;
		}break;
		case INY: {
			SetFlagsNZ(++CPU::Reg.Y);
			return 2;
		}break;


			// ROL
		case ROL_ACC: {
			Word8bit c = CPU::Status.C;
			CPU::Status.C = CPU::Reg.A & 0x80 >> 7;
			CPU::Reg.A = CPU::Reg.A << 1;
			PasteBit(CPU::Reg.A, 0, c);
			return 2;
		}break;
		case ROL_ZERO: {
			ROL(CPU::GetAddr_ZERO());
			return 5;
		}break;
		case ROL_ZERO_X: {
			ROL(CPU::GetAddr_ZERO_X());
			return 6;
		}break;
		case ROL_ABS: {
			ROL(CPU::GetAddr_ABS());
			return 6;
		}break;
		case ROL_ABS_X: {
			ROL(CPU::GetAddr_ABS_X());
			return 7;
		}break;


			// ROR
		case ROR_ACC: {
			Word8bit c = CPU::Status.C;
			CPU::Status.C = CPU::Reg.A & 0x1;
			CPU::Reg.A = CPU::Reg.A >> 1;
			PasteBit(CPU::Reg.A, 7, c);
			return 2;
		}break;
		case ROR_ZERO: {
			ROR(CPU::GetAddr_ZERO());
			return 5;
		}break;
		case ROR_ZERO_X: {
			ROR(CPU::GetAddr_ZERO_X());
			return 6;
		}break;
		case ROR_ABS: {
			ROR(CPU::GetAddr_ABS());
			return 6;
		}break;
		case ROR_ABS_X: {
			ROR(CPU::GetAddr_ABS_X());
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
			SBC(CPU::Read_PC());
			return 2;
		}break;
		case SBC_ZERO: {
			SBC(CPU::Read_ZERO());
			return 3;
		}break;
		case SBC_ZERO_X: {
			SBC(CPU::Read_ZERO_X());
			return 4;
		}break;
		case SBC_ABS: {
			SBC(CPU::Read_ABS());
			return 4;
		}break;
		case SBC_ABS_X: {
			SBC(CPU::Read_ABS_X());
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case SBC_ABS_Y: {
			SBC(CPU::Read_ABS_Y());
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case SBC_IND_X: {
			SBC(CPU::Read_IND_X());
			return 6;
		}break;
		case SBC_IND_Y: {
			SBC(CPU::Read_IND_Y());
			return 5 + CPU::PageBoundaryCrossed();
		}break;


			// STA
		case STA_ZERO: {
			CPU::Memory[CPU::GetAddr_ZERO()] = CPU::Reg.A;
			return 3;
		}break;
		case STA_ZERO_X: {
			CPU::Memory[CPU::GetAddr_ZERO_X()] = CPU::Reg.A;
			return 4;
		}break;
		case STA_ABS: {
			CPU::Memory[CPU::GetAddr_ABS()] = CPU::Reg.A;
			return 4;
		}break;
		case STA_ABS_X: {
			CPU::Memory[CPU::GetAddr_ABS_X()] = CPU::Reg.A;
			return 5;
		}break;
		case STA_ABS_Y: {
			CPU::Memory[CPU::GetAddr_ABS_Y()] = CPU::Reg.A;
			return 5;
		}break;
		case STA_IND_X: {
			CPU::Memory[CPU::GetAddr_IND_X()] = CPU::Reg.A;
			return 6;
		}break;
		case STA_IND_Y: {
			CPU::Memory[CPU::GetAddr_IND_Y()] = CPU::Reg.A;
			return 6;
		}break;


			// Stack Instructions
		case TXS: {
			CPU::Reg.SP = CPU::Reg.X;
			return 2;
		}break;
		case TSX: {
			SetFlagsNZ(CPU::Reg.X = CPU::Reg.SP);
			return 2;
		}break;
		case PHA: {
			// TODO: PHA
			return 3;
		}break;
		case PLA: {
			// TODO: PLA
			return 4;
		}break;
		case PHP: {
			// TODO: PHP
			return 3;
		}break;
		case PLP: {
			// TODO: PLP
			return 4;
		}break;



			// STX
		case STX_ZERO: {
			CPU::Memory[CPU::GetAddr_ZERO()] = CPU::Reg.X;
			return 3;
		}break;
		case STX_ZERO_Y: {
			CPU::Memory[CPU::GetAddr_ZERO_Y()] = CPU::Reg.X;
			return 4;
		}break;
		case STX_ABS: {
			CPU::Memory[CPU::GetAddr_ABS()] = CPU::Reg.X;
			return 4;
		}break;


			// STY
		case STY_ZERO: {
			CPU::Memory[CPU::GetAddr_ZERO()] = CPU::Reg.Y;
			return 3;
		}break;
		case STY_ZERO_X: {
			CPU::Memory[CPU::GetAddr_ZERO_X()] = CPU::Reg.Y;
			return 4;
		}break;
		case STY_ABS: {
			CPU::Memory[CPU::GetAddr_ABS()] = CPU::Reg.Y;
			return 4;
		}break;


		default: {
			// illegal instructions, for now, do nothing. Same as NOP.
			LOG("Opcode not supported by this emulator: %x", opcode);
			// return 1 to simply push the CPU forward, doing nothing. 1 cycle might not be true for illegal opcodes but its true when there is no ROM and user is trying to run the CPU.
			return 1;
		}break;
		}

	}

}




