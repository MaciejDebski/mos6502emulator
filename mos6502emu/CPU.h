#pragma once

#include "mos6502emu.h"

namespace mos6502emu {
	extern class MemoryCell Memory[0xFFFF + 1];
	extern StatusRegisters Status;
	extern ProcessorRegisters Reg;

	namespace CPU {
		CyclesUsed Tick();

		bool PageBoundaryCrossed();

		Fast16bit GetAddr_(Fast8bit LSbyte, Fast8bit MSbyte);
		Fast16bit GetAddr_ABS();
		Fast16bit GetAddr_ABS_X();
		Fast16bit GetAddr_ABS_Y();
		Fast16bit GetAddr_ZERO();
		Fast16bit GetAddr_ZERO_X();
		Fast16bit GetAddr_ZERO_Y();
		Fast16bit GetAddr_REL();
		Fast16bit GetAddr_IND();
		Fast16bit GetAddr_IND_X();
		Fast16bit GetAddr_IND_Y();

		Fast8bit Deref_IMM();
		Fast8bit Deref_ABS();
		Fast8bit Deref_ABS_X();
		Fast8bit Deref_ABS_Y();
		Fast8bit Deref_ZERO();
		Fast8bit Deref_ZERO_X();
		Fast8bit Deref_ZERO_Y();
		Fast8bit Deref_REL();
		Fast8bit Deref_IND();
		Fast8bit Deref_IND_X();
		Fast8bit Deref_IND_Y();

		void Stack_Push(Fast8bit data);
		Fast8bit Stack_Pull();
		bool Stack_IsEmpty();
		bool Stack_IsFull();

		static bool bNMI = false;
		static bool bIRQ = false;

		CyclesUsed InterruptCheck();

		CyclesUsed BRK();
		CyclesUsed IRQ();
		CyclesUsed NMI();
		CyclesUsed RESET();

	}
}



