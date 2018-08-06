#pragma once

#include <stdint.h>

namespace mos6502emu {
	typedef uint_fast32_t CyclesUsed;
	typedef uint_least16_t Reg16bit;
	typedef uint_least8_t Reg8bit;
	typedef uint_fast8_t Word8bit;
	
	// Parameters
	extern float CPUCycleLength;
	extern float PPUCycleLength;
	extern uint_least16_t InitialPC;

	// ROM
	const char* ROMCartridge;

	// CPU
	void RealCPUTick(float deltatime);
	inline CyclesUsed TickCPU();

	// PPU
	inline void RealPPUTick(float deltatime);
	inline void TickPPU();
	void SetPPUCallback(void(*callback)());

	// Controls
	void Reset();
	void ClearDeltatimeBuffer();
	void SetPC(uint_least16_t newPC);

	// Debug
	void SetDebugCallback(void(*callback)(const char* message, int count));

}




