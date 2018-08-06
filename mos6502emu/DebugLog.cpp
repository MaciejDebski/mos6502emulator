#include "stdafx.h"

namespace mos6502emu {
	namespace debug {
		static void LogCallbackDummy(const char* message, int count) {};
		static void(*LogCallback)(const char* message, int count) = LogCallbackDummy;

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

			LogCallback(buffer, count);
		};
	}
}




