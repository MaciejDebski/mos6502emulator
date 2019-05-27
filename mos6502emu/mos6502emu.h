#pragma once

#include <stdint.h>
#include <utility>
#include <string>


/*
This mos6502 emulator does not support:
 - Decimal mode,
 - Illegal instructions.

*/

namespace mos6502emu {
	typedef uint_fast32_t CyclesUsed;
	typedef uint16_t Word16bit;
	typedef uint8_t Word8bit;
	class MemoryCell;

	//constexpr std :: size_t memorySize = 0xFFFFu + 1u;
	//extern std :: array<MemoryCell, meomorySize> Memory;
	extern class MemoryBlock Memory;
	extern union StatusRegisters Status;
	extern struct ProcessorRegisters Reg;

	// Initialization
	bool InsertROM(Word16bit address, Word8bit* rom, Word16bit size);

	// PIN setup
	void SetOnReadCallback(Word16bit memory_address, Word8bit(*OnReadCallback)(MemoryCell* memory_cell));
	void SetOnWriteCallback(Word16bit memory_address, void(*OnWriteCallback)(MemoryCell* memory_cell, Word8bit value));

	// Const parameters
	static const float CPUClockSpeedHZ = 1'000'000;
	static const float CPUCycleLength = 1 / CPUClockSpeedHZ;
	static const Word16bit InitialPC = 0x0;

	// CPU Controls
	void Update(float deltatime);
	CyclesUsed TickCPU();

	void Reset();
	void NMI();
	void IRQ();
	void ClearDeltatimeBuffer();
	void SetPC(Word16bit newPC);

	// Debug
	void SetDebugCallback(void(*callback)(const struct DebugInfo&));
	bool SetDebugHistoryCapacity(float kilobytes);

	extern class Debug DebugLog;

	// Memory Cell Class
	class MemoryCell {
	public:
		inline MemoryCell() = default;
		MemoryCell(int value_8bit) : data((Word8bit)value_8bit) {}

		MemoryCell(const MemoryCell& right) : data(right.data) {}

		MemoryCell& operator=(const MemoryCell& right) {
			data = right.data;
			OnReadCallback = right.OnReadCallback;
			OnWriteCallback = right.OnWriteCallback;
			return *this;
		}

		MemoryCell& operator=(const int& right) {
			data = right;
			return *this;
		}

		int operator&(const MemoryCell& right) const {
			return data & right.data;
		}

		int operator&(const int& right) const {
			return data & right;
		}

		int operator+(const MemoryCell& right) const {
			return data + right.data;
		}

		int operator+(const int& right) const {
			return data + right;
		}

		int operator+(const Word8bit right) const {
			return data + right;
		}

		int operator<<(const int& shift) const {
			return data << shift;
		}

		int operator>>(const int& shift) const {
			return data >> shift;
		}

		MemoryCell& operator&=(const MemoryCell& right) {
			data &= right.data;
			return *this;
		}

		MemoryCell& operator++() {
			++data;
			return *this;
		}

		MemoryCell&& operator++(int) {
			MemoryCell temp = *this;
			++data;
			return std::move(temp);
		}

		MemoryCell& operator--() {
			--data;
			return *this;
		}

		MemoryCell&& operator--(const int right) {
			MemoryCell temp = *this;
			--data;
			return std::move(temp);
		}

		operator int() const {
			return data;
		}

	public:
		static __forceinline Word8bit MemoryOnRead_Default(class MemoryCell* memory_cell) {
			return memory_cell->data;
		}

		static __forceinline void MemoryOnWrite_Default(class MemoryCell* memory_cell, Word8bit value) {
			memory_cell->data = value;
		}

		// PIN setup
		void SetCallback(Word8bit(*OnReadCallback)(MemoryCell* memory_cell));
		void SetCallback(void(*OnWriteCallback)(MemoryCell* memory_cell, Word8bit value));

	public:
		Word8bit Read();
		void Write(Word8bit value);

	public:
		Word8bit data;

	private:
		// Callbacks
		Word8bit(*OnReadCallback)(MemoryCell* memory_cell) = &MemoryOnRead_Default;
		void(*OnWriteCallback)(MemoryCell* memory_cell, Word8bit value) = &MemoryOnWrite_Default;
	};

	union StatusRegisters {
		struct {
			volatile Word8bit C : 1;
			volatile Word8bit Z : 1;
			volatile Word8bit I : 1;
			volatile Word8bit D : 1; // ignored on NES's mos6502.
			volatile Word8bit B : 1;
			volatile Word8bit reserved : 1;
			volatile Word8bit V : 1;
			volatile Word8bit N : 1;
		};
		volatile Word8bit all_flags;
		StatusRegisters() : all_flags(0x30) {};
	};

	extern StatusRegisters Status;

	struct ProcessorRegisters {
		volatile Word16bit PC;
		volatile Word8bit A;
		volatile Word8bit X;
		volatile Word8bit Y;
		volatile Word8bit SP;
		volatile union StatusRegisters* P = &Status;
		ProcessorRegisters() : PC(InitialPC), A(0x0), X(0x0), Y(0x0), SP(0xFF) {};
	};

	extern ProcessorRegisters Reg;

	struct DebugInfo {
		enum class ECommand {
			NONE,
			READ,
			WRITE
		};

		ProcessorRegisters Reg;
		StatusRegisters Status;
		std::string OpCodeName;
		ECommand Command;
		Word8bit CellValue;
		Word16bit CellIndex;

		inline DebugInfo() : Command(ECommand::NONE), CellValue(0), CellIndex(0) {};
	};

	class MemoryBlock {
	public:
		MemoryCell& operator[](const int index);

	private:
		MemoryCell Block[0xFFFF + 1];
	};
}



