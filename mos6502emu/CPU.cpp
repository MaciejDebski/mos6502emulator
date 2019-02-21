#include "stdafx.h"
#include "CPU.h"
#include "Memory_NES.h"
#include "Opcodes_mos6502.h"

namespace mos6502emu {
	MemoryCell Memory[0xFFFF + 1];
	StatusRegisters Status;
	ProcessorRegisters Reg;


	namespace CPU {
		bool bPageBoundaryCrossed;

		CyclesUsed Tick() {
			return ExecuteOpcode(Memory[Reg.PC].data) + InterruptCheck();
		}

		bool PageBoundaryCrossed() {
			bool extra_cycle = bPageBoundaryCrossed;
			bPageBoundaryCrossed = 0;
			return extra_cycle;
		}

		static Word16bit AddBytes_TestPageBoundary(Word16bit address, Word16bit register_value) {
			bPageBoundaryCrossed = (((address & 0xFF) + register_value) > 255);
			return address + register_value;
		}

		// GET ADDRESS
		static Word8bit ReadNextByte() {
			return Memory[++Reg.PC].data;
		}

		Word16bit LinkBytes(Word8bit LSByte, Word8bit MSByte) {
			return (MSByte << 8) | LSByte;
		}

		Word16bit GetAddr_ABS() {
			Word8bit LSByte = ReadNextByte();
			Word8bit MSByte = ReadNextByte();
			return LinkBytes(LSByte, MSByte);
		}

		Word16bit GetAddr_ABS_X() {
			Word16bit addr = GetAddr_ABS();
			AddBytes_TestPageBoundary(addr, Reg.X);
			return addr + Reg.X;
		}

		Word16bit GetAddr_ABS_Y() {
			Word16bit addr = GetAddr_ABS();
			AddBytes_TestPageBoundary(addr, Reg.Y);
			return addr + Reg.Y;
		}

		Word8bit GetAddr_ZERO() {
			return Memory[++Reg.PC].data;
		}

		Word8bit GetAddr_ZERO_X() {
			return GetAddr_ZERO() + Reg.X;
		}

		Word8bit GetAddr_ZERO_Y() {
			return GetAddr_ZERO() + Reg.Y;
		}

		Word16bit GetAddr_REL() {
			// Relative address mode is sensitive to sign, its scope is from -128, to 127
			Word8bit displacement = ReadNextByte();
			return AddBytes_TestPageBoundary(Reg.PC, mos6502emu::To16Bit(displacement)) + 0x1;
		}

		Word16bit GetAddr_IND() {
			Word16bit pointer = GetAddr_ABS();
			return LinkBytes(Memory[pointer], Memory[pointer + 1]);
		}

		Word16bit GetAddr_IND_X() {
			Word8bit pointer = GetAddr_ZERO() + Reg.X;
			return LinkBytes(Memory[pointer], Memory[pointer + 1]);
		}

		Word16bit GetAddr_IND_Y() {
			Word8bit pointer = GetAddr_ZERO();
			return LinkBytes(Memory[pointer], Memory[pointer + 1]) + Reg.Y;
		}

		// DEREFERENCE
		Word8bit Deref_IMM() {
			return Memory[++Reg.PC].Read();
		}

		Word8bit Deref_ABS() {
			return Memory[GetAddr_ABS()].Read();
		}

		Word8bit Deref_ABS_X() {
			return Memory[GetAddr_ABS_X()].Read();
		}

		Word8bit Deref_ABS_Y() {
			return Memory[GetAddr_ABS_Y()].Read();
		}

		Word8bit Deref_ZERO() {
			return Memory[GetAddr_ZERO()].Read();
		}

		Word8bit Deref_ZERO_X() {
			return Memory[GetAddr_ZERO_X()].Read();
		}

		Word8bit Deref_ZERO_Y() {
			return Memory[GetAddr_ZERO_Y()].Read();
		}
		
		Word8bit Deref_REL() {
			return Memory[GetAddr_REL()].Read();
		}

		Word8bit Deref_IND() {
			return Memory[GetAddr_IND()].Read();
		}

		Word8bit Deref_IND_X() {
			return Memory[GetAddr_IND_X()].Read();
		}

		Word8bit Deref_IND_Y() {
			return Memory[GetAddr_IND_Y()].Read();
		}


		void Stack_Push(Word8bit data) {
			--Reg.SP;
			if (Stack_IsFull()) {
				//LOG("::STACK_FULL::\n\n");
			}

			Memory[0x0100 + Reg.SP + 1].data = data;
		}

		Word8bit Stack_Pull() {
			++Reg.SP;
			if (Stack_IsEmpty()) {
				//LOG("::STACK_EMPTY::\n\n");
			}

			return Memory[0x0100 + Reg.SP].data;
		}

		bool Stack_IsEmpty() {
			return Reg.SP == 0x00;
		}

		bool Stack_IsFull() {
			return Reg.SP == 0xFF;
		}

		static void Interrupt(Word16bit VectorLow, Word16bit VectorHigh, Word8bit flag_B) {
			Stack_Push((Reg.PC & 0xFF00) >> 8); // Push PC High byte on stack
			Stack_Push(Reg.PC & 0xFF);	// Push PC Low byte on stack
			volatile StatusRegisters p;
			p.all_flags = Status.all_flags;
			p.B = flag_B;
			Stack_Push(p.all_flags);
			Reg.PC = LinkBytes(Memory[VectorLow], Memory[VectorHigh]);
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
			Reg.PC = LinkBytes(Memory[0xFFFC], Memory[0xFFFD]);
			Status.I = 1;
			return 7;
		}

	}
}



