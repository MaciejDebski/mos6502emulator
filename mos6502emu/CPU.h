#pragma once

#include "mos6502emu.h"

namespace mos6502emu {
	namespace CPU {
		static union StatusRegisters {
			struct {
				Word8bit C : 1;
				Word8bit Z : 1;
				Word8bit I : 1;
				Word8bit D : 1; // ignored on NES's mos6502.
				Word8bit B : 1;
				Word8bit reserved : 1;
				Word8bit V : 1;
				Word8bit N : 1;
			};
			Word8bit all_flags = 0x34;
		} Status;

		static struct ProcessorRegisters {
			Word16bit PC = InitialPC;
			Word8bit A = 0;
			Word8bit X = 0;
			Word8bit Y = 0;
			Word16bit SP = 0x01ff;
			union StatusRegisters* P = &Status;
		} Reg;

		typedef Word8bit MemoryCell;
		extern MemoryCell Memory[65535];

		inline CyclesUsed Tick();

		inline bool PageBoundaryCrossed();

		inline Word16bit GetAddr_ABS();
		inline Word16bit GetAddr_ABS_X();
		inline Word16bit GetAddr_ABS_Y();
		inline Word16bit GetAddr_ZERO();
		inline Word16bit GetAddr_ZERO_X();
		inline Word16bit GetAddr_ZERO_Y();
		inline Word16bit GetAddr_REL();
		inline Word16bit GetAddr_IND();
		inline Word16bit GetAddr_IND_X();
		inline Word16bit GetAddr_IND_Y();

		inline void Write_ACC(Word8bit data);
		inline void Write_ABS(Word8bit data, Word8bit addr_LSbyte, Word8bit addr_MSbyte);
		inline void Write_ABS_X(Word8bit data, Word8bit addr_LSbyte, Word8bit addr_MSbyte);
		inline void Write_ABS_Y(Word8bit data, Word8bit addr_LSbyte, Word8bit addr_MSbyte);
		inline void Write_ZERO(Word8bit data, Word8bit addr_LSbyte);
		inline void Write_ZERO_X(Word8bit data, Word8bit addr_LSbyte);
		inline void Write_ZERO_Y(Word8bit data, Word8bit addr_LSbyte);
		inline void Write_REL(Word8bit data, Word8bit addr_LSbyte);
		inline void Write_IND(Word8bit data, Word8bit addr_LSbyte, Word8bit addr_MSbyte);
		inline void Write_IND_X(Word8bit data, Word8bit addr_LSbyte);
		inline void Write_IND_Y(Word8bit data, Word8bit addr_LSbyte);

		inline Word8bit Read_PC();
		inline Word8bit Read_ABS();
		inline Word8bit Read_ABS_X();
		inline Word8bit Read_ABS_Y();
		inline Word8bit Read_ZERO();
		inline Word8bit Read_ZERO_X();
		inline Word8bit Read_ZERO_Y();
		inline Word8bit Read_REL();
		inline Word8bit Read_IND();
		inline Word8bit Read_IND_X();
		inline Word8bit Read_IND_Y();

		inline void Stack_Push(Word8bit data);
		inline Word8bit Stack_Pull();
		inline bool Stack_IsEmpty();
		inline bool Stack_IsFull();

	}
}



