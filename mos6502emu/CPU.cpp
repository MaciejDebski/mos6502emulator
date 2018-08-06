#include "stdafx.h"
#include "CPU.h"
#include "Memory_NES.h"
#include "Opcodes_mos6502.h"

namespace mos6502emu {
	namespace CPU {
		CyclesUsed Tick() {
			Word8bit current_opcode = GetData(ProcessorRegisters1.PC);
			return ExecuteOpcode(current_opcode);
		}

		inline Word8bit GetData(uint_least16_t PC) {
			return Memory[PC];
		}








	}
}



