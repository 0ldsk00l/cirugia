/*
 * Cirugía - Copyright (C) R. Danbrook 2015-2016
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of the program nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Global Variables
extern uint8_t *romheader;

int version = 0;

int cir_header_validate() {
	// Check if there is a valid header
	
	//0-3: Constant $4E $45 $53 $1A ("NES" followed by EOF)
	if (romheader[0] == 0x4e && romheader[1] == 0x45 &&
		romheader[2] == 0x53 && romheader[3] == 0x1a) {
		return 1;
	}
	else { return 0; }
}

int cir_header_get_version() {
	// Return the header version
	
	if (romheader[7] & 0x08 && !(romheader[7] & 0x04)) {
		return 2;
	}
	else { return 1; }
}

void cir_header_set_version(uint8_t data) {
	// Set the header version
	
	if (data == 1) {
		romheader[7] &= ~(1 << 2);
		romheader[7] &= ~(1 << 3);
	}
	else {
		romheader[7] &= ~(1 << 2);
		romheader[7] |= 1 << 3;
	}
	
	version = data;
}

int cir_header_get_prgrom() {
	// Get the PRG ROM size in 16K blocks
	
	// NES 2.0
	if (version == 2) {
		int value = 0;
		value = ((romheader[9] & 0x0f) << 8) | romheader[4];
		return value;
	}
	// iNES
	else { return romheader[4]; }
}

void cir_header_set_prgrom(uint16_t data) {
	// Set PRG ROM size
	romheader[4] = 0x00;
	romheader[4] = data & 0xff;
	if (version == 2) {
		romheader[9] &= ~0x0f;
		romheader[9] |= (data & 0xff00) >> 8;
	}
}

int cir_header_get_prgram_present() {
	// Check if PRG RAM exists
	if (romheader[6] & 0x02) { return 1; }
	else { return 0; }
}

void cir_header_set_prgram_present(uint8_t data) {
	// Set PRG RAM exists flag
	romheader[6] &= ~(1 << 1);
	romheader[6] |= data << 1;
}

int cir_header_get_prgram() {
	// Get PRG RAM size
	if (version == 2) {
		return 0x80 << ((romheader[10] & 0x0f) - 1);
	}
	else {
		return romheader[8];
	}
}

void cir_header_set_prgram(uint8_t data) {
	// Set PRG RAM size
	if (version == 2) {
		romheader[10] &= ~0x0f;
		romheader[10] |= data;
	}
	else {
		romheader[8] = 0x00;
		romheader[8] |= data;
	}
}

int cir_header_get_prgnvram() {
	// Get PRG NVRAM size
	return 0x80 << (((romheader[10] & 0xf0) >> 4) - 1);
}

void cir_header_set_prgnvram(uint8_t data) {
	// Set PRG NVRAM size
	romheader[10] &= ~0xf0;
	romheader[10] |= data << 4;
}

int cir_header_get_chrram() {
	// Get CHR RAM size
	return 0x80 << ((romheader[11] & 0x0f) - 1);
}

void cir_header_set_chrram(uint8_t data) {
	// Set CHR RAM size
	romheader[11] &= ~0x0f;
	romheader[11] |= data;
}

int cir_header_get_chrnvram() {
	// Get CHR NVRAM size
	return 0x80 << (((romheader[11] & 0xf0) >> 4) - 1);
}

void cir_header_set_chrnvram(uint8_t data) {
	// Set CHR NVRAM size
	romheader[11] &= ~0xf0;
	romheader[11] |= data << 4;
}

int cir_header_get_chrrom() {
	// Get CHR ROM size if there is CHR ROM
	
	if (version == 2) {
		return ((romheader[9] & 0xf0) << 4) | romheader[5];
	}
	else {
		if (romheader[5] == 0) {
			return 0;
		}
		else {
			return romheader[5];
		}
	}
}

void cir_header_set_chrrom(uint16_t data) {
	// Set CHR ROM size
	romheader[5] = 0x00;
	romheader[5] = data & 0xff;
	if (version == 2) {
		romheader[9] &= ~0xf0;
		romheader[9] |= (data & 0xff00) >> 4;
	}
}

int cir_header_get_mapper() {
	// Get the Mapper number
	if (version == 2) {
		return (((romheader[6]) >> 4) & 0x0f) | ((((romheader[7]) >> 4) & 0x0f) << 4) | ((romheader[8] & 0x0f) << 8);
	}
	else {
		return (((romheader[6]) >> 4) & 0x0f) | ((((romheader[7]) >> 4) & 0x0f) << 4);
	}
}

void cir_header_set_mapper(uint16_t data) {
	// Set the Mapper number (0 - 4095)
	
	if (version == 2) {
		romheader[8] &= ~0x0f;
		romheader[8] |= (data & 0xff00) >> 8;
	}
	
	romheader[7] &= ~0xf0;
	romheader[7] |= (data >> 4) << 4;
	
	romheader[6] &= ~0xf0;
	romheader[6] |= (data & 0x000f) << 4;
}

int cir_header_get_submapper() {
	// Read the NES 2.0 Submapper number
	int value = romheader[8] >> 4;
	return value;
}

void cir_header_set_submapper(uint8_t data) {
	// Set the NES 2.0 Submapper number (0 - 15)
	
	romheader[8] &= ~0xf0;
	romheader[8] |= data << 4;
}

int cir_header_get_mirroring() {
	// Get mirroring
	
	if (romheader[6] & 0x08) { // Four screen mode
		return 2;
	}
	else if (romheader[6] & 0x01) { // Vertical
		return 1;
	}
	else { // Horizontal
		return 0;
	}
}

void cir_header_set_mirroring(uint8_t data) {
	// Set mirroring
	
	if (data == 0) { // Horizontal
		romheader[6] &= ~(1 << 3);
		romheader[6] &= 1;
	}
	else if (data == 1) { // Vertical
		romheader[6] &= ~(1 << 3);
		romheader[6] |= 1;
	}
	else { // Four screen mode
		romheader[6] &= ~1;
		romheader[6] |= (1 << 3);
	}
}

int cir_header_get_trainer() {
	// Check if there is a 512-byte trainer
	
	return romheader[6] & 0x04 ? 1 : 0;
}

void cir_header_set_trainer(uint8_t data) {
	// Set the 512-byte trainer bit
	
	if (data == 0) {
		romheader[6] &= ~(1 << 2);
	}
	else {
		romheader[6] |= 1 << 2;
	}
}

int cir_header_get_system() {
	// Get the hardware version/system
	if (romheader[7] & 0x02) { return 2; } // PlayChoice-10
	else if (romheader[7] & 0x01) { return 1; } // VS. Unisystem
	else { return 0; } // Home Console
}

void cir_header_set_system(uint8_t data) {
	// Set the hardware version/system
	
	if (data == 0) { // Home System
		romheader[7] &= ~1;
		romheader[7] &= ~(1 << 1);
	}
	else if (data == 1) { // VS. Unisystem
		romheader[7] &= ~(1 << 1);
		romheader[7] |= 1;
	}
	else { // PlayChoice-10
		romheader[7] &= ~1;
		romheader[7] |= 1 << 1;
	}
}

int cir_header_get_tvsystem() {
	// Get the TV System
	
	if (version == 2) { // NES 2.0
		if (romheader[12] & 0x02) {
			return 2; // NTSC/PAL
		}
		else if (romheader[12] & 0x01) {
			return 1; // PAL
		}
		else { return 0; } // NTSC
	}
	else { // iNES
		if (romheader[9] & 0x01) {
			return 1; // PAL
		}
		else { return 0; } // NTSC
	}
}

void cir_header_set_tvsystem(uint8_t data) {
	// Set the TV System
	// Remember to deal with version here
	if (data == 0) { // NTSC
		romheader[12] &= ~1;
	}
	else if (data == 1) { // PAL
		romheader[12] |= 1;
	}
	else { // NTSC/PAL
		romheader[12] &= ~1;
		romheader[12] |= 1 << 1;
	}
}

int cir_header_get_vsppu() {
	// Get VS. System PPU Hardware
	return romheader[13] & 0x0f;
}

void cir_header_set_vsppu(uint8_t data) {
	// Set VS. System PPU Hardware (0 - 12)
	romheader[13] &= ~0x0f;
	romheader[13] |= (data & 0x0f);
}

int cir_header_get_vsmode() {
	// Get VS. System PPU Mode
	return (romheader[13] >> 4) & 0x0f;
}

void cir_header_set_vsmode(uint8_t data) {
	// Set VS. System PPU Mode (0-3)
	romheader[13] &= ~0xf0;
	romheader[13] |= (data & 0x0f) << 4;
}
