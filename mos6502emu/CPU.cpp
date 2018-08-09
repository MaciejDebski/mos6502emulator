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
#define __REL__(addr_LSbyte) ((addr_LSbyte & 0x80) >> 7 == 1)? (Reg.PC - (0xFF & negativeU2toUNS(addr_LSbyte))) + 0x1 : Reg.PC + (0xFF & (addr_LSbyte) + 0x1)
//#define __IMP__(addr_LSbyte, addr_MSbyte) // Implicit doesn't deal with memory addresses.
#define __IND__(addr_LSbyte, addr_MSbyte) (__ABS__(Memory[__ABS__((addr_LSbyte), (addr_MSbyte))], Memory[__ABS__((addr_LSbyte), (addr_MSbyte)) + 0x1]))
#define __IND_X__(addr_LSbyte) (__ABS__(__ZERO__((addr_LSbyte) + Reg.X), __ZERO__((addr_LSbyte) + Reg.X + 0x1))) // Indirect X adds X register to 8 bit address, wrapping around if 0xFF is overstepped.
#define __IND_Y__(addr_LSbyte) ( AddTestingPageBoundary( (__ABS__(__ZERO__((addr_LSbyte)), __ZERO__((addr_LSbyte) + 0x1))), Reg.Y )) // Indirect Y adds Y register to full 16 bit address, wrapping around if 0xFFFF is overstepped.

		inline CyclesUsed Tick() {
			return ExecuteOpcode(Memory[++Reg.PC]);
		}

		inline bool PageBoundaryCrossed() {
			bool extra_cycle = bPageBoundaryCrossed;
			bPageBoundaryCrossed = 0;
			return extra_cycle;
		}

		static Word16bit AddTestingPageBoundary(Word16bit A, Word16bit B) {
			bPageBoundaryCrossed = ((A + B) > 255);
			return A + B;
		}



		// GET ADDRESS
		inline Word8bit GetAddr_ABS() {
			return __ABS__(Memory[++Reg.PC], Memory[++Reg.PC]);
		}

		inline Word8bit GetAddr_ABS_X() {
			return __ABS_X__(Memory[++Reg.PC], Memory[++Reg.PC]);
		}

		inline Word8bit GetAddr_ABS_Y() {
			return __ABS_Y__(Memory[++Reg.PC], Memory[++Reg.PC]);
		}

		inline Word8bit GetAddr_ZERO() {
			return __ZERO__(Memory[++Reg.PC]);
		}

		inline Word8bit GetAddr_ZERO_X() {
			return __ZERO_X__(Memory[++Reg.PC]);
		}

		inline Word8bit GetAddr_ZERO_Y() {
			return __ZERO_Y__(Memory[++Reg.PC]);
		}

		inline Word8bit GetAddr_REL() {
			return __REL__(Memory[++Reg.PC]);
		}

		inline Word8bit GetAddr_IND() {
			return __IND__(Memory[++Reg.PC], Memory[++Reg.PC]);
		}

		inline Word8bit GetAddr_IND_X() {
			return __IND_X__(Memory[++Reg.PC]);
		}

		inline Word8bit GetAddr_IND_Y() {
			return __IND_Y__(Memory[++Reg.PC]);
		}


		// WRITE
		inline void Write_ACC(Word8bit data) {
			Reg.A = data;
		}

		inline void Write_ABS(Word8bit data, Word8bit addr_LSbyte, Word8bit addr_MSbyte) {
			Memory[__ABS__(addr_LSbyte, addr_MSbyte)] = data;
		}

		inline void Write_ABS_X(Word8bit data, Word8bit addr_LSbyte, Word8bit addr_MSbyte) {
			Memory[__ABS_X__(addr_LSbyte, addr_MSbyte)] = data;
		}

		inline void Write_ABS_Y(Word8bit data, Word8bit addr_LSbyte, Word8bit addr_MSbyte) {
			Memory[__ABS_Y__(addr_LSbyte, addr_MSbyte)] = data;
		}

		inline void Write_ZERO(Word8bit data, Word8bit addr_LSbyte) {
			Memory[__ZERO__(addr_LSbyte)] = data;
		}

		inline void Write_ZERO_X(Word8bit data, Word8bit addr_LSbyte) {
			Memory[__ZERO_X__(addr_LSbyte)] = data;
		}

		inline void Write_ZERO_Y(Word8bit data, Word8bit addr_LSbyte) {
			Memory[__ZERO_Y__(addr_LSbyte)] = data;
		}

		inline void Write_REL(Word8bit data, Word8bit addr_LSbyte) {
			Memory[__REL__(addr_LSbyte)] = data;
		}

		inline void Write_IND(Word8bit data, Word8bit addr_LSbyte, Word8bit addr_MSbyte) {
			Memory[__IND__(addr_LSbyte, addr_MSbyte)] = data;
		}

		inline void Write_IND_X(Word8bit data, Word8bit addr_LSbyte) {
			Memory[__IND_X__(addr_LSbyte)] = data;
		}

		inline void Write_IND_Y(Word8bit data, Word8bit addr_LSbyte) {
			Memory[__IND_Y__(addr_LSbyte)] = data;
		}



		// READ
		inline Word8bit Read_PC() {
			return Memory[++Reg.PC];
		}

		inline Word8bit Read_ABS() {
			return Memory[__ABS__(Memory[++Reg.PC], Memory[++Reg.PC])];
		}

		inline Word8bit Read_ABS_X() {
			return Memory[__ABS_X__(Memory[++Reg.PC], Memory[++Reg.PC])];
		}

		inline Word8bit Read_ABS_Y() {
			return Memory[__ABS_Y__(Memory[++Reg.PC], Memory[++Reg.PC])];
		}

		inline Word8bit Read_ZERO() {
			return Memory[__ZERO__(Memory[++Reg.PC])];
		}

		inline Word8bit Read_ZERO_X() {
			return Memory[__ZERO_X__(Memory[++Reg.PC])];
		}

		inline Word8bit Read_ZERO_Y() {
			return Memory[__ZERO_Y__(Memory[++Reg.PC])];
		}
		
		inline Word8bit Read_REL() {
			return Memory[__REL__(Memory[++Reg.PC])];
		}

		inline Word8bit Read_IND() {
			return Memory[__IND__(Memory[++Reg.PC], Memory[++Reg.PC])];
		}

		inline Word8bit Read_IND_X() {
			return Memory[__IND_X__(Memory[++Reg.PC])];
		}

		inline Word8bit Read_IND_Y() {
			return Memory[__IND_Y__(Memory[++Reg.PC])];
		}


		inline void Stack_Push(Word8bit data) {
			if (Stack_IsFull()) {
				Reg.SP = 0x01FF;
				// throw INT;
			}

			Memory[Reg.SP--] = data;
		}

		inline Word8bit Stack_Pull() {
			if (Stack_IsEmpty()) {
				// throw INT;
			}

			return Memory[++Reg.SP];
		}

		inline bool Stack_IsEmpty() {
			return Reg.SP == 0x01FF;
		}

		inline bool Stack_IsFull() {
			return Reg.SP == 0x00FF;
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



