#include "stdafx.h"
#include "CPU.h"

namespace mos6502emu{
	float CPUCycleLength = 5.5865921787709497206703910614525e-7f;
	float PPUCycleLength = 2.9411764705882352941176470588235e-5f;
	uint_least16_t InitialPC = 0x00;

	void PPUCallbackDummy();

	static float CPUAccDeltaTime;
	static float PPUAccDeltaTime;
	static void(*PPUCallback)() = PPUCallbackDummy;

	void RealCPUTick(float deltatime) {
		CPUAccDeltaTime += deltatime;
		while(CPUAccDeltaTime >= CPUCycleLength) {

			CyclesUsed cycles = TickCPU();
			CPUAccDeltaTime -= (CPUCycleLength * cycles);

			RealPPUTick(CPUCycleLength * cycles);

		}
	}

	inline CyclesUsed TickCPU() {
		
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
		CPU::ProcessorRegisters1.PC = newPC;
	}

	struct CPU::ProcessorRegisters* GetStatus() {
		return &CPU::ProcessorRegisters1;
	}

	void PPUCallbackDummy() {};

	void SetDebugCallback(void(*callback)(const char* message, int count)) {
		debug::SetDebugCallback(callback);
	}

}



