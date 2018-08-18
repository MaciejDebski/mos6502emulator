#include "stdafx.h"
#include "CPU.h"
#include "Memory_NES.h"
#include "Opcodes_mos6502.h"

namespace mos6502emu {
	namespace CPU {
		MemoryCell Memory[65535];
		bool bPageBoundaryCrossed;

#define __ABS__(addr_LSbyte, addr_MSbyte) ((0xFF00 & ((addr_MSbyte) << 8)) | (0x00FF & (addr_LSbyte)))
#define __ABS_X__(addr_LSbyte, addr_MSbyte) ( AddTestingPageBoundary( ((0xFF00 & ((addr_MSbyte) << 8)) | (0x00FF & (addr_LSbyte))), Reg.X ))
#define __ABS_Y__(addr_LSbyte, addr_MSbyte) ( AddTestingPageBoundary( ((0xFF00 & ((addr_MSbyte) << 8)) | (0x00FF & (addr_LSbyte))), Reg.Y ))
#define __ZERO__(addr_LSbyte) (0xFF & (addr_LSbyte))
#define __ZERO_X__(addr_LSbyte) (0xFF & ((addr_LSbyte) + Reg.X))
#define __ZERO_Y__(addr_LSbyte) (0xFF & ((addr_LSbyte) + Reg.Y))
//#define __IMM__(addr_LSbyte, addr_MSbyte) // Immediate doesn't deal with memory addresses.
#define __REL__(addr_LSbyte) ((addr_LSbyte & 0x80) >> 7 == 1)? ((Reg.PC - negativeU2toUNS(addr_LSbyte) + 0x1) & 0xFFFF) : ((Reg.PC + (0xFF & (addr_LSbyte)) + 0x1) & 0xFFFF)
//#define __IMP__(addr_LSbyte, addr_MSbyte) // Implicit doesn't deal with memory addresses.
#define __IND__(addr_LSbyte, addr_MSbyte) (__ABS__(Memory[__ABS__((addr_LSbyte), (addr_MSbyte))], Memory[(__ABS__((addr_LSbyte), (addr_MSbyte)) + 0x1) & 0xFFFF]))
#define __IND_X__(addr_LSbyte) (__ABS__(Memory[__ZERO__((addr_LSbyte) + Reg.X)], Memory[__ZERO__((addr_LSbyte) + Reg.X + 0x1)])) // Indirect X adds X register to 8 bit address, wrapping around if 0xFF is overstepped.
#define __IND_Y__(addr_LSbyte) ( AddTestingPageBoundary( (__ABS__(Memory[__ZERO__((addr_LSbyte))], Memory[__ZERO__((addr_LSbyte) + 0x1)])), Reg.Y )) // Indirect Y adds Y register to full 16 bit address, wrapping around if 0xFFFF is overstepped.

		inline CyclesUsed Tick() {
			return ExecuteOpcode(Memory[Reg.PC++]) + InterruptCheck();
		}

		inline bool PageBoundaryCrossed() {
			bool extra_cycle = bPageBoundaryCrossed;
			bPageBoundaryCrossed = 0;
			return extra_cycle;
		}

		static Fast16bit AddTestingPageBoundary(Fast16bit A, Fast16bit B) {
			Fast16bit result = (A + B) & 0xFFFF;
			bPageBoundaryCrossed = (( result > 255));
			return result;
		}


		// GET ADDRESS
		inline Fast16bit GetAddr_(Fast8bit LSbyte, Fast8bit MSbyte) {
			return __ABS__(LSbyte, MSbyte);
		}

		inline Fast16bit GetAddr_ABS() {
			return __ABS__(Memory[++Reg.PC], Memory[++Reg.PC]);
		}

		inline Fast16bit GetAddr_ABS_X() {
			return __ABS_X__(Memory[++Reg.PC], Memory[++Reg.PC]);
		}

		inline Fast16bit GetAddr_ABS_Y() {
			return __ABS_Y__(Memory[++Reg.PC], Memory[++Reg.PC]);
		}

		inline Fast16bit GetAddr_ZERO() {
			return __ZERO__(Memory[++Reg.PC]);
		}

		inline Fast16bit GetAddr_ZERO_X() {
			return __ZERO_X__(Memory[++Reg.PC]);
		}

		inline Fast16bit GetAddr_ZERO_Y() {
			return __ZERO_Y__(Memory[++Reg.PC]);
		}

		inline Fast16bit GetAddr_REL() {
			return __REL__(Memory[++Reg.PC]);
		}

		inline Fast16bit GetAddr_IND() {
			return __IND__(Memory[++Reg.PC], Memory[++Reg.PC]);
		}

		inline Fast16bit GetAddr_IND_X() {
			return __IND_X__(Memory[++Reg.PC]);
		}

		inline Fast16bit GetAddr_IND_Y() {
			return __IND_Y__(Memory[++Reg.PC]);
		}


		// WRITE
		inline void Write_ACC(Fast8bit data) {
			Reg.A = data;
		}

		inline void Write_ABS(Fast8bit data, Fast8bit addr_LSbyte, Fast8bit addr_MSbyte) {
			Memory[__ABS__(addr_LSbyte, addr_MSbyte)] = data;
		}

		inline void Write_ABS_X(Fast8bit data, Fast8bit addr_LSbyte, Fast8bit addr_MSbyte) {
			Memory[__ABS_X__(addr_LSbyte, addr_MSbyte)] = data;
		}

		inline void Write_ABS_Y(Fast8bit data, Fast8bit addr_LSbyte, Fast8bit addr_MSbyte) {
			Memory[__ABS_Y__(addr_LSbyte, addr_MSbyte)] = data;
		}

		inline void Write_ZERO(Fast8bit data, Fast8bit addr_LSbyte) {
			Memory[__ZERO__(addr_LSbyte)] = data;
		}

		inline void Write_ZERO_X(Fast8bit data, Fast8bit addr_LSbyte) {
			Memory[__ZERO_X__(addr_LSbyte)] = data;
		}

		inline void Write_ZERO_Y(Fast8bit data, Fast8bit addr_LSbyte) {
			Memory[__ZERO_Y__(addr_LSbyte)] = data;
		}

		inline void Write_REL(Fast8bit data, Fast8bit addr_LSbyte) {
			Memory[__REL__(addr_LSbyte)] = data;
		}

		inline void Write_IND(Fast8bit data, Fast8bit addr_LSbyte, Fast8bit addr_MSbyte) {
			Memory[__IND__(addr_LSbyte, addr_MSbyte)] = data;
		}

		inline void Write_IND_X(Fast8bit data, Fast8bit addr_LSbyte) {
			Memory[__IND_X__(addr_LSbyte)] = data;
		}

		inline void Write_IND_Y(Fast8bit data, Fast8bit addr_LSbyte) {
			Memory[__IND_Y__(addr_LSbyte)] = data;
		}



		// READ
		inline Fast8bit Deref_PC() {
			return Memory[++Reg.PC];
		}

		inline Fast8bit Deref_ABS() {
			return Memory[__ABS__(Memory[++Reg.PC], Memory[++Reg.PC])];
		}

		inline Fast8bit Deref_ABS_X() {
			return Memory[__ABS_X__(Memory[++Reg.PC], Memory[++Reg.PC])];
		}

		inline Fast8bit Deref_ABS_Y() {
			return Memory[__ABS_Y__(Memory[++Reg.PC], Memory[++Reg.PC])];
		}

		inline Fast8bit Deref_ZERO() {
			return Memory[__ZERO__(Memory[++Reg.PC])];
		}

		inline Fast8bit Deref_ZERO_X() {
			return Memory[__ZERO_X__(Memory[++Reg.PC])];
		}

		inline Fast8bit Deref_ZERO_Y() {
			return Memory[__ZERO_Y__(Memory[++Reg.PC])];
		}
		
		inline Fast8bit Deref_REL() {
			return Memory[__REL__(Memory[++Reg.PC])];
		}

		inline Fast8bit Deref_IND() {
			return Memory[__IND__(Memory[++Reg.PC], Memory[++Reg.PC])];
		}

		inline Fast8bit Deref_IND_X() {
			return Memory[__IND_X__(Memory[++Reg.PC])];
		}

		inline Fast8bit Deref_IND_Y() {
			return Memory[__IND_Y__(Memory[++Reg.PC])];
		}


		inline void Stack_Push(Fast8bit data) {
			--Reg.SP;
			if (Stack_IsFull()) {
				// TODO: throw INT;
				return;
			}

			Memory[0x0100 + Reg.SP + 1] = data;
		}

		inline Fast8bit Stack_Pull() {
			++Reg.SP;
			if (Stack_IsEmpty()) {
				// TODO: throw INT;
			}

			return Memory[0x0100 + Reg.SP];
		}

		inline bool Stack_IsEmpty() {
			return Reg.SP == 0x00;
		}

		inline bool Stack_IsFull() {
			return Reg.SP == 0xFF;
		}

		static inline void Stack_FakePush() {
			// FakePush is a push executed in read mode, so no values are overwrited on the stack.
			// It does not throw overflow as it is used mainly by reset sequence to initialize stack.
			--Reg.SP;
		}

		static inline void Interrupt(Fast16bit VectorLow, Fast16bit VectorHigh, Fast8bit flag_B) {
			Stack_Push((Reg.PC & 0xFF00) >> 8); // Push PC High byte on stack
			Stack_Push(Reg.PC & 0xFF);	// Push PC Low byte on stack
			StatusRegisters p = Status;
			p.B = flag_B;
			Stack_Push(p.all_flags);
			Reg.PC = __ABS__(Memory[VectorLow], Memory[VectorHigh]);
			Status.I = 1;
		}

		inline CyclesUsed InterruptCheck() {
			if (bNMI) {
				bNMI = false;
				return CPU::NMI();
			}
			else if (bIRQ) {
				bIRQ = false;
				return CPU::IRQ();
			}
		}

		inline CyclesUsed BRK() {
			Interrupt(0xFFFE, 0xFFFF, 1);
			return 7;
		}

		inline CyclesUsed IRQ() {
			Interrupt(0xFFFE, 0xFFFF, 0);
			return 7;
		}

		inline CyclesUsed NMI() {
			Interrupt(0xFFFA, 0xFFFB, 0);
			return 7;
		}

		inline CyclesUsed RESET() {
			Reg.SP = 0x00;
			Stack_FakePush();	// Only in the NMOS version of 6502?
			Stack_FakePush();
			Stack_FakePush();
			Reg.PC = __ABS__(Memory[0xFFFC], Memory[0xFFFD]);
			Status.I = 1;
			return 7;
		}


#undef __ABS__
#undef __ABS_X__
#undef __ABS_Y__
#undef __ZERO__
#undef __ZERO_X__
#undef __ZERO_Y__
//#undef __IMM__(addr_LSbyte, addr_MSbyte) // Immediate doesn't deal with memory addresses.
#undef __REL__
//#undef __IMP__(addr_LSbyte, addr_MSbyte) // Implicit doesn't deal with memory addresses.
#undef __IND__
#undef __IND_X__
#undef __IND_Y__



	}
}



