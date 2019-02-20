#include "stdafx.h"
#include "mos6502emu.h"
#include "Opcodes_mos6502.h"
#include "CPU.h"

namespace mos6502emu {
	static inline void SetFlagsNZ(Word8bit result) {
		Status.N = (result & 0x80) >> 7;
		Status.Z = ((result & 0xFF) == 0);
	}

	static inline void ADC(Word8bit data) {
		Word8bit A = Reg.A;
		Word16bit sum = A + (data & 0xFF) + Status.C;
		Reg.A = sum & 0xFF;

		Status.C = (sum > 255);
		Status.V = ((A^sum)&(data^sum) & 0x80) >> 7;
		SetFlagsNZ((Word8bit)sum);
	}

	static inline void ASL(Word16bit address) {
		Word8bit value = Memory[address].Read();
		Status.C = (value & 0x80) >> 7;
		SetFlagsNZ(value <<= 1);
		Memory[address].Write(value);
	}

	static inline void LSR(Word16bit address) {
		Word8bit value = Memory[address].Read();
		Status.C = (value & 0x1);
		SetFlagsNZ(value >>= 1);
		Memory[address].Write(value);
	}

	static inline void ROL(Word16bit address) {
		Word8bit c = Status.C;
		Word8bit value = Memory[address].Read();
		Status.C = value & 0x80 >> 7;
		Memory[address].Write(PASTE_BIT(value << 1, 0, c));
	}

	static inline void ROR(Word16bit address) {
		Word8bit c = Status.C;
		Word8bit value = Memory[address].Read();
		Status.C = value; // no need of clearing MSBits (Memory[address] & 0x01), the expression needs LSB in this case.
		Memory[address].Write(PASTE_BIT(value >> 1, 7, c));
	}

	static inline void SBC(Word8bit data) {
		Word8bit A = Reg.A & 0xFF;

		Word8bit diff = (Reg.A = (A + ((~data) & 0xFF) + Status.C));

		Status.C = (data <= A);
		Status.V = ((A^diff)&(~data^diff) & 0x80) >> 7;
		SetFlagsNZ(diff);
	}

	CyclesUsed ExecuteOpcode(Word8bit opcode) {
		//LOG("%hX: %hhX,\n", Reg.PC, opcode);
		/*LOG("%hX: %hhX,\n\nA: %hhX,\nX: %hhX,\nY: %hhX,\nSP: %hhX,\n\nP: %d%d%d%d%d%d%d%d\n\n",
			Reg.PC, opcode,
			Reg.A,
			Reg.X,
			Reg.Y,
			Reg.SP,
			Status.N,
			Status.V,
			Status.reserved,
			Status.B,
			Status.D,
			Status.I,
			Status.Z,
			Status.C
			); */
		switch (opcode) {

			// ADC
		case ADC_IMM: {
			ADC(CPU::Deref_IMM());
			++Reg.PC;
			return 2;
		}break;
		case ADC_ZERO: {
			ADC(CPU::Deref_ZERO());
			++Reg.PC;
			return 3;
		}break;
		case ADC_ZERO_X: {
			ADC(CPU::Deref_ZERO_X());
			++Reg.PC;
			return 4;
		}break;
		case ADC_ABS: {
			ADC(CPU::Deref_ABS());
			++Reg.PC;
			return 4;
		}break;
		case ADC_ABS_X: {
			ADC(CPU::Deref_ABS_X());
			++Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case ADC_ABS_Y: {
			ADC(CPU::Deref_ABS_Y());
			++Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case ADC_IND_X: {
			ADC(CPU::Deref_IND_X());
			++Reg.PC;
			return 6;
		}break;
		case ADC_IND_Y: {
			ADC(CPU::Deref_IND_Y());
			++Reg.PC;
			return 5 + CPU::PageBoundaryCrossed();
		}break;


			// AND
		case AND_IMM: {
			SetFlagsNZ(Reg.A &= CPU::Deref_IMM());
			++Reg.PC;
			return 2;
		}break;
		case AND_ZERO: {
			SetFlagsNZ(Reg.A &= CPU::Deref_ZERO());
			++Reg.PC;
			return 3;
		}break;
		case AND_ZERO_X: {
			SetFlagsNZ(Reg.A &= CPU::Deref_ZERO_X());
			++Reg.PC;
			return 4;
		}break;
		case AND_ABS: {
			SetFlagsNZ(Reg.A &= CPU::Deref_ABS());
			++Reg.PC;
			return 4;
		}break;
		case AND_ABS_X: {
			SetFlagsNZ(Reg.A &= CPU::Deref_ABS_X());
			++Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case AND_ABS_Y: {
			SetFlagsNZ(Reg.A &= CPU::Deref_ABS_Y());
			++Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case AND_IND_X: {
			SetFlagsNZ(Reg.A &= CPU::Deref_IND_X());
			++Reg.PC;
			return 6;
		}break;
		case AND_IND_Y: {
			SetFlagsNZ(Reg.A &= CPU::Deref_IND_Y());
			++Reg.PC;
			return 5 + CPU::PageBoundaryCrossed();
		}break;


			// ASL
		case ASL_ACC: {
			Status.C = (Reg.A & 0x80) >> 7;
			SetFlagsNZ(Reg.A = Reg.A << 1);
			++Reg.PC;
			return 2;
		}break;
		case ASL_ZERO: {
			ASL(CPU::GetAddr_ZERO());
			++Reg.PC;
			return 5;
		}break;
		case ASL_ZERO_X: {
			ASL(CPU::GetAddr_ZERO_X());
			++Reg.PC;
			return 6;
		}break;
		case ASL_ABS: {
			ASL(CPU::GetAddr_ABS());
			++Reg.PC;
			return 6;
		}break;
		case ASL_ABS_X: {
			ASL(CPU::GetAddr_ABS_X());
			++Reg.PC;
			return 7;
		}break;


			// BIT
		case BIT_ZERO: {
			Word8bit result = Reg.A & CPU::Deref_ZERO();
			SetFlagsNZ(result);
			Status.V = (result & 0x40) >> 6;
			++Reg.PC;
			return 3;
		}break;
		case BIT_ABS: {
			Word8bit result = Reg.A & CPU::Deref_ABS();
			SetFlagsNZ(result);
			Status.V = (result & 0x40) >> 6;
			++Reg.PC;
			return 4;
		}break;


			// Branch instructions
			// 2 cycles + 1 if true, + 1 if crosses page boundary
		case BPL: {
			if (Status.N) {
				// branch false
				Reg.PC += 2;
				return 2;
			}
			// branch true
			Reg.PC = CPU::GetAddr_REL();
			return 3 + CPU::PageBoundaryCrossed();
		}break;
		case BMI: {
			if (Status.N) {
				// branch true
				Reg.PC = CPU::GetAddr_REL();
				return 3 + CPU::PageBoundaryCrossed();
			}
			// branch false
			Reg.PC += 2;
			return 2;
		}break;
		case BVC: {
			if (Status.V) {
				// branch false
				Reg.PC += 2;
				return 2;
			}
			// branch true
			Reg.PC = CPU::GetAddr_REL();
			return 3 + CPU::PageBoundaryCrossed();
		}break;
		case BVS: {
			if (Status.V) {
				// branch true
				Reg.PC = CPU::GetAddr_REL();
				return 3 + CPU::PageBoundaryCrossed();
			}
			// branch false
			Reg.PC += 2;
			return 2;
		}break;
		case BCC: {
			if (Status.C) {
				// branch false
				Reg.PC += 2;
				return 2;
			}
			// branch true
			Reg.PC = CPU::GetAddr_REL();
			return 3 + CPU::PageBoundaryCrossed();
		}break;
		case BCS: {
			if (Status.C) {
				// branch true
				Reg.PC = CPU::GetAddr_REL();
				return 3 + CPU::PageBoundaryCrossed();
			}
			// branch false
			Reg.PC += 2;
			return 2;
		}break;
		case BNE: {
			if (Status.Z) {
				// branch false
				Reg.PC += 2;
				return 2;
			}
			// branch true
			Reg.PC = CPU::GetAddr_REL();
			return 3 + CPU::PageBoundaryCrossed();
		}break;
		case BEQ: {
			if (Status.Z) {
				// branch true
				Reg.PC = CPU::GetAddr_REL();
				return 3 + CPU::PageBoundaryCrossed();
			}
			// branch false
			Reg.PC += 2;
			return 2;
		}break;


			// BREAK
		case BRK: {
			return CPU::BRK();
		}break;


			// CMP
		case CMP_IMM: {
			Word8bit M = CPU::Deref_IMM();
			SetFlagsNZ(Reg.A - M);
			Status.C = Reg.A >= M;
			++Reg.PC;
			return 2;
		}break;
		case CMP_ZERO: {
			Word8bit M = CPU::Deref_ZERO();
			SetFlagsNZ(Reg.A - M);
			Status.C = Reg.A >= M;
			++Reg.PC;
			return 3;
		}break;
		case CMP_ZERO_X: {
			Word8bit M = CPU::Deref_ZERO_X();
			SetFlagsNZ(Reg.A - M);
			Status.C = Reg.A >= M;
			++Reg.PC;
			return 4;
		}break;
		case CMP_ABS: {
			Word8bit M = CPU::Deref_ABS();
			SetFlagsNZ(Reg.A - M);
			Status.C = Reg.A >= M;
			++Reg.PC;
			return 4;
		}break;
		case CMP_ABS_X: {
			Word8bit M = CPU::Deref_ABS_X();
			SetFlagsNZ(Reg.A - M);
			Status.C = Reg.A >= M;
			++Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case CMP_ABS_Y: {
			Word8bit M = CPU::Deref_ABS_Y();
			SetFlagsNZ(Reg.A - M);
			Status.C = Reg.A >= M;
			++Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case CMP_IND_X: {
			Word8bit M = CPU::Deref_IND_X();
			SetFlagsNZ(Reg.A - M);
			Status.C = Reg.A >= M;
			++Reg.PC;
			return 6;
		}break;
		case CMP_IND_Y: {
			Word8bit M = CPU::Deref_IND_Y();
			SetFlagsNZ(Reg.A - M);
			Status.C = Reg.A >= M;
			++Reg.PC;
			return 5 + CPU::PageBoundaryCrossed();
		}break;


			// CPX
		case CPX_IMM: {
			Word8bit M = CPU::Deref_IMM();
			SetFlagsNZ(Reg.X - M);
			Status.C = Reg.X >= M;
			++Reg.PC;
			return 2;
		}break;
		case CPX_ZERO: {
			Word8bit M = CPU::Deref_ZERO();
			SetFlagsNZ(Reg.X - M);
			Status.C = Reg.X >= M;
			++Reg.PC;
			return 3;
		}break;
		case CPX_ABS: {
			Word8bit M = CPU::Deref_ABS();
			SetFlagsNZ(Reg.X - M);
			Status.C = Reg.X >= M;
			++Reg.PC;
			return 4;
		}break;



			// CPY
		case CPY_IMM: {
			Word8bit M = CPU::Deref_IMM();
			SetFlagsNZ(Reg.Y - M);
			Status.C = Reg.Y >= M;
			++Reg.PC;
			return 2;
		}break;
		case CPY_ZERO: {
			Word8bit M = CPU::Deref_ZERO();
			SetFlagsNZ(Reg.Y - M);
			Status.C = Reg.Y >= M;
			++Reg.PC;
			return 3;
		}break;
		case CPY_ABS: {
			Word8bit M = CPU::Deref_ABS();
			SetFlagsNZ(Reg.Y - M);
			Status.C = Reg.Y >= M;
			++Reg.PC;
			return 4;
		}break;


			// DEC
		case DEC_ZERO: {
			SetFlagsNZ(--Memory[CPU::GetAddr_ZERO()]);
			++Reg.PC;
			return 5;
		}break;
		case DEC_ZERO_X: {
			SetFlagsNZ(--Memory[CPU::GetAddr_ZERO_X()]);
			++Reg.PC;
			return 6;
		}break;
		case DEC_ABS: {
			SetFlagsNZ(--Memory[CPU::GetAddr_ABS()]);
			++Reg.PC;
			return 6;
		}break;
		case DEC_ABS_X: {
			SetFlagsNZ(--Memory[CPU::GetAddr_ABS_X()]);
			++Reg.PC;
			return 7;
		}break;


			// EOR
		case EOR_IMM: {
			SetFlagsNZ( Reg.A ^= CPU::Deref_IMM() );
			++Reg.PC;
			return 2;
		}break;
		case EOR_ZERO: {
			SetFlagsNZ(Reg.A ^= CPU::Deref_ZERO());
			++Reg.PC;
			return 3;
		}break;
		case EOR_ZERO_X: {
			SetFlagsNZ(Reg.A ^= CPU::Deref_ZERO_X());
			++Reg.PC;
			return 4;
		}break;
		case EOR_ABS: {
			SetFlagsNZ(Reg.A ^= CPU::Deref_ABS());
			++Reg.PC;
			return 4;
		}break;
		case EOR_ABS_X: {
			SetFlagsNZ(Reg.A ^= CPU::Deref_ABS_X());
			++Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case EOR_ABS_Y: {
			SetFlagsNZ(Reg.A ^= CPU::Deref_ABS_Y());
			++Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case EOR_IND_X: {
			SetFlagsNZ(Reg.A ^= CPU::Deref_IND_X());
			++Reg.PC;
			return 6;
		}break;
		case EOR_IND_Y: {
			SetFlagsNZ(Reg.A ^= CPU::Deref_IND_Y());
			++Reg.PC;
			return 5 + CPU::PageBoundaryCrossed();
		}break;


			// Flag (Processor Status) Instructions
		case CLC: {
			Status.C = 0;
			++Reg.PC;
			return 2;
		}break;
		case SEC: {
			Status.C = 1;
			++Reg.PC;
			return 2;
		}break;
		case CLI: {
			Status.I = 0;
			++Reg.PC;
			return 2;
		}break;
		case SEI: {
			Status.I = 1;
			++Reg.PC;
			return 2;
		}break;
		case CLV: {
			Status.V = 0;
			++Reg.PC;
			return 2;
		}break;
		case CLD: {
			Status.D = 0;
			++Reg.PC;
			return 2;
		}break;
		case SED: {
			Status.D = 1;
			++Reg.PC;
			return 2;
		}break;


			// INC
		case INC_ZERO: {
			SetFlagsNZ(++Memory[CPU::GetAddr_ZERO()]);
			++Reg.PC;
			return 5;
		}break;
		case INC_ZERO_X: {
			SetFlagsNZ(++Memory[CPU::GetAddr_ZERO_X()]);
			++Reg.PC;
			return 6;
		}break;
		case INC_ABS: {
			SetFlagsNZ(++Memory[CPU::GetAddr_ABS()]);
			++Reg.PC;
			return 6;
		}break;
		case INC_ABS_X: {
			SetFlagsNZ(++Memory[CPU::GetAddr_ABS_X()]);
			++Reg.PC;
			return 7;
		}break;


			// JMP
		case JMP_ABS: {
			Reg.PC = CPU::GetAddr_ABS();
			return 3;
		}break;
		case JMP_IND: {
			Reg.PC = CPU::GetAddr_IND();
			return 5;
		}break;


			// JSR
		case JSR_ABS: {
			Word16bit pc = Reg.PC + 2;
			CPU::Stack_Push((pc & 0xFF00) >> 8); // Push PC High byte on stack
			CPU::Stack_Push(pc & 0xFF);	// Push PC Low byte on stack
			Reg.PC = CPU::GetAddr_ABS();
			return 6;
		}break;


			// LDA
		case LDA_IMM: {
			SetFlagsNZ(Reg.A = CPU::Deref_IMM());
			++Reg.PC;
			return 2;
		}break;
		case LDA_ZERO: {
			SetFlagsNZ(Reg.A = CPU::Deref_ZERO());
			++Reg.PC;
			return 3;
		}break;
		case LDA_ZERO_X: {
			SetFlagsNZ(Reg.A = CPU::Deref_ZERO_X());
			++Reg.PC;
			return 4;
		}break;
		case LDA_ABS: {
			SetFlagsNZ(Reg.A = CPU::Deref_ABS());
			++Reg.PC;
			return 4;
		}break;
		case LDA_ABS_X: {
			SetFlagsNZ(Reg.A = CPU::Deref_ABS_X());
			++Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case LDA_ABS_Y: {
			SetFlagsNZ(Reg.A = CPU::Deref_ABS_Y());
			++Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case LDA_IND_X: {
			SetFlagsNZ(Reg.A = CPU::Deref_IND_X());
			++Reg.PC;
			return 6;
		}break;
		case LDA_IND_Y: {
			SetFlagsNZ(Reg.A = CPU::Deref_IND_Y());
			++Reg.PC;
			return 5 + CPU::PageBoundaryCrossed();
		}break;


			// LDX
		case LDX_IMM: {
			SetFlagsNZ(Reg.X = CPU::Deref_IMM());
			++Reg.PC;
			return 2;
		}break;
		case LDX_ZERO: {
			SetFlagsNZ(Reg.X = CPU::Deref_ZERO());
			++Reg.PC;
			return 3;
		}break;
		case LDX_ZERO_Y: {
			SetFlagsNZ(Reg.X = CPU::Deref_ZERO_Y());
			++Reg.PC;
			return 4;
		}break;
		case LDX_ABS: {
			SetFlagsNZ(Reg.X = CPU::Deref_ABS());
			++Reg.PC;
			return 4;
		}break;
		case LDX_ABS_Y: {
			SetFlagsNZ(Reg.X = CPU::Deref_ABS_Y());
			++Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;


			// LDY
		case LDY_IMM: {
			SetFlagsNZ(Reg.Y = CPU::Deref_IMM());
			++Reg.PC;
			return 2;
		}break;
		case LDY_ZERO: {
			SetFlagsNZ(Reg.Y = CPU::Deref_ZERO());
			++Reg.PC;
			return 3;
		}break;
		case LDY_ZERO_X: {
			SetFlagsNZ(Reg.Y = CPU::Deref_ZERO_X());
			++Reg.PC;
			return 4;
		}break;
		case LDY_ABS: {
			SetFlagsNZ(Reg.Y = CPU::Deref_ABS());
			++Reg.PC;
			return 4;
		}break;
		case LDY_ABS_X: {
			SetFlagsNZ(Reg.Y = CPU::Deref_ABS_X());
			++Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;


			// LSR
		case LSR_ACC: {
			Status.C = (Reg.A & 0x1);
			SetFlagsNZ(Reg.A = Reg.A >> 1);
			++Reg.PC;
			return 2;
		}break;
		case LSR_ZERO: {
			LSR(CPU::GetAddr_ZERO());
			++Reg.PC;
			return 5;
		}break;
		case LSR_ZERO_X: {
			LSR(CPU::GetAddr_ZERO_X());
			++Reg.PC;
			return 6;
		}break;
		case LSR_ABS: {
			LSR(CPU::GetAddr_ABS());
			++Reg.PC;
			return 6;
		}break;
		case LSR_ABS_X: {
			LSR(CPU::GetAddr_ABS_X());
			++Reg.PC;
			return 7;
		}break;


			// NOP
		case NOP: {
			++Reg.PC;
			return 2;
		}break;


			// ORA
		case ORA_IMM: {
			SetFlagsNZ(Reg.A |= CPU::Deref_IMM());
			++Reg.PC;
			return 2;
		}break;
		case ORA_ZERO: {
			SetFlagsNZ(Reg.A |= CPU::Deref_ZERO());
			++Reg.PC;
			return 3;
		}break;
		case ORA_ZERO_X: {
			SetFlagsNZ(Reg.A |= CPU::Deref_ZERO_X());
			++Reg.PC;
			return 4;
		}break;
		case ORA_ABS: {
			SetFlagsNZ(Reg.A |= CPU::Deref_ABS());
			++Reg.PC;
			return 4;
		}break;
		case ORA_ABS_X: {
			SetFlagsNZ(Reg.A |= CPU::Deref_ABS_X());
			++Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case ORA_ABS_Y: {
			SetFlagsNZ(Reg.A |= CPU::Deref_ABS_Y());
			++Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case ORA_IND_X: {
			SetFlagsNZ(Reg.A |= CPU::Deref_IND_X());
			++Reg.PC;
			return 6;
		}break;
		case ORA_IND_Y: {
			SetFlagsNZ(Reg.A |= CPU::Deref_IND_Y());
			++Reg.PC;
			return 5 + CPU::PageBoundaryCrossed();
		}break;


			// Register Instructions 
		case TAX: {
			SetFlagsNZ(Reg.X = Reg.A);
			++Reg.PC;
			return 2;
		}break;
		case TXA: {
			SetFlagsNZ(Reg.A = Reg.X);
			++Reg.PC;
			return 2;
		}break;
		case DEX: {
			SetFlagsNZ(--Reg.X);
			++Reg.PC;
			return 2;
		}break;
		case INX: {
			SetFlagsNZ(++Reg.X);
			++Reg.PC;
			return 2;
		}break;
		case TAY: {
			SetFlagsNZ(Reg.Y = Reg.A);
			++Reg.PC;
			return 2;
		}break;
		case TYA: {
			SetFlagsNZ(Reg.A = Reg.Y);
			++Reg.PC;
			return 2;
		}break;
		case DEY: {
			SetFlagsNZ(--Reg.Y);
			++Reg.PC;
			return 2;
		}break;
		case INY: {
			SetFlagsNZ(++Reg.Y);
			++Reg.PC;
			return 2;
		}break;


			// ROL
		case ROL_ACC: {
			Word8bit c = Status.C;
			Status.C = Reg.A & 0x80 >> 7;
			Reg.A = Reg.A << 1;
			PASTE_BIT(Reg.A, 0, c);
			++Reg.PC;
			return 2;
		}break;
		case ROL_ZERO: {
			ROL(CPU::GetAddr_ZERO());
			++Reg.PC;
			return 5;
		}break;
		case ROL_ZERO_X: {
			ROL(CPU::GetAddr_ZERO_X());
			++Reg.PC;
			return 6;
		}break;
		case ROL_ABS: {
			ROL(CPU::GetAddr_ABS());
			++Reg.PC;
			return 6;
		}break;
		case ROL_ABS_X: {
			ROL(CPU::GetAddr_ABS_X());
			++Reg.PC;
			return 7;
		}break;


			// ROR
		case ROR_ACC: {
			Word8bit c = Status.C;
			Status.C = Reg.A & 0x1;
			Reg.A = Reg.A >> 1;
			PASTE_BIT(Reg.A, 7, c);
			++Reg.PC;
			return 2;
		}break;
		case ROR_ZERO: {
			ROR(CPU::GetAddr_ZERO());
			++Reg.PC;
			return 5;
		}break;
		case ROR_ZERO_X: {
			ROR(CPU::GetAddr_ZERO_X());
			++Reg.PC;
			return 6;
		}break;
		case ROR_ABS: {
			ROR(CPU::GetAddr_ABS());
			++Reg.PC;
			return 6;
		}break;
		case ROR_ABS_X: {
			ROR(CPU::GetAddr_ABS_X());
			++Reg.PC;
			return 7;
		}break;


			// RTI
		case RTI_IMP: {
			Status.all_flags = CPU::Stack_Pull();
			Word8bit LSB = CPU::Stack_Pull();
			Word8bit MSB = CPU::Stack_Pull();
			Reg.PC = CPU::LinkBytes(LSB, MSB);
			return 6;
		}break;


			// RTS
		case RTS: {
			Word8bit LSB = CPU::Stack_Pull();
			Word8bit MSB = CPU::Stack_Pull();
			Reg.PC = CPU::LinkBytes(LSB, MSB);
			++Reg.PC;
			return 6;
		}break;


			// SBC
		case SBC_IMM: {
			SBC(CPU::Deref_IMM());
			++Reg.PC;
			return 2;
		}break;
		case SBC_ZERO: {
			SBC(CPU::Deref_ZERO());
			++Reg.PC;
			return 3;
		}break;
		case SBC_ZERO_X: {
			SBC(CPU::Deref_ZERO_X());
			++Reg.PC;
			return 4;
		}break;
		case SBC_ABS: {
			SBC(CPU::Deref_ABS());
			++Reg.PC;
			return 4;
		}break;
		case SBC_ABS_X: {
			SBC(CPU::Deref_ABS_X());
			++Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case SBC_ABS_Y: {
			SBC(CPU::Deref_ABS_Y());
			++Reg.PC;
			return 4 + CPU::PageBoundaryCrossed();
		}break;
		case SBC_IND_X: {
			SBC(CPU::Deref_IND_X());
			++Reg.PC;
			return 6;
		}break;
		case SBC_IND_Y: {
			SBC(CPU::Deref_IND_Y());
			++Reg.PC;
			return 5 + CPU::PageBoundaryCrossed();
		}break;


			// STA
		case STA_ZERO: {
			Memory[CPU::GetAddr_ZERO()].Write(Reg.A);
			++Reg.PC;
			return 3;
		}break;
		case STA_ZERO_X: {
			Memory[CPU::GetAddr_ZERO_X()].Write(Reg.A);
			++Reg.PC;
			return 4;
		}break;
		case STA_ABS: {
			Memory[CPU::GetAddr_ABS()].Write(Reg.A);
			++Reg.PC;
			return 4;
		}break;
		case STA_ABS_X: {
			Memory[CPU::GetAddr_ABS_X()].Write(Reg.A);
			++Reg.PC;
			return 5;
		}break;
		case STA_ABS_Y: {
			Memory[CPU::GetAddr_ABS_Y()].Write(Reg.A);
			++Reg.PC;
			return 5;
		}break;
		case STA_IND_X: {
			Memory[CPU::GetAddr_IND_X()].Write(Reg.A);
			++Reg.PC;
			return 6;
		}break;
		case STA_IND_Y: {
			Memory[CPU::GetAddr_IND_Y()].Write(Reg.A);
			++Reg.PC;
			return 6;
		}break;


			// Stack Instructions
		case TXS: {
			Reg.SP = Reg.X & 0xFF;
			++Reg.PC;
			return 2;
		}break;
		case TSX: {
			SetFlagsNZ(Reg.X = Reg.SP & 0xFF);
			++Reg.PC;
			return 2;
		}break;
		case PHA: {
			CPU::Stack_Push(Reg.A);
			++Reg.PC;
			return 3;
		}break;
		case PLA: {
			SetFlagsNZ(Reg.A = CPU::Stack_Pull());
			++Reg.PC;
			return 4;
		}break;
		case PHP: {
			volatile StatusRegisters p;
			p.all_flags = Status.all_flags;
			p.B = 1;
			CPU::Stack_Push(p.all_flags);
			++Reg.PC;
			return 3;
		}break;
		case PLP: {
			Status.all_flags = CPU::Stack_Pull();
			++Reg.PC;
			return 4;
		}break;



			// STX
		case STX_ZERO: {
			Memory[CPU::GetAddr_ZERO()].Write(Reg.X);
			++Reg.PC;
			return 3;
		}break;
		case STX_ZERO_Y: {
			Memory[CPU::GetAddr_ZERO_Y()].Write(Reg.X);
			++Reg.PC;
			return 4;
		}break;
		case STX_ABS: {
			Memory[CPU::GetAddr_ABS()].Write(Reg.X);
			++Reg.PC;
			return 4;
		}break;


			// STY
		case STY_ZERO: {
			Memory[CPU::GetAddr_ZERO()].Write(Reg.Y);
			++Reg.PC;
			return 3;
		}break;
		case STY_ZERO_X: {
			Memory[CPU::GetAddr_ZERO_X()].Write(Reg.Y);
			++Reg.PC;
			return 4;
		}break;
		case STY_ABS: {
			Memory[CPU::GetAddr_ABS()].Write(Reg.Y);
			++Reg.PC;
			return 4;
		}break;


		default: {
			// illegal instructions are not supported in this version.
			//LOG("(Error:%hhX), ", opcode);
			++Reg.PC;
			return 1;
		}break;
		}

	}

}




