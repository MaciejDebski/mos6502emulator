#include "stdafx.h"
#include "CPU.h"
#include "Memory_NES.h"
#include "Opcodes_mos6502.h"

namespace mos6502emu {
	MemoryCell Memory[65535];

	namespace CPU {
		StatusRegisters Status;
		ProcessorRegisters Reg;

		bool bIsNotInitialized = 1;
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
#define __IND__(addr_LSbyte, addr_MSbyte) (__ABS__(Memory[__ABS__((addr_LSbyte), (addr_MSbyte))].data, Memory[(__ABS__((addr_LSbyte), (addr_MSbyte)) + 0x1) & 0xFFFF].data))
#define __IND_X__(addr_LSbyte) (__ABS__(Memory[__ZERO__((addr_LSbyte) + Reg.X)].data, Memory[__ZERO__((addr_LSbyte) + Reg.X + 0x1)].data)) // Indirect X adds X register to 8 bit address, wrapping around if 0xFF is overstepped.
#define __IND_Y__(addr_LSbyte) ( AddTestingPageBoundary( (__ABS__(Memory[__ZERO__((addr_LSbyte))].data, Memory[__ZERO__((addr_LSbyte) + 0x1)].data)), Reg.Y )) // Indirect Y adds Y register to full 16 bit address, wrapping around if 0xFFFF is overstepped.

		static void PowerUp() {
			Memory[0x4017] = 0x0;
			Memory[0x4015] = 0x0;
			for (int i = 0; i < 0x10; ++i) {
				Memory[0x4000 + i] = 0x0; // APU noise channels;
			}
		}

		CyclesUsed Tick() {
			return ExecuteOpcode(Memory[Reg.PC].data) + InterruptCheck();
		}

		bool PageBoundaryCrossed() {
			bool extra_cycle = bPageBoundaryCrossed;
			bPageBoundaryCrossed = 0;
			return extra_cycle;
		}

		static Word16bit AddTestingPageBoundary(Word16bit address, Word8bit LSB) {
			bPageBoundaryCrossed = ( ((address & 0xFF) + LSB) > 255);
			return address + LSB;
		}


		// GET ADDRESS
		Fast16bit GetAddr_(Fast8bit LSbyte, Fast8bit MSbyte) {
			return __ABS__(LSbyte, MSbyte);
		}

		Fast16bit GetAddr_ABS() {
			Word8bit LSB = Memory[++Reg.PC].data;
			Word8bit MSB = Memory[++Reg.PC].data;
			return __ABS__(LSB, MSB);
		}

		Fast16bit GetAddr_ABS_X() {
			Word8bit LSB = Memory[++Reg.PC].data;
			Word8bit MSB = Memory[++Reg.PC].data;
			return __ABS_X__(LSB, MSB);
		}

		Fast16bit GetAddr_ABS_Y() {
			Word8bit LSB = Memory[++Reg.PC].data;
			Word8bit MSB = Memory[++Reg.PC].data;
			return __ABS_Y__(LSB, MSB);
		}

		Fast16bit GetAddr_ZERO() {
			return __ZERO__(Memory[++Reg.PC].data);
		}

		Fast16bit GetAddr_ZERO_X() {
			return __ZERO_X__(Memory[++Reg.PC].data);
		}

		Fast16bit GetAddr_ZERO_Y() {
			return __ZERO_Y__(Memory[++Reg.PC].data);
		}

		Fast16bit GetAddr_REL() {
			return __REL__(Memory[++Reg.PC].data);
		}

		Fast16bit GetAddr_IND() {
			Word8bit LSB = Memory[++Reg.PC].data;
			Word8bit MSB = Memory[++Reg.PC].data;
			return __IND__(LSB, MSB);
		}

		Fast16bit GetAddr_IND_X() {
			return __IND_X__(Memory[++Reg.PC].data);
		}

		Fast16bit GetAddr_IND_Y() {
			return __IND_Y__(Memory[++Reg.PC].data);
		}

		// DEREFERENCE
		Fast8bit Deref_IMM() {
			return Memory[++Reg.PC].Read();
		}

		Fast8bit Deref_ABS() {
			return Memory[GetAddr_ABS()].Read();
		}

		Fast8bit Deref_ABS_X() {
			return Memory[GetAddr_ABS_X()].Read();
		}

		Fast8bit Deref_ABS_Y() {
			return Memory[GetAddr_ABS_Y()].Read();
		}

		Fast8bit Deref_ZERO() {
			return Memory[GetAddr_ZERO()].Read();
		}

		Fast8bit Deref_ZERO_X() {
			return Memory[GetAddr_ZERO_X()].Read();
		}

		Fast8bit Deref_ZERO_Y() {
			return Memory[GetAddr_ZERO_Y()].Read();
		}
		
		Fast8bit Deref_REL() {
			return Memory[GetAddr_REL()].Read();
		}

		Fast8bit Deref_IND() {
			return Memory[GetAddr_IND()].Read();
		}

		Fast8bit Deref_IND_X() {
			return Memory[GetAddr_IND_X()].Read();
		}

		Fast8bit Deref_IND_Y() {
			return Memory[GetAddr_IND_Y()].Read();
		}


		void Stack_Push(Fast8bit data) {
			--Reg.SP;
			if (Stack_IsFull()) {
				// TODO: throw INT;
				return;
			}

			Memory[0x0100 + Reg.SP + 1].data = data;
		}

		Fast8bit Stack_Pull() {
			++Reg.SP;
			if (Stack_IsEmpty()) {
				// TODO: throw INT;
			}

			return Memory[0x0100 + Reg.SP].data;
		}

		bool Stack_IsEmpty() {
			return Reg.SP == 0x00;
		}

		bool Stack_IsFull() {
			return Reg.SP == 0xFF;
		}

		static void Stack_FakePush() {
			// FakePush is a push executed in read mode, so no values are overwrited on the stack.
			// It does not throw overflow as it is used mainly by reset sequence to initialize stack.
			--Reg.SP;
		}

		static void Interrupt(Fast16bit VectorLow, Fast16bit VectorHigh, Fast8bit flag_B) {
			Stack_Push((Reg.PC & 0xFF00) >> 8); // Push PC High byte on stack
			Stack_Push(Reg.PC & 0xFF);	// Push PC Low byte on stack
			volatile StatusRegisters p;
			p.all_flags = Status.all_flags;
			p.B = flag_B;
			Stack_Push(p.all_flags);
			Reg.PC = __ABS__(Memory[VectorLow], Memory[VectorHigh]);
			Status.I = 1;
		}

		CyclesUsed InterruptCheck() {
			if (bNMI) {
				bNMI = false;
				return CPU::NMI();
			}
			else if (bIRQ) {
				bIRQ = false;
				return CPU::IRQ();
			}
			return 0;
		}

		CyclesUsed BRK() {
			Interrupt(0xFFFE, 0xFFFF, 1);
			return 7;
		}

		CyclesUsed IRQ() {
			Interrupt(0xFFFE, 0xFFFF, 0);
			return 7;
		}

		CyclesUsed NMI() {
			Interrupt(0xFFFA, 0xFFFB, 0);
			return 7;
		}

		CyclesUsed RESET() {
			if (bIsNotInitialized) {
				PowerUp();
				bIsNotInitialized = 0;
			}
			Stack_FakePush();	// Only in the NMOS version of 6502?
			Stack_FakePush();
			Stack_FakePush();
			Reg.PC = __ABS__(Memory[0xFFFC], Memory[0xFFFD]);
			Status.I = 1;
			Memory[0x4015] = 0x0; // NES version of mos6502 silencing APU at reset.
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



