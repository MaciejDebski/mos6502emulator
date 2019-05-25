#include "stdafx.h"
#include <string>
#include <sstream>

namespace mos6502emu {
	void Debug::SetDebugCallback(void(*callback)(const DebugInfo& info)) {
		if (callback == nullptr) {
			LogCallback = LogCallbackDummy;
			return;
		}
		LogCallback = callback;
	}

	void Debug::SendLog() {
		LogCallback(TempLog);
		LogBuffer->Push(TempLog);
	};
}




