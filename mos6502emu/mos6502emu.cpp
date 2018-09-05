#include "stdafx.h"
#include "CPU.h"
#include "string.h"

namespace mos6502emu{
	void PPUCallbackDummy();

	static float CPUAccDeltaTime;
	static float PPUAccDeltaTime;
	static void(*PPUCallback)() = PPUCallbackDummy;

	bool InsertROM(Fast8bit* rom, Word16bit size) {
		if (size > 0xBFE0) {
			LOG("\nError! -- InsertROM: rom size is too extensive.\n\n")
				return false;
		}
		return InsertROM(0x4020, rom, size);
	}

	bool InsertROM(Word16bit PC, Fast8bit* rom, Word16bit size) {
		if (rom == nullptr) {
			return false;
		}
		memcpy(&Memory[PC], &rom[0], size * sizeof(Fast8bit));
		return true;
	}

	void Update(float deltatime) {
		RealCPUTick(deltatime);
		RealPPUTick(deltatime);
	}

	void RealCPUTick(float deltatime) {
		debug::bBufferLog = true;
		CPUAccDeltaTime += deltatime;
		while(CPUAccDeltaTime >= CPUCycleLength) {

			float cycles = CPU::Tick() * CPUCycleLength;
			CPUAccDeltaTime -= (cycles);

		}
		debug::SendLog();
	}

	CyclesUsed TickCPU() {
		debug::bBufferLog = false;
		return CPU::Tick();
	}

	void RealPPUTick(float deltatime) {
		PPUAccDeltaTime += deltatime;
		while (PPUAccDeltaTime >= PPUCycleLength) {
			TickPPU();
			PPUAccDeltaTime -= PPUCycleLength;
		}
	}

	void TickPPU() {
		PPUCallback();
	}

	void SetPPUCallback(void(*callback)()) {
		if (callback == nullptr) {
			return;
		}
		PPUCallback = callback;
	}



	void Reset() {
		PPUAccDeltaTime = 0;
		CPUAccDeltaTime = 0 - (CPU::RESET() * CPUCycleLength );
	}

	void NMI() {
		CPU::bNMI = true;
	}

	void IRQ() {
		CPU::bIRQ = !CPU::Status.I;
	}

	void ClearDeltatimeBuffer() {
		CPUAccDeltaTime = 0;
		PPUAccDeltaTime = 0;
	}

	void SetPC(Word16bit newPC) {
		CPU::Reg.PC = newPC;
	}

	void PPUCallbackDummy() {};

	void SetDebugCallback(void(*callback)(const char* message, int count)) {
		debug::SetDebugCallback(callback);
	}

}



