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
	typedef Fast8bit MemoryCell;

	// Parameters
	static const float CPUClockSpeedHZ = 21477272;
	static const float CPUCycleLength_Default = 1 / (CPUClockSpeedHZ / 12);
	static const float PPUCycleLength_Default = 1 / (CPUClockSpeedHZ / 4);
	static float CPUCycleLength = CPUCycleLength_Default;
	static float PPUCycleLength = PPUCycleLength_Default;
	static Word16bit InitialPC = 0x4020;

	// Memory
	extern MemoryCell Memory[65535];

	// Initialization
	bool InsertROM(Fast8bit* rom, Word16bit size);
	bool InsertROM(Word16bit PC, Fast8bit* rom, Word16bit size);

	// Update
	void Update(float deltatime);

	// CPU
	void RealCPUTick(float deltatime);
	CyclesUsed TickCPU();

	// PPU
	void RealPPUTick(float deltatime);
	void TickPPU();
	void SetPPUCallback(void(*callback)());

	// Controls
	void Reset();
	void NMI();
	void IRQ();
	void ClearDeltatimeBuffer();
	void SetPC(Word16bit newPC);

	// Debug
	void SetDebugCallback(void(*callback)(const char* message, int count));

}




