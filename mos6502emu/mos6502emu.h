#pragma once

#include <stdint.h>

namespace mos6502emu {
	typedef uint_fast32_t CyclesUsed;
	typedef uint_fast16_t Word16bit;
	typedef uint_fast8_t Word8bit;
	typedef int_fast8_t U2Word8bit;

	// Parameters
	static float CPUCycleLength = 5.587301776501224e-7f;
	static float PPUCycleLength = 1.862433925500408e-7f;
	static uint_least16_t InitialPC = 0x00;

	// ROM
	const char* ROMCartridge;

	// Update
	void Update(float deltatime);

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




