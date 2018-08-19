#pragma once

#include "mos6502emu.h"

namespace mos6502emu {
	namespace CPU {
		static union StatusRegisters {
			struct {
				Fast8bit C : 1;
				Fast8bit Z : 1;
				Fast8bit I : 1;
				Fast8bit D : 1; // ignored on NES's mos6502.
				Fast8bit B : 1;
				Fast8bit reserved : 1;
				Fast8bit V : 1;
				Fast8bit N : 1;
			};
			Fast8bit all_flags = 0x34;
		} Status;

		static struct ProcessorRegisters {
			Word16bit PC = InitialPC;
			Fast8bit A = 0;
			Fast8bit X = 0;
			Fast8bit Y = 0;
			Word8bit SP = 0x00;
			union StatusRegisters* P = &Status;
		} Reg;

		typedef Fast8bit MemoryCell;
		extern MemoryCell Memory[65535];

		inline void PowerUp();
		inline CyclesUsed Tick();

		inline bool PageBoundaryCrossed();

		inline Fast16bit GetAddr_(Fast8bit LSbyte, Fast8bit MSbyte);
		inline Fast16bit GetAddr_ABS();
		inline Fast16bit GetAddr_ABS_X();
		inline Fast16bit GetAddr_ABS_Y();
		inline Fast16bit GetAddr_ZERO();
		inline Fast16bit GetAddr_ZERO_X();
		inline Fast16bit GetAddr_ZERO_Y();
		inline Fast16bit GetAddr_REL();
		inline Fast16bit GetAddr_IND();
		inline Fast16bit GetAddr_IND_X();
		inline Fast16bit GetAddr_IND_Y();

		inline void Write_ACC(Fast8bit data);
		inline void Write_ABS(Fast8bit data, Fast8bit addr_LSbyte, Fast8bit addr_MSbyte);
		inline void Write_ABS_X(Fast8bit data, Fast8bit addr_LSbyte, Fast8bit addr_MSbyte);
		inline void Write_ABS_Y(Fast8bit data, Fast8bit addr_LSbyte, Fast8bit addr_MSbyte);
		inline void Write_ZERO(Fast8bit data, Fast8bit addr_LSbyte);
		inline void Write_ZERO_X(Fast8bit data, Fast8bit addr_LSbyte);
		inline void Write_ZERO_Y(Fast8bit data, Fast8bit addr_LSbyte);
		inline void Write_REL(Fast8bit data, Fast8bit addr_LSbyte);
		inline void Write_IND(Fast8bit data, Fast8bit addr_LSbyte, Fast8bit addr_MSbyte);
		inline void Write_IND_X(Fast8bit data, Fast8bit addr_LSbyte);
		inline void Write_IND_Y(Fast8bit data, Fast8bit addr_LSbyte);

		inline Fast8bit Deref_PC();
		inline Fast8bit Deref_ABS();
		inline Fast8bit Deref_ABS_X();
		inline Fast8bit Deref_ABS_Y();
		inline Fast8bit Deref_ZERO();
		inline Fast8bit Deref_ZERO_X();
		inline Fast8bit Deref_ZERO_Y();
		inline Fast8bit Deref_REL();
		inline Fast8bit Deref_IND();
		inline Fast8bit Deref_IND_X();
		inline Fast8bit Deref_IND_Y();

		inline void Stack_Push(Fast8bit data);
		inline Fast8bit Stack_Pull();
		inline bool Stack_IsEmpty();
		inline bool Stack_IsFull();

		static bool bNMI = false;
		static bool bIRQ = false;

		inline CyclesUsed InterruptCheck();

		inline CyclesUsed BRK();
		inline CyclesUsed IRQ();
		inline CyclesUsed NMI();
		inline CyclesUsed RESET();

	}
}



