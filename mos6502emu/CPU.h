#pragma once

#include "mos6502emu.h"

namespace mos6502emu {
	typedef Fast8bit MemoryCell;
	extern MemoryCell Memory[65535];

	namespace CPU {
		union StatusRegisters {
			struct {
				volatile Fast8bit C : 1;
				volatile Fast8bit Z : 1;
				volatile Fast8bit I : 1;
				volatile Fast8bit D : 1; // ignored on NES's mos6502.
				volatile Fast8bit B : 1;
				volatile Fast8bit reserved : 1;
				volatile Fast8bit V : 1;
				volatile Fast8bit N : 1;
			};
			volatile Fast8bit all_flags;
			StatusRegisters() : all_flags(0x34) {};
		};

		extern StatusRegisters Status;

		struct ProcessorRegisters {
			volatile Word16bit PC;
			volatile Fast8bit A;
			volatile Fast8bit X;
			volatile Fast8bit Y;
			volatile Word8bit SP;
			volatile union StatusRegisters* P = &Status;
			ProcessorRegisters() : PC(InitialPC), A(0x0), X(0x0), Y(0x0), SP(0x00) {};
		};

		extern ProcessorRegisters Reg;

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

		void Write_ACC(Fast8bit data);
		void Write_ABS(Fast8bit data, Fast8bit addr_LSbyte, Fast8bit addr_MSbyte);
		void Write_ABS_X(Fast8bit data, Fast8bit addr_LSbyte, Fast8bit addr_MSbyte);
		void Write_ABS_Y(Fast8bit data, Fast8bit addr_LSbyte, Fast8bit addr_MSbyte);
		void Write_ZERO(Fast8bit data, Fast8bit addr_LSbyte);
		void Write_ZERO_X(Fast8bit data, Fast8bit addr_LSbyte);
		void Write_ZERO_Y(Fast8bit data, Fast8bit addr_LSbyte);
		void Write_REL(Fast8bit data, Fast8bit addr_LSbyte);
		void Write_IND(Fast8bit data, Fast8bit addr_LSbyte, Fast8bit addr_MSbyte);
		void Write_IND_X(Fast8bit data, Fast8bit addr_LSbyte);
		void Write_IND_Y(Fast8bit data, Fast8bit addr_LSbyte);

		Fast8bit Deref_PC();
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



