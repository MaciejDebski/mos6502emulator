#pragma once

#include "mos6502emu.h"

namespace mos6502emu {

#define PASTE_BIT(byte, shift, bit) (((byte) & (~(1 << (shift)) & 0xFF)) | ((bit) << (shift)) )

	Word16bit To16Bit(Word8bit word);

}



