#include "stdafx.h"
#include "CPU.h"
#include "string.h"

namespace mos6502emu{
	void PPUCallbackDummy();

	static float CPUAccDeltaTime;
	static float PPUAccDeltaTime;
	static void(*PPUCallback)() = PPUCallbackDummy;

	void PowerUp() {
		CPU::PowerUp();
	}

	void InsertROM(const Fast8bit** rom, Word16bit size) {
		if (size > 0xBFE0) {
			return;
		}
		memcpy(&Memory[0x4020], &rom, size);
	}

	void Update(float deltatime) {
		RealCPUTick(deltatime);
		RealPPUTick(deltatime);
	}

	void RealCPUTick(float deltatime) {
		CPUAccDeltaTime += deltatime;
		while(CPUAccDeltaTime >= CPUCycleLength) {

			float cycles = TickCPU() * CPUCycleLength;
			CPUAccDeltaTime -= (cycles);

		}
	}

	inline CyclesUsed TickCPU() {
		return CPU::Tick();
	}

	inline void RealPPUTick(float deltatime) {
		PPUAccDeltaTime += deltatime;
		while (PPUAccDeltaTime >= PPUCycleLength) {
			TickPPU();
			PPUAccDeltaTime -= PPUCycleLength;
		}
	}

	inline void TickPPU() {
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

	void SetPC(uint_least16_t newPC) {
		CPU::Reg.PC = newPC;
	}

	void PPUCallbackDummy() {};

	void SetDebugCallback(void(*callback)(const char* message, int count)) {
		debug::SetDebugCallback(callback);
	}

}



