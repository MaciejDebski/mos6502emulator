#pragma once

#include <stdio.h>
#include <stdarg.h>


namespace mos6502emu {
	namespace debug {
		extern bool bBufferLog;
		
		void SetDebugCallback(void(*callback)(const char* message, int count));

		void Log(const char* text, ...);
		void SendLog();
	}

#define LOG(x, ...) debug::Log(x, __VA_ARGS__);

}









