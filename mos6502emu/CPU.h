#pragma once

#include "mos6502emu.h"

namespace mos6502emu {
	extern MemoryBlock Memory;
	extern StatusRegisters Status;
	extern ProcessorRegisters Reg;
	extern Debug DebugLog;

	namespace CPU {
		CyclesUsed Tick();

		bool PageBoundaryCrossed();

		Word16bit LinkBytes(Word8bit LSByte, Word8bit MSByte);
		Word16bit GetAddr_ABS();
		Word16bit GetAddr_ABS_X();
		Word16bit GetAddr_ABS_Y();
		Word8bit GetAddr_ZERO();
		Word8bit GetAddr_ZERO_X();
		Word8bit GetAddr_ZERO_Y();
		Word16bit GetAddr_REL();
		Word16bit GetAddr_IND();
		Word16bit GetAddr_IND_X();
		Word16bit GetAddr_IND_Y();

		Word8bit Deref_IMM();
		Word8bit Deref_ABS();
		Word8bit Deref_ABS_X();
		Word8bit Deref_ABS_Y();
		Word8bit Deref_ZERO();
		Word8bit Deref_ZERO_X();
		Word8bit Deref_ZERO_Y();
		Word8bit Deref_REL();
		Word8bit Deref_IND();
		Word8bit Deref_IND_X();
		Word8bit Deref_IND_Y();

		void Stack_Push(Word8bit data);
		Word8bit Stack_Pull();
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



