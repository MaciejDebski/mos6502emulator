#include "stdafx.h"
#include "mos6502emu.h"
#include "Opcodes_mos6502.h"
#include "CPU.h"

namespace mos6502emu {
	static inline void SetFlagsNZ(Fast8bit result) {
		CPU::Status.N = (result & 0x80) >> 7;
		CPU::Status.Z = ((result & 0xFF) == 0);
	}

	static inline void ADC(Fast8bit data) {
		Word8bit A = CPU::Reg.A;
		Fast16bit sum = (CPU::Reg.A = (A + (data & 0xFF)));
		
		CPU::Status.C = (sum > 255);
		CPU::Status.V = ((A^sum)&(data^sum) & 0x80) >> 7;
		
		SetFlagsNZ(sum);
	}

	static inline void ASL(Word16bit address) {
		Fast8bit value = Memory[address].Read();
		CPU::Status.C = (value & 0x80) >> 7;
		SetFlagsNZ(value <<= 1);
		Memory[address].Write(value);
	}

	static inline void LSR(Word16bit address) {
		Fast8bit value = Memory[address].Read();
		CPU::Status.C = (value & 0x1);
		SetFlagsNZ(value >>= 1);
		Memory[address].Write(value);
	}

	static inline void ROL(Word16bit address) {
		Fast8bit c = CPU::Status.C;
		Fast8bit value = Memory[address].Read();
		CPU::Status.C = value & 0x80 >> 7;
		Memory[address].Write(PASTE_BIT(value << 1, 0, c));
	}

	static inline void ROR(Word16bit address) {
		Fast8bit c = CPU::Status.C;
		Fast8bit value = Memory[address].Read();
		CPU::Status.C = value; // no need of clearing MSBits (Memory[address] & 0x01), the expression needs LSB in this case.
		Memory[address].Write(PASTE_BIT(value >> 1, 7, c));
	}

	static inline void SBC(Fast8bit data) {
		Fast8bit A = CPU::Reg.A & 0xFF;
		CPU::Status.C = (data <= A);

		Fast8bit diff = (CPU::Reg.A = (A + ((~data) & 0xFF)));
		CPU::Status.V = ((A^diff)&((~data)^diff) & 0x80) >> 7;

		SetFlagsNZ(diff);
	}

	CyclesUsed ExecuteOpcode(Fast8bit opcode) {
		LOG("%hhX, ", opcode);
		switch (opcode) {

			// ADC
		case ADC_IMM: {
			ADC(CPU::Deref_IMM());
			++CPU::Reg.PC;
			return 2;
		}break;
		case ADC_ZERO: {
			ADC(CPU::Deref_ZERO());
			++CPU::Reg.PC;
			return 3;
		}break;
		case ADC_ZERO_X: {
			ADC(CPU::Deref_ZERO_X());
			++CPU::Reg.PC;
			return 4;
		}break;
		case ADC_ABS: {
			ADC(CPU::Deref_ABS());
			++CPU::Reg.PC;
			return 4;
		}break;
		case ADC_ABS_X: {
			ADC(CPU::Deref_ABS_X());
			++CPU::Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case ADC_ABS_Y: {
			ADC(CPU::Deref_ABS_Y());
			++CPU::Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case ADC_IND_X: {
			ADC(CPU::Deref_IND_X());
			++CPU::Reg.PC;
			return 6;
		}break;
		case ADC_IND_Y: {
			ADC(CPU::Deref_IND_Y());
			++CPU::Reg.PC;
			return 5 + CPU::PageBoundaryCrossed();
		}break;


			// AND
		case AND_IMM: {
			SetFlagsNZ(CPU::Reg.A &= CPU::Deref_IMM());
			++CPU::Reg.PC;
			return 2;
		}break;
		case AND_ZERO: {
			SetFlagsNZ(CPU::Reg.A &= CPU::Deref_ZERO());
			++CPU::Reg.PC;
			return 3;
		}break;
		case AND_ZERO_X: {
			SetFlagsNZ(CPU::Reg.A &= CPU::Deref_ZERO_X());
			++CPU::Reg.PC;
			return 4;
		}break;
		case AND_ABS: {
			SetFlagsNZ(CPU::Reg.A &= CPU::Deref_ABS());
			++CPU::Reg.PC;
			return 4;
		}break;
		case AND_ABS_X: {
			SetFlagsNZ(CPU::Reg.A &= CPU::Deref_ABS_X());
			++CPU::Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case AND_ABS_Y: {
			SetFlagsNZ(CPU::Reg.A &= CPU::Deref_ABS_Y());
			++CPU::Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case AND_IND_X: {
			SetFlagsNZ(CPU::Reg.A &= CPU::Deref_IND_X());
			++CPU::Reg.PC;
			return 6;
		}break;
		case AND_IND_Y: {
			SetFlagsNZ(CPU::Reg.A &= CPU::Deref_IND_Y());
			++CPU::Reg.PC;
			return 5 + CPU::PageBoundaryCrossed();
		}break;


			// ASL
		case ASL_ACC: {
			CPU::Status.C = (CPU::Reg.A & 0x80) >> 7;
			SetFlagsNZ(CPU::Reg.A = CPU::Reg.A << 1);
			++CPU::Reg.PC;
			return 2;
		}break;
		case ASL_ZERO: {
			ASL(CPU::GetAddr_ZERO());
			++CPU::Reg.PC;
			return 5;
		}break;
		case ASL_ZERO_X: {
			ASL(CPU::GetAddr_ZERO_X());
			++CPU::Reg.PC;
			return 6;
		}break;
		case ASL_ABS: {
			ASL(CPU::GetAddr_ABS());
			++CPU::Reg.PC;
			return 6;
		}break;
		case ASL_ABS_X: {
			ASL(CPU::GetAddr_ABS_X());
			++CPU::Reg.PC;
			return 7;
		}break;


			// BIT
		case BIT_ZERO: {
			Fast8bit result = CPU::Reg.A & CPU::Deref_ZERO();
			SetFlagsNZ(result);
			CPU::Status.V = (result & 0x40) >> 6;
			++CPU::Reg.PC;
			return 3;
		}break;
		case BIT_ABS: {
			Fast8bit result = CPU::Reg.A & CPU::Deref_ABS();
			SetFlagsNZ(result);
			CPU::Status.V = (result & 0x40) >> 6;
			++CPU::Reg.PC;
			return 4;
		}break;


			// Branch instructions
			// 2 cycles + 1 if true, + 1 if crosses page boundary
		case BPL: {
			if (CPU::Status.N) {
				// branch false
				CPU::Reg.PC += 2;
				return 2;
			}
			// branch true
			CPU::Reg.PC = CPU::GetAddr_REL();
			++CPU::Reg.PC;
			return 3 + CPU::PageBoundaryCrossed();
		}break;
		case BMI: {
			if (CPU::Status.N) {
				// branch true
				CPU::Reg.PC = CPU::GetAddr_REL();
				++CPU::Reg.PC;
				return 3 + CPU::PageBoundaryCrossed();
			}
			// branch false
			CPU::Reg.PC += 2;
			return 2;
		}break;
		case BVC: {
			if (CPU::Status.V) {
				// branch false
				CPU::Reg.PC += 2;
				return 2;
			}
			// branch true
			CPU::Reg.PC = CPU::GetAddr_REL();
			++CPU::Reg.PC;
			return 3 + CPU::PageBoundaryCrossed();
		}break;
		case BVS: {
			if (CPU::Status.V) {
				// branch true
				CPU::Reg.PC = CPU::GetAddr_REL();
				++CPU::Reg.PC;
				return 3 + CPU::PageBoundaryCrossed();
			}
			// branch false
			CPU::Reg.PC += 2;
			return 2;
		}break;
		case BCC: {
			if (CPU::Status.C) {
				// branch false
				CPU::Reg.PC += 2;
				return 2;
			}
			// branch true
			CPU::Reg.PC = CPU::GetAddr_REL();
			++CPU::Reg.PC;
			return 3 + CPU::PageBoundaryCrossed();
		}break;
		case BCS: {
			if (CPU::Status.C) {
				// branch true
				CPU::Reg.PC = CPU::GetAddr_REL();
				++CPU::Reg.PC;
				return 3 + CPU::PageBoundaryCrossed();
			}
			// branch false
			CPU::Reg.PC += 2;
			return 2;
		}break;
		case BNE: {
			if (CPU::Status.Z) {
				// branch false
				CPU::Reg.PC += 2;
				return 2;
			}
			// branch true
			CPU::Reg.PC = CPU::GetAddr_REL();
			++CPU::Reg.PC;
			return 3 + CPU::PageBoundaryCrossed();
		}break;
		case BEQ: {
			if (CPU::Status.Z) {
				// branch true
				CPU::Reg.PC = CPU::GetAddr_REL();
				++CPU::Reg.PC;
				return 3 + CPU::PageBoundaryCrossed();
			}
			// branch false
			CPU::Reg.PC += 2;
			return 2;
		}break;


			// BREAK
		case BRK: {
			return CPU::BRK();
		}break;


			// CMP
		case CMP_IMM: {
			Fast8bit M = CPU::Deref_IMM();
			SetFlagsNZ(CPU::Reg.A - M);
			CPU::Status.C = CPU::Reg.A >= M;
			++CPU::Reg.PC;
			return 2;
		}break;
		case CMP_ZERO: {
			Fast8bit M = CPU::Deref_ZERO();
			SetFlagsNZ(CPU::Reg.A - M);
			CPU::Status.C = CPU::Reg.A >= M;
			++CPU::Reg.PC;
			return 3;
		}break;
		case CMP_ZERO_X: {
			Fast8bit M = CPU::Deref_ZERO_X();
			SetFlagsNZ(CPU::Reg.A - M);
			CPU::Status.C = CPU::Reg.A >= M;
			++CPU::Reg.PC;
			return 4;
		}break;
		case CMP_ABS: {
			Fast8bit M = CPU::Deref_ABS();
			SetFlagsNZ(CPU::Reg.A - M);
			CPU::Status.C = CPU::Reg.A >= M;
			++CPU::Reg.PC;
			return 4;
		}break;
		case CMP_ABS_X: {
			Fast8bit M = CPU::Deref_ABS_X();
			SetFlagsNZ(CPU::Reg.A - M);
			CPU::Status.C = CPU::Reg.A >= M;
			++CPU::Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case CMP_ABS_Y: {
			Fast8bit M = CPU::Deref_ABS_Y();
			SetFlagsNZ(CPU::Reg.A - M);
			CPU::Status.C = CPU::Reg.A >= M;
			++CPU::Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case CMP_IND_X: {
			Fast8bit M = CPU::Deref_IND_X();
			SetFlagsNZ(CPU::Reg.A - M);
			CPU::Status.C = CPU::Reg.A >= M;
			++CPU::Reg.PC;
			return 6;
		}break;
		case CMP_IND_Y: {
			Fast8bit M = CPU::Deref_IND_Y();
			SetFlagsNZ(CPU::Reg.A - M);
			CPU::Status.C = CPU::Reg.A >= M;
			++CPU::Reg.PC;
			return 5 + CPU::PageBoundaryCrossed();
		}break;


			// CPX
		case CPX_IMM: {
			Fast8bit M = CPU::Deref_IMM();
			SetFlagsNZ(CPU::Reg.X - M);
			CPU::Status.C = CPU::Reg.X >= M;
			++CPU::Reg.PC;
			return 2;
		}break;
		case CPX_ZERO: {
			Fast8bit M = CPU::Deref_ZERO();
			SetFlagsNZ(CPU::Reg.X - M);
			CPU::Status.C = CPU::Reg.X >= M;
			++CPU::Reg.PC;
			return 3;
		}break;
		case CPX_ABS: {
			Fast8bit M = CPU::Deref_ABS();
			SetFlagsNZ(CPU::Reg.X - M);
			CPU::Status.C = CPU::Reg.X >= M;
			++CPU::Reg.PC;
			return 4;
		}break;



			// CPY
		case CPY_IMM: {
			Fast8bit M = CPU::Deref_IMM();
			SetFlagsNZ(CPU::Reg.Y - M);
			CPU::Status.C = CPU::Reg.Y >= M;
			++CPU::Reg.PC;
			return 2;
		}break;
		case CPY_ZERO: {
			Fast8bit M = CPU::Deref_ZERO();
			SetFlagsNZ(CPU::Reg.Y - M);
			CPU::Status.C = CPU::Reg.Y >= M;
			++CPU::Reg.PC;
			return 3;
		}break;
		case CPY_ABS: {
			Fast8bit M = CPU::Deref_ABS();
			SetFlagsNZ(CPU::Reg.Y - M);
			CPU::Status.C = CPU::Reg.Y >= M;
			++CPU::Reg.PC;
			return 4;
		}break;


			// DEC
		case DEC_ZERO: {
			SetFlagsNZ(--Memory[CPU::GetAddr_ZERO()]);
			++CPU::Reg.PC;
			return 5;
		}break;
		case DEC_ZERO_X: {
			SetFlagsNZ(--Memory[CPU::GetAddr_ZERO_X()]);
			++CPU::Reg.PC;
			return 6;
		}break;
		case DEC_ABS: {
			SetFlagsNZ(--Memory[CPU::GetAddr_ABS()]);
			++CPU::Reg.PC;
			return 6;
		}break;
		case DEC_ABS_X: {
			SetFlagsNZ(--Memory[CPU::GetAddr_ABS_X()]);
			++CPU::Reg.PC;
			return 7;
		}break;


			// EOR
		case EOR_IMM: {
			SetFlagsNZ( CPU::Reg.A ^= CPU::Deref_IMM() );
			++CPU::Reg.PC;
			return 2;
		}break;
		case EOR_ZERO: {
			SetFlagsNZ(CPU::Reg.A ^= CPU::Deref_ZERO());
			++CPU::Reg.PC;
			return 3;
		}break;
		case EOR_ZERO_X: {
			SetFlagsNZ(CPU::Reg.A ^= CPU::Deref_ZERO_X());
			++CPU::Reg.PC;
			return 4;
		}break;
		case EOR_ABS: {
			SetFlagsNZ(CPU::Reg.A ^= CPU::Deref_ABS());
			++CPU::Reg.PC;
			return 4;
		}break;
		case EOR_ABS_X: {
			SetFlagsNZ(CPU::Reg.A ^= CPU::Deref_ABS_X());
			++CPU::Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case EOR_ABS_Y: {
			SetFlagsNZ(CPU::Reg.A ^= CPU::Deref_ABS_Y());
			++CPU::Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case EOR_IND_X: {
			SetFlagsNZ(CPU::Reg.A ^= CPU::Deref_IND_X());
			++CPU::Reg.PC;
			return 6;
		}break;
		case EOR_IND_Y: {
			SetFlagsNZ(CPU::Reg.A ^= CPU::Deref_IND_Y());
			++CPU::Reg.PC;
			return 5 + CPU::PageBoundaryCrossed();
		}break;


			// Flag (Processor Status) Instructions
		case CLC: {
			CPU::Status.C = 0;
			++CPU::Reg.PC;
			return 2;
		}break;
		case SEC: {
			CPU::Status.C = 1;
			++CPU::Reg.PC;
			return 2;
		}break;
		case CLI: {
			CPU::Status.I = 0;
			++CPU::Reg.PC;
			return 2;
		}break;
		case SEI: {
			CPU::Status.I = 1;
			++CPU::Reg.PC;
			return 2;
		}break;
		case CLV: {
			CPU::Status.V = 0;
			++CPU::Reg.PC;
			return 2;
		}break;
		case CLD: {
			CPU::Status.D = 0;
			++CPU::Reg.PC;
			return 2;
		}break;
		case SED: {
			CPU::Status.D = 1;
			++CPU::Reg.PC;
			return 2;
		}break;


			// INC
		case INC_ZERO: {
			SetFlagsNZ(++Memory[CPU::GetAddr_ZERO()]);
			++CPU::Reg.PC;
			return 5;
		}break;
		case INC_ZERO_X: {
			SetFlagsNZ(++Memory[CPU::GetAddr_ZERO_X()]);
			++CPU::Reg.PC;
			return 6;
		}break;
		case INC_ABS: {
			SetFlagsNZ(++Memory[CPU::GetAddr_ABS()]);
			++CPU::Reg.PC;
			return 6;
		}break;
		case INC_ABS_X: {
			SetFlagsNZ(++Memory[CPU::GetAddr_ABS_X()]);
			++CPU::Reg.PC;
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
			CPU::Stack_Push(((CPU::Reg.PC + 3) & 0xFF00) >> 8); // Push PC High byte on stack
			CPU::Stack_Push((CPU::Reg.PC + 3) & 0xFF);	// Push PC Low byte on stack
			CPU::Reg.PC = CPU::GetAddr_ABS();
			return 6;
		}break;


			// LDA
		case LDA_IMM: {
			SetFlagsNZ(CPU::Reg.A = CPU::Deref_IMM());
			++CPU::Reg.PC;
			return 2;
		}break;
		case LDA_ZERO: {
			SetFlagsNZ(CPU::Reg.A = CPU::Deref_ZERO());
			++CPU::Reg.PC;
			return 3;
		}break;
		case LDA_ZERO_X: {
			SetFlagsNZ(CPU::Reg.A = CPU::Deref_ZERO_X());
			++CPU::Reg.PC;
			return 4;
		}break;
		case LDA_ABS: {
			SetFlagsNZ(CPU::Reg.A = CPU::Deref_ABS());
			++CPU::Reg.PC;
			return 4;
		}break;
		case LDA_ABS_X: {
			SetFlagsNZ(CPU::Reg.A = CPU::Deref_ABS_X());
			++CPU::Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case LDA_ABS_Y: {
			SetFlagsNZ(CPU::Reg.A = CPU::Deref_ABS_Y());
			++CPU::Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case LDA_IND_X: {
			SetFlagsNZ(CPU::Reg.A = CPU::Deref_IND_X());
			++CPU::Reg.PC;
			return 6;
		}break;
		case LDA_IND_Y: {
			SetFlagsNZ(CPU::Reg.A = CPU::Deref_IND_Y());
			++CPU::Reg.PC;
			return 5 + CPU::PageBoundaryCrossed();
		}break;


			// LDX
		case LDX_IMM: {
			SetFlagsNZ(CPU::Reg.X = CPU::Deref_IMM());
			++CPU::Reg.PC;
			return 2;
		}break;
		case LDX_ZERO: {
			SetFlagsNZ(CPU::Reg.X = CPU::Deref_ZERO());
			++CPU::Reg.PC;
			return 3;
		}break;
		case LDX_ZERO_Y: {
			SetFlagsNZ(CPU::Reg.X = CPU::Deref_ZERO_Y());
			++CPU::Reg.PC;
			return 4;
		}break;
		case LDX_ABS: {
			SetFlagsNZ(CPU::Reg.X = CPU::Deref_ABS());
			++CPU::Reg.PC;
			return 4;
		}break;
		case LDX_ABS_Y: {
			SetFlagsNZ(CPU::Reg.X = CPU::Deref_ABS_Y());
			++CPU::Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;


			// LDY
		case LDY_IMM: {
			SetFlagsNZ(CPU::Reg.Y = CPU::Deref_IMM());
			++CPU::Reg.PC;
			return 2;
		}break;
		case LDY_ZERO: {
			SetFlagsNZ(CPU::Reg.Y = CPU::Deref_ZERO());
			++CPU::Reg.PC;
			return 3;
		}break;
		case LDY_ZERO_X: {
			SetFlagsNZ(CPU::Reg.Y = CPU::Deref_ZERO_X());
			++CPU::Reg.PC;
			return 4;
		}break;
		case LDY_ABS: {
			SetFlagsNZ(CPU::Reg.Y = CPU::Deref_ABS());
			++CPU::Reg.PC;
			return 4;
		}break;
		case LDY_ABS_X: {
			SetFlagsNZ(CPU::Reg.Y = CPU::Deref_ABS_X());
			++CPU::Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;


			// LSR
		case LSR_ACC: {
			CPU::Status.C = (CPU::Reg.A & 0x1);
			SetFlagsNZ(CPU::Reg.A = CPU::Reg.A >> 1);
			++CPU::Reg.PC;
			return 2;
		}break;
		case LSR_ZERO: {
			LSR(CPU::GetAddr_ZERO());
			++CPU::Reg.PC;
			return 5;
		}break;
		case LSR_ZERO_X: {
			LSR(CPU::GetAddr_ZERO_X());
			++CPU::Reg.PC;
			return 6;
		}break;
		case LSR_ABS: {
			LSR(CPU::GetAddr_ABS());
			++CPU::Reg.PC;
			return 6;
		}break;
		case LSR_ABS_X: {
			LSR(CPU::GetAddr_ABS_X());
			++CPU::Reg.PC;
			return 7;
		}break;


			// NOP
		case NOP: {
			++CPU::Reg.PC;
			return 2;
		}break;


			// ORA
		case ORA_IMM: {
			SetFlagsNZ(CPU::Reg.A |= CPU::Deref_IMM());
			++CPU::Reg.PC;
			return 2;
		}break;
		case ORA_ZERO: {
			SetFlagsNZ(CPU::Reg.A |= CPU::Deref_ZERO());
			++CPU::Reg.PC;
			return 3;
		}break;
		case ORA_ZERO_X: {
			SetFlagsNZ(CPU::Reg.A |= CPU::Deref_ZERO_X());
			++CPU::Reg.PC;
			return 4;
		}break;
		case ORA_ABS: {
			SetFlagsNZ(CPU::Reg.A |= CPU::Deref_ABS());
			++CPU::Reg.PC;
			return 4;
		}break;
		case ORA_ABS_X: {
			SetFlagsNZ(CPU::Reg.A |= CPU::Deref_ABS_X());
			++CPU::Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case ORA_ABS_Y: {
			SetFlagsNZ(CPU::Reg.A |= CPU::Deref_ABS_Y());
			++CPU::Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case ORA_IND_X: {
			SetFlagsNZ(CPU::Reg.A |= CPU::Deref_IND_X());
			++CPU::Reg.PC;
			return 6;
		}break;
		case ORA_IND_Y: {
			SetFlagsNZ(CPU::Reg.A |= CPU::Deref_IND_Y());
			++CPU::Reg.PC;
			return 5 + CPU::PageBoundaryCrossed();
		}break;


			// Register Instructions 
		case TAX: {
			SetFlagsNZ(CPU::Reg.X = CPU::Reg.A);
			++CPU::Reg.PC;
			return 2;
		}break;
		case TXA: {
			SetFlagsNZ(CPU::Reg.A = CPU::Reg.X);
			++CPU::Reg.PC;
			return 2;
		}break;
		case DEX: {
			SetFlagsNZ(--CPU::Reg.X);
			++CPU::Reg.PC;
			return 2;
		}break;
		case INX: {
			SetFlagsNZ(++CPU::Reg.X);
			++CPU::Reg.PC;
			return 2;
		}break;
		case TAY: {
			SetFlagsNZ(CPU::Reg.Y = CPU::Reg.A);
			++CPU::Reg.PC;
			return 2;
		}break;
		case TYA: {
			SetFlagsNZ(CPU::Reg.A = CPU::Reg.Y);
			++CPU::Reg.PC;
			return 2;
		}break;
		case DEY: {
			SetFlagsNZ(--CPU::Reg.Y);
			++CPU::Reg.PC;
			return 2;
		}break;
		case INY: {
			SetFlagsNZ(++CPU::Reg.Y);
			++CPU::Reg.PC;
			return 2;
		}break;


			// ROL
		case ROL_ACC: {
			Fast8bit c = CPU::Status.C;
			CPU::Status.C = CPU::Reg.A & 0x80 >> 7;
			CPU::Reg.A = CPU::Reg.A << 1;
			PASTE_BIT(CPU::Reg.A, 0, c);
			++CPU::Reg.PC;
			return 2;
		}break;
		case ROL_ZERO: {
			ROL(CPU::GetAddr_ZERO());
			++CPU::Reg.PC;
			return 5;
		}break;
		case ROL_ZERO_X: {
			ROL(CPU::GetAddr_ZERO_X());
			++CPU::Reg.PC;
			return 6;
		}break;
		case ROL_ABS: {
			ROL(CPU::GetAddr_ABS());
			++CPU::Reg.PC;
			return 6;
		}break;
		case ROL_ABS_X: {
			ROL(CPU::GetAddr_ABS_X());
			++CPU::Reg.PC;
			return 7;
		}break;


			// ROR
		case ROR_ACC: {
			Fast8bit c = CPU::Status.C;
			CPU::Status.C = CPU::Reg.A & 0x1;
			CPU::Reg.A = CPU::Reg.A >> 1;
			PASTE_BIT(CPU::Reg.A, 7, c);
			++CPU::Reg.PC;
			return 2;
		}break;
		case ROR_ZERO: {
			ROR(CPU::GetAddr_ZERO());
			++CPU::Reg.PC;
			return 5;
		}break;
		case ROR_ZERO_X: {
			ROR(CPU::GetAddr_ZERO_X());
			++CPU::Reg.PC;
			return 6;
		}break;
		case ROR_ABS: {
			ROR(CPU::GetAddr_ABS());
			++CPU::Reg.PC;
			return 6;
		}break;
		case ROR_ABS_X: {
			ROR(CPU::GetAddr_ABS_X());
			++CPU::Reg.PC;
			return 7;
		}break;


			// RTI
		case RTI_IMP: {
			CPU::Status.all_flags = CPU::Stack_Pull();
			Word8bit LSB = CPU::Stack_Pull();
			Word8bit MSB = CPU::Stack_Pull();
			CPU::Reg.PC = CPU::GetAddr_(LSB, MSB);
			return 6;
		}break;


			// RTS
		case RTS: {
			Word8bit LSB = CPU::Stack_Pull();
			Word8bit MSB = CPU::Stack_Pull();
			CPU::Reg.PC = CPU::GetAddr_(LSB, MSB);
			return 6;
		}break;


			// SBC
		case SBC_IMM: {
			SBC(CPU::Deref_IMM());
			++CPU::Reg.PC;
			return 2;
		}break;
		case SBC_ZERO: {
			SBC(CPU::Deref_ZERO());
			++CPU::Reg.PC;
			return 3;
		}break;
		case SBC_ZERO_X: {
			SBC(CPU::Deref_ZERO_X());
			++CPU::Reg.PC;
			return 4;
		}break;
		case SBC_ABS: {
			SBC(CPU::Deref_ABS());
			++CPU::Reg.PC;
			return 4;
		}break;
		case SBC_ABS_X: {
			SBC(CPU::Deref_ABS_X());
			++CPU::Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case SBC_ABS_Y: {
			SBC(CPU::Deref_ABS_Y());
			++CPU::Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case SBC_IND_X: {
			SBC(CPU::Deref_IND_X());
			++CPU::Reg.PC;
			return 6;
		}break;
		case SBC_IND_Y: {
			SBC(CPU::Deref_IND_Y());
			++CPU::Reg.PC;
			return 5 + CPU::PageBoundaryCrossed();
		}break;


			// STA
		case STA_ZERO: {
			Memory[CPU::GetAddr_ZERO()].Write(CPU::Reg.A);
			++CPU::Reg.PC;
			return 3;
		}break;
		case STA_ZERO_X: {
			Memory[CPU::GetAddr_ZERO_X()].Write(CPU::Reg.A);
			++CPU::Reg.PC;
			return 4;
		}break;
		case STA_ABS: {
			Memory[CPU::GetAddr_ABS()].Write(CPU::Reg.A);
			++CPU::Reg.PC;
			return 4;
		}break;
		case STA_ABS_X: {
			Memory[CPU::GetAddr_ABS_X()].Write(CPU::Reg.A);
			++CPU::Reg.PC;
			return 5;
		}break;
		case STA_ABS_Y: {
			Memory[CPU::GetAddr_ABS_Y()].Write(CPU::Reg.A);
			++CPU::Reg.PC;
			return 5;
		}break;
		case STA_IND_X: {
			Memory[CPU::GetAddr_IND_X()].Write(CPU::Reg.A);
			++CPU::Reg.PC;
			return 6;
		}break;
		case STA_IND_Y: {
			Memory[CPU::GetAddr_IND_Y()].Write(CPU::Reg.A);
			++CPU::Reg.PC;
			return 6;
		}break;


			// Stack Instructions
		case TXS: {
			CPU::Reg.SP = CPU::Reg.X & 0xFF;
			++CPU::Reg.PC;
			return 2;
		}break;
		case TSX: {
			SetFlagsNZ(CPU::Reg.X = CPU::Reg.SP & 0xFF);
			++CPU::Reg.PC;
			return 2;
		}break;
		case PHA: {
			CPU::Stack_Push(CPU::Reg.A);
			++CPU::Reg.PC;
			return 3;
		}break;
		case PLA: {
			SetFlagsNZ(CPU::Reg.A = CPU::Stack_Pull());
			++CPU::Reg.PC;
			return 4;
		}break;
		case PHP: {
			volatile CPU::StatusRegisters p;
			p.all_flags = CPU::Status.all_flags;
			p.B = 1;
			CPU::Stack_Push(p.all_flags);
			++CPU::Reg.PC;
			return 3;
		}break;
		case PLP: {
			CPU::Status.all_flags = CPU::Stack_Pull();
			++CPU::Reg.PC;
			return 4;
		}break;



			// STX
		case STX_ZERO: {
			Memory[CPU::GetAddr_ZERO()].Write(CPU::Reg.X);
			++CPU::Reg.PC;
			return 3;
		}break;
		case STX_ZERO_Y: {
			Memory[CPU::GetAddr_ZERO_Y()].Write(CPU::Reg.X);
			++CPU::Reg.PC;
			return 4;
		}break;
		case STX_ABS: {
			Memory[CPU::GetAddr_ABS()].Write(CPU::Reg.X);
			++CPU::Reg.PC;
			return 4;
		}break;


			// STY
		case STY_ZERO: {
			Memory[CPU::GetAddr_ZERO()].Write(CPU::Reg.Y);
			++CPU::Reg.PC;
			return 3;
		}break;
		case STY_ZERO_X: {
			Memory[CPU::GetAddr_ZERO_X()].Write(CPU::Reg.Y);
			++CPU::Reg.PC;
			return 4;
		}break;
		case STY_ABS: {
			Memory[CPU::GetAddr_ABS()].Write(CPU::Reg.Y);
			++CPU::Reg.PC;
			return 4;
		}break;


		default: {
			// illegal instructions are not supported in this version.
			LOG("(Error:%hhX), ", opcode);
			++CPU::Reg.PC;
			return 1;
		}break;
		}

	}

}




