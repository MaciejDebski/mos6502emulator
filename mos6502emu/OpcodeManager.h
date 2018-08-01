#pragma once

namespace mos6502emu {
		enum OPCODES {
			ADC_IMM = 0x69,
			ADC_ZERO = 0x65,
			ADC_ZERO_X = 0x75,
			ADC_ABS = 0x6D,
			ADC_ABS_X = 0x7D,
			ADC_ABS_Y = 0x79,
			ADC_IND_X = 0x61,
			ADC_IND_Y = 0x71,

			AND_IMM = 0x29,
			AND_ZERO = 0x25,
			AND_ZERO_X = 0x35,
			AND_ABS = 0x2D,
			AND_ABS_X = 0x3D,
			AND_ABS_Y = 0x39,
			AND_IND_X = 0x21,
			AND_IND_Y = 0x31,

			ASL_ACC = 0x0A,
			ASL_ZERO = 0x06,
			ASL_ZERO_X = 0x16,
			ASL_ABS = 0x0E,
			ASL_ABS_X = 0x1E,

			BIT_ZERO = 0x24,
			BIT_ABS = 0x2C,

			BPL = 0x10,
			BMI = 0x30,
			BVC = 0x50,
			BVS = 0x70,
			BCC = 0x90,
			BCS = 0xB0,
			BNE = 0xD0,
			BEQ = 0xF0,

			BRK = 0x00,

			CMP_IMM = 0xC9,
			CMP_ZERO = 0xC5,
			CMP_ZERO_X = 0xD5,
			CMP_ABS = 0xCD,
			CMP_ABS_X = 0xDD,
			CMP_ABS_Y = 0xD9,
			CMP_IND_X = 0xC1,
			CMP_IND_Y = 0xD1,

			CPX_IMM = 0xE0,
			CPX_ZERO = 0xE4,
			CPX_ABS = 0xEC,

			CPY_IMM = 0xC0,
			CPY_ZERO = 0xC4,
			CPY_ABS = 0xCC,

			DEC_ZERO = 0xC6,
			DEC_ZERO_X = 0xD6,
			DEC_ABS = 0xCE,
			DEC_ABS_X = 0xDE,

			EOR_IMM = 0x49,
			EOR_ZERO = 0x45,
			EOR_ZERO_X = 0x55,
			EOR_ABS = 0x4D,
			EOR_ABS_X = 0x5D,
			EOR_ABS_Y = 0x59,
			EOR_IND_X = 0x41,
			EOR_IND_Y = 0x51,

			CLC = 0x18,
			SEC = 0x38,
			CLI = 0x58,
			SEI = 0x78,
			CLV = 0xB8,
			CLD = 0xD8,
			SED = 0xF8,

			INC_ZERO = 0xE6,
			INC_ZERO_X = 0xF6,
			INC_ABS = 0xEE,
			INC_ABS_X = 0xFE,

			JMP_ABS = 0x4C,
			JMP_IND = 0x6C,

			JSR_ABS = 0x20,

			LDA_IMM = 0xA9,
			LDA_ZERO = 0xA5,
			LDA_ZERO_X = 0xB5,
			LDA_ABS = 0xAD,
			LDA_ABS_X = 0xBD,
			LDA_ABS_Y = 0xB9,
			LDA_IND_X = 0xA1,
			LDA_IND_Y = 0xB1,

			LDX_IMM = 0xA2,
			LDX_ZERO = 0xA6,
			LDX_ZERO_Y = 0xB6,
			LDX_ABS = 0xAE,
			LDX_ABS_Y = 0xBE,

			LDY_IMM = 0xA0,
			LDY_ZERO = 0xA4,
			LDY_ZERO_X = 0xB4,
			LDY_ABS = 0xAC,
			LDY_ABS_X = 0xBC,

			LSR_ACC = 0x4A,
			LSR_ZERO = 0x46,
			LSR_ZERO_X = 0x56,
			LSR_ABS = 0x4E,
			LSR_ABS_X = 0x5E,

			NOP = 0xEA,

			ORA_IMM = 0x09,
			ORA_ZERO = 0x05,
			ORA_ZERO_X = 0x15,
			ORA_ABS = 0x0D,
			ORA_ABS_X = 0x1D,
			ORA_ABS_Y = 0x19,
			ORA_IND_X = 0x01,
			ORA_IND_Y = 0x11,

			TAX = 0xAA,
			TXA = 0x8A,
			DEX = 0xCA,
			INX = 0xE8,
			TAY = 0xA8,
			TYA = 0x98,
			DEY = 0x88,
			INY = 0xC8,

			ROL_ACC = 0x2A,
			ROL_ZERO = 0x26,
			ROL_ZERO_X = 0x36,
			ROL_ABS = 0x2E,
			ROL_ABS_X = 0x3E,

			ROR_ACC = 0x6A,
			ROR_ZERO = 0x66,
			ROR_ZERO_X = 0x76,
			ROR_ABS = 0x6E,
			ROR_ABS_X = 0x7E,

			RTI_IMP = 0x40,

			RTS = 0x60,

			SBC_IMM = 0xE9,
			SBC_ZERO = 0xE5,
			SBC_ZERO_X = 0xF5,
			SBC_ABS = 0xED,
			SBC_ABS_X = 0xFD,
			SBC_ABS_Y = 0xF9,
			SBC_IND_X = 0xE1,
			SBC_IND_Y = 0xF1,

			STA_ZERO = 0x85,
			STA_ZERO_X = 0x95,
			STA_ABS = 0x8D,
			STA_ABS_X = 0x9D,
			STA_ABS_Y = 0x99,
			STA_IND_X = 0x81,
			STA_IND_Y = 0x91,

			TXS = 0x9A,
			TSX = 0xBA,
			PHA = 0x48,
			PLA = 0x68,
			PHP = 0x08,
			PLP = 0x28,

			STX_ZERO = 0x86,
			STX_ZERO_Y = 0x96,
			STX_ABS = 0x8E,

			STY_ZERO = 0x84,
			STY_ZERO_X = 0x94,
			STY_ABS = 0x8C
		};

		typedef uint_fast32_t cycles_t;
		typedef uint_fast8_t opcodes_t;

		static inline cycles_t ExecuteOpcode(uint32_t opcode);

}




