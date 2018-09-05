#include "stdafx.h"
#include <string>
#include <sstream>

namespace mos6502emu {
	namespace debug {
		bool bBufferLog;

		static void LogCallbackDummy(const char* message, int count) {};
		static void(*LogCallback)(const char* message, int count) = LogCallbackDummy;

		static std::stringstream LastUpdateMessageStream;

		static inline void AddToDebugBuffer(const char* message, int count) {
			LastUpdateMessageStream << std::string(message, count);
		}

		void SetDebugCallback(void(*callback)(const char* message, int count)) {
			if (callback == nullptr) {
				return;
			}
			LogCallback = callback;
		}
		
		void Log(const char* text, ...) {
			va_list args;
			va_start(args, text);
			char buffer[128];
			int count = vsprintf_s(buffer, 128, text, args);

			if (bBufferLog) {
				AddToDebugBuffer(buffer, count);
			}
			else {
				LogCallback(buffer, count);
			}
		};

		void SendLog() {
			LogCallback(LastUpdateMessageStream.str().c_str(), (int)LastUpdateMessageStream.str().size());
			LastUpdateMessageStream.str(std::string(""));
		}
	}
}




