#include "stdafx.h"
#include "CPU.h"

namespace mos6502emu{
	void PPUCallbackDummy();

	static float CPUAccDeltaTime;
	static float PPUAccDeltaTime;
	static void(*PPUCallback)() = PPUCallbackDummy;

	void Update(float deltatime) {
		RealCPUTick(deltatime);
		RealPPUTick(deltatime);
	}

	void RealCPUTick(float deltatime) {
		CPUAccDeltaTime += deltatime;
		while(CPUAccDeltaTime >= CPUCycleLength) {

			CyclesUsed cycles = TickCPU() * CPUCycleLength;
			CPUAccDeltaTime -= (cycles);

		}
	}

	inline CyclesUsed TickCPU() {
		CPU::Tick();
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
		CPUAccDeltaTime = 0;
		// TODO: reset

	}

	void ClearDeltatimeBuffer() {
		CPUAccDeltaTime = 0;
	}

	void SetPC(uint_least16_t newPC) {
		CPU::Reg.PC = newPC;
	}

	struct CPU::ProcessorRegisters* GetStatus() {
		return &CPU::Reg;
	}

	void PPUCallbackDummy() {};

	void SetDebugCallback(void(*callback)(const char* message, int count)) {
		debug::SetDebugCallback(callback);
	}

}



