#pragma once

#include "mos6502emu.h"

namespace mos6502emu {
	namespace CPU {

		enum AMODE {
			ACC,
			ABS_,
			ABS_X,
			ABS_Y,
			ZERO_,
			ZERO_X,
			ZERO_Y,
			IMM,
			REL,
			IMP,
			IND,
			IND_X,
			IND_Y
		};

		static union StatusRegisters {
			struct {
				Reg8bit N : 1;
				Reg8bit V : 1;
				Reg8bit reserved : 1;
				Reg8bit B : 1;
				Reg8bit D : 1;
				Reg8bit I : 1;
				Reg8bit Z : 1;
				Reg8bit C : 1;
			};
			Reg8bit all_flags = 0x34;
		} StatusRegisters1;

		static struct ProcessorRegisters {
			Reg16bit PC = InitialPC;
			Reg8bit A = 0;
			Reg8bit X = 0;
			Reg8bit Y = 0;
			Reg8bit SP = 0xff;
			union StatusRegisters* P = &StatusRegisters1;
		} ProcessorRegisters1;

		typedef Word8bit MemoryCell;
		MemoryCell Memory[65535];

		CyclesUsed Tick();
		inline Word8bit GetData(uint_least16_t PC);

		inline void Write(Word8bit addr_b1, Word8bit addr_b2, AMODE addressing_mode);
		inline void Read(Word8bit addr_b1, Word8bit addr_b2, AMODE addressing_mode);









	}
}



