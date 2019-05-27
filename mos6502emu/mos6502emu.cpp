#include "stdafx.h"
#include "CPU.h"
#include "string.h"

namespace mos6502emu{
	void PPUCallbackDummy();

	static float CPUAccDeltaTime;
	static float PPUAccDeltaTime;
	static void(*PPUCallback)() = PPUCallbackDummy;

	bool InsertROM(Word16bit address, Word8bit* rom, Word16bit size) {
		if (rom == nullptr) {
			return false;
		}

		for (int i = 0; i < size; ++i) {
			Memory[address + i].data = *(rom + i);
		}
		return true;
	}

	void SetOnReadCallback(Word16bit memory_address, Word8bit(*OnReadCallback)(mos6502emu::MemoryCell* memory_cell)) {
		mos6502emu::Memory[memory_address].SetCallback(OnReadCallback);
	}

	void SetOnWriteCallback(Word16bit memory_address, void(*OnWriteCallback)(mos6502emu::MemoryCell* memory_cell, Word8bit value)) {
		mos6502emu::Memory[memory_address].SetCallback(OnWriteCallback);
	}

	void Update(float deltatime) {
		CPUAccDeltaTime += deltatime;
		while (CPUAccDeltaTime >= CPUCycleLength) {

			float cycles = CPU::Tick() * CPUCycleLength;
			CPUAccDeltaTime -= (cycles);

		}
		DebugLog.SendLog();
	}

	CyclesUsed TickCPU() {
		return CPU::Tick();
	}

	void Reset() {
		PPUAccDeltaTime = 0;
		CPUAccDeltaTime = 0 - (CPU::RESET() * CPUCycleLength );
	}

	void NMI() {
		CPU::bNMI = true;
	}

	void IRQ() {
		CPU::bIRQ = !Status.I;
	}

	void ClearDeltatimeBuffer() {
		CPUAccDeltaTime = 0;
		PPUAccDeltaTime = 0;
	}

	void SetPC(Word16bit newPC) {
		Reg.PC = newPC;
	}

	void PPUCallbackDummy() {};

	void SetDebugCallback(void(*callback)(const DebugInfo&)) {
		DebugLog.SetDebugCallback(callback);
	}

	bool SetDebugHistoryCapacity(float kilobytes) {
		return DebugLog.SetDebugCapacity(kilobytes);
	}

	
	// Memory Cell
	void MemoryCell::SetCallback(Word8bit(*OnReadCallback)(MemoryCell* memory_cell)) {
		if (OnReadCallback == nullptr) {
			MemoryCell::OnReadCallback = &MemoryOnRead_Default;
			return;
		}
		MemoryCell::OnReadCallback = OnReadCallback;
	}

	void MemoryCell::SetCallback(void(*OnWriteCallback)(MemoryCell* memory_cell, Word8bit value)) {
		if (OnWriteCallback == nullptr) {
			MemoryCell::OnWriteCallback = &MemoryOnWrite_Default;
			return;
		}
		MemoryCell::OnWriteCallback = OnWriteCallback;
	}


	Word8bit MemoryCell::Read() {
		DebugLog.Log(DebugInfo::ECommand::READ, *this);
		return OnReadCallback(this);
	}

	void MemoryCell::Write(Word8bit value) {
		DebugLog.Log(DebugInfo::ECommand::WRITE, *this);
		OnWriteCallback(this, value);
	}

	MemoryCell& MemoryBlock::operator[](const int index) {
		DebugLog.Log(DebugInfo::ECommand::NONE, index);
		return Block[index];
	};

}



