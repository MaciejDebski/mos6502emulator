#include "stdafx.h"
#include "Commons.h"


namespace mos6502emu {

	Word16bit To16Bit(Word8bit word) {
		if ((word & 0x80) == 0x80) { // if 8 bit value is negative,
			return 0xFFFF - ((~word) & 0xFF);
		}
		return word;
	}

};



