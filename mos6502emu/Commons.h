#pragma once

#include "mos6502emu.h"

namespace mos6502emu {

#define PasteBit(byte, shift, bit) ((byte & ~(1 << shift)) | (bit << shift) )

	static inline Word8bit negativeU2toUNS(U2Word8bit U2) {
		return (Word8bit)~(U2 - 0x1);
	}

}



