#pragma once

#include "mos6502emu.h"

namespace mos6502emu {

#define PASTE_BIT(byte, shift, bit) (((byte) & (~(1 << (shift)) & 0xFF)) | ((bit) << (shift)) )

	static inline Fast8bit negativeU2toUNS(U2Word8bit U2) {
		return ~(U2 - 0x1) & 0xFF;
	}

}



