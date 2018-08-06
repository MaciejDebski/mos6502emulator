#pragma once

namespace mos6502emu {

	enum NES_PPU {
		PPUCTRL = 0x2000,		//NMI enable(V), PPU master / slave(P), sprite height(H), background tile select(B), sprite tile select(S), increment mode(I), nametable select(NN)
		PPUMASK = 0x2001,		//color emphasis(BGR), sprite enable(s), background enable(b), sprite left column enable(M), background left column enable(m), greyscale(G)
		PPUSTATUS = 0x2002,		//vblank(V), sprite 0 hit(S), sprite overflow(O), read resets write pair for $2005 / 2006
		OAMADDR = 0x2003,		//OAM read / write address
		OAMDATA = 0x2004, 		//OAM data read / write
		PPUSCROLL = 0x2005,		//fine scroll position(two writes : X, Y)
		PPUADDR = 0x2006,		//PPU read / write address(two writes : MSB, LSB)
		PPUDATA = 0x2007,		//PPU data read / write
		OAMDMA = 0x4014,		//OAM DMA high address
	};

	enum NES_APU {
		SQ1_VOL = 0x4000,			//Duty and volume for square wave 1
		SQ1_SWEEP = 0x4001, 		//Sweep control register for square wave 1
		SQ1_LO = 0x4002, 	 		//Low byte of period for square wave 1
		SQ1_HI = 0x4003,			//High byte of period and length counter value for square wave 1
		SQ2_VOL = 0x4004,			//Duty and volume for square wave 2
		SQ2_SWEEP = 0x4005,			//Sweep control register for square wave 2
		SQ2_LO = 0x4006,			//Low byte of period for square wave 2
		SQ2_HI = 0x4007,			//High byte of period and length counter value for square wave 2
		TRI_LINEAR = 0x4008, 		//Triangle wave linear counter
		// 0x4009 					//Unused, but is eventually accessed in memory - clearing loops
		TRI_LO = 0x400A, 	 		//Low byte of period for triangle wave
		TRI_HI = 0x400B, 			//High byte of period and length counter value for triangle wave
		NOISE_VOL = 0x400C,			//Volume for noise generator
		// 0x400D 					//Unused, but is eventually accessed in memory - clearing loops
		NOISE_LO = 0x400E,  		//Period and waveform shape for noise generator
		NOISE_HI = 0x400F, 			//Length counter value for noise generator
		DMC_FREQ = 0x4010, 			//Play mode and frequency for DMC samples
		DMC_RAW = 0x4011, 			//7 - bit DAC
		DMC_START = 0x4012, 		//Start of DMC waveform is at address $C000 + $40 * $xx
		DMC_LEN = 0x4013, 			//Length of DMC waveform is $10*$xx + 1 bytes(128 * $xx + 8 samples)
		SND_CHN = 0x4015,			//Sound channels enable and status
	};

	enum NES_IO {
		JOY1 = 0x4016,	 			//Joystick 1 data(R) and joystick strobe(W)
		JOY2 = 0x4017,				//Joystick 2 data(R) and frame counter control(W)
	};

	





}


