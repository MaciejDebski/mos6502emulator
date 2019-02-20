#pragma once

#include <stdint.h>
#include <utility>


/*
This mos6502 emulator does not support:
 - Decimal mode,
 - Illegal instructions.

*/

namespace mos6502emu {
	typedef uint_fast32_t CyclesUsed;
	typedef uint_fast16_t Fast16bit;
	typedef uint_fast8_t Fast8bit;
	typedef uint16_t Word16bit;
	typedef uint8_t Word8bit;
	typedef int_fast8_t U2Word8bit;
	class MemoryCell;

	//constexpr std :: size_t memorySize = 0xFFFFu + 1u;
	//extern std :: array<MemoryCell, meomorySize> Memory;
	extern MemoryCell Memory[0xFFFF + 1];
	extern union StatusRegisters Status;
	extern struct ProcessorRegisters Reg;

	// Initialization
	bool InsertROM(Word16bit address, Fast8bit* rom, Word16bit size);

	// PIN setup
	void SetOnReadCallback(Word16bit memory_address, Fast8bit(*OnReadCallback)(MemoryCell* memory_cell));
	void SetOnWriteCallback(Word16bit memory_address, void(*OnWriteCallback)(MemoryCell* memory_cell, Fast8bit value));

	// Const parameters
	static const float CPUClockSpeedHZ = 21477272;
	static const float CPUCycleLength = 1 / (CPUClockSpeedHZ / 12);
	static const float PPUCycleLength = 1 / (CPUClockSpeedHZ / 4);
	static const Word16bit InitialPC = 0x0;

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



	// Memory Cell Class
	class MemoryCell {
	public:
		MemoryCell() = default;
		MemoryCell(int value_8bit) : data((Fast8bit)value_8bit) {}

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

		int operator&(const MemoryCell& right) {
			return data & right.data;
		}

		int operator&(const int& right) {
			return data & right;
		}

		int operator+(const MemoryCell& right) {
			return data + right.data;
		}

		int operator+(const int& right) {
			return data + right;
		}

		int operator+(const Fast8bit right) {
			return data + right;
		}

		int operator<<(const int& shift) {
			return data << shift;
		}

		int operator>>(const int& shift) {
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

		operator int() {
			return data;
		}

	public:
		static __forceinline Fast8bit MemoryOnRead_Default(class MemoryCell* memory_cell) {
			return memory_cell->data;
		}

		static __forceinline void MemoryOnWrite_Default(class MemoryCell* memory_cell, Fast8bit value) {
			memory_cell->data = value;
		}

		// PIN setup
		void SetCallback(Fast8bit(*OnReadCallback)(MemoryCell* memory_cell));
		void SetCallback(void(*OnWriteCallback)(MemoryCell* memory_cell, Fast8bit value));

	public:
		__forceinline Fast8bit Read() {
			return OnReadCallback(this);
		}

		__forceinline void Write(Fast8bit value) {
			OnWriteCallback(this, value);
		}

	public:
		Fast8bit data;

	private:
		// Callbacks
		Fast8bit(*OnReadCallback)(MemoryCell* memory_cell) = &MemoryOnRead_Default;
		void(*OnWriteCallback)(MemoryCell* memory_cell, Fast8bit value) = &MemoryOnWrite_Default;
	};


	union StatusRegisters {
		struct {
			volatile Fast8bit C : 1;
			volatile Fast8bit Z : 1;
			volatile Fast8bit I : 1;
			volatile Fast8bit D : 1; // ignored on NES's mos6502.
			volatile Fast8bit B : 1;
			volatile Fast8bit reserved : 1;
			volatile Fast8bit V : 1;
			volatile Fast8bit N : 1;
		};
		volatile Fast8bit all_flags;
		StatusRegisters() : all_flags(0x30) {};
	};

	extern StatusRegisters Status;

	struct ProcessorRegisters {
		volatile Word16bit PC;
		volatile Fast8bit A;
		volatile Fast8bit X;
		volatile Fast8bit Y;
		volatile Word8bit SP;
		volatile union StatusRegisters* P = &Status;
		ProcessorRegisters() : PC(InitialPC), A(0x0), X(0x0), Y(0x0), SP(0xFF) {};
	};

	extern ProcessorRegisters Reg;



}



