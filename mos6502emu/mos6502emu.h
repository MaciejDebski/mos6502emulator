#pragma once

#include <stdint.h>



/*
This mos6502 emulator does not support:
 - Decimal mode,

*/


namespace mos6502emu {
	typedef uint_fast32_t CyclesUsed;
	typedef uint_fast16_t Fast16bit;
	typedef uint_fast8_t Fast8bit;
	typedef uint16_t Word16bit;
	typedef uint8_t Word8bit;
	typedef int_fast8_t U2Word8bit;

	// Parameters
	static const float CPUClockSpeedHZ = 21477272;
	static const float CPUCycleLength_Default = 1 / (CPUClockSpeedHZ / 12);
	static const float PPUCycleLength_Default = 1 / (CPUClockSpeedHZ / 4);
	static float CPUCycleLength = CPUCycleLength_Default;
	static float PPUCycleLength = PPUCycleLength_Default;
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
	void NMI();
	void IRQ();
	void ClearDeltatimeBuffer();
	void SetPC(uint_least16_t newPC);

	// Debug
	void SetDebugCallback(void(*callback)(const char* message, int count));

}




