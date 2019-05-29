#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <memory>


namespace mos6502emu {
	class Debug {
	public:
		inline Debug() : LogCallback(LogCallbackDummy) {
			LogBuffer = std::make_unique<RingBuffer<DebugInfo>>(1024);
		};

		inline bool SetDebugCapacity(float kilobytes) {
			try {
				LogBuffer->resize(static_cast<unsigned int>(kilobytes * 1024 / sizeof(DebugInfo)));
			}
			catch (...) {
				return false;
			}
			return true;
		};

		void SetDebugCallback(void(*callback)(const DebugInfo& info));

		inline void UpdateRegAndStatus() {
			DebugLog.Log(Status.all_flags);
			DebugLog.Log(Reg);
		};

		inline void Log(const ProcessorRegisters& new_reg) {
			TempLog.Reg = new_reg;
		};

		inline void Log(const Word8bit new_status) {
			TempLog.Status.all_flags = new_status;
		};

		inline void Log(const std::string& opcode_name) {
			TempLog.OpCodeName = std::move(opcode_name);
		};

		inline void Log(const DebugInfo::ECommand command, const Word8bit index) {
			TempLog.CellIndex = index;
			TempLog.Command = command;
		};

		inline void Log(const DebugInfo::ECommand command, const MemoryCell& cell) {
			TempLog.Command = command;
			TempLog.CellValue = cell.data;
		};

		void SendLog();

	private:
		DebugInfo TempLog;
		std::unique_ptr<RingBuffer<DebugInfo>> LogBuffer;

		static void LogCallbackDummy(const DebugInfo& info) {};
		void(*LogCallback)(const DebugInfo& info);
	};

}









