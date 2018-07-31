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
#include <string.h>

// Global Variables
extern uint8_t *rom_hdr;
extern uint8_t *rom;

int version = 0;

int cir_header_validate() {
	// Check if there is a valid header
	
	//0-3: Constant $4E $45 $53 $1A ("NES" followed by EOF)
	if (rom[0] == 0x4e && rom[1] == 0x45 &&
		rom[2] == 0x53 && rom[3] == 0x1a) {
		return 1;
	}
	//0-3: Constant $46 $44 $53 $1A ("FDS" followed by EOF)
	else if (rom[0] == 0x46 && rom[1] == 0x44 &&
		rom[2] == 0x53 && rom[3] == 0x1a) {
		return 2;
	}
	else { return 0; }
}

int cir_ines_get_version() {
	// Return the header version
	
	if (rom_hdr[7] & 0x08 && !(rom_hdr[7] & 0x04)) {
		return 2;
	}
	else { return 1; }
}

void cir_ines_set_version(uint8_t data) {
	// Set the header version
	
	if (data == 1) {
		rom_hdr[7] &= ~(1 << 2);
		rom_hdr[7] &= ~(1 << 3);
	}
	else {
		rom_hdr[7] &= ~(1 << 2);
		rom_hdr[7] |= 1 << 3;
	}
	
	version = data;
}

int cir_ines_get_prgrom() {
	// Get the PRG ROM size in 16K blocks
	
	// NES 2.0
	if (version == 2) {
		int value = 0;
		value = ((rom_hdr[9] & 0x0f) << 8) | rom_hdr[4];
		return value;
	}
	// iNES
	else { return rom_hdr[4]; }
}

void cir_ines_set_prgrom(uint16_t data) {
	// Set PRG ROM size
	rom_hdr[4] = 0x00;
	rom_hdr[4] = data & 0xff;
	if (version == 2) {
		rom_hdr[9] &= ~0x0f;
		rom_hdr[9] |= (data & 0xff00) >> 8;
	}
}

int cir_ines_get_prgram_present() {
	// Check if PRG RAM exists
	if (rom_hdr[6] & 0x02) { return 1; }
	else { return 0; }
}

void cir_ines_set_prgram_present(uint8_t data) {
	// Set PRG RAM exists flag
	rom_hdr[6] &= ~(1 << 1);
	rom_hdr[6] |= data << 1;
}

int cir_ines_get_prgram() {
	// Get PRG RAM size
	if (version == 2) {
		return 0x80 << ((rom_hdr[10] & 0x0f) - 1);
	}
	else {
		return rom_hdr[8];
	}
}

void cir_ines_set_prgram(uint8_t data) {
	// Set PRG RAM size
	if (version == 2) {
		rom_hdr[10] &= ~0x0f;
		rom_hdr[10] |= data;
	}
	else {
		rom_hdr[8] = 0x00;
		rom_hdr[8] |= data;
	}
}

int cir_ines_get_prgnvram() {
	// Get PRG NVRAM size
	return 0x80 << (((rom_hdr[10] & 0xf0) >> 4) - 1);
}

void cir_ines_set_prgnvram(uint8_t data) {
	// Set PRG NVRAM size
	rom_hdr[10] &= ~0xf0;
	rom_hdr[10] |= data << 4;
}

int cir_ines_get_chrram() {
	// Get CHR RAM size
	return 0x80 << ((rom_hdr[11] & 0x0f) - 1);
}

void cir_ines_set_chrram(uint8_t data) {
	// Set CHR RAM size
	rom_hdr[11] &= ~0x0f;
	rom_hdr[11] |= data;
}

int cir_ines_get_chrnvram() {
	// Get CHR NVRAM size
	return 0x80 << (((rom_hdr[11] & 0xf0) >> 4) - 1);
}

void cir_ines_set_chrnvram(uint8_t data) {
	// Set CHR NVRAM size
	rom_hdr[11] &= ~0xf0;
	rom_hdr[11] |= data << 4;
}

int cir_ines_get_chrrom() {
	// Get CHR ROM size if there is CHR ROM
	
	if (version == 2) {
		return ((rom_hdr[9] & 0xf0) << 4) | rom_hdr[5];
	}
	else {
		if (rom_hdr[5] == 0) {
			return 0;
		}
		else {
			return rom_hdr[5];
		}
	}
}

void cir_ines_set_chrrom(uint16_t data) {
	// Set CHR ROM size
	rom_hdr[5] = 0x00;
	rom_hdr[5] = data & 0xff;
	if (version == 2) {
		rom_hdr[9] &= ~0xf0;
		rom_hdr[9] |= (data & 0xff00) >> 4;
	}
}

int cir_ines_get_mapper() {
	// Get the Mapper number
	if (version == 2) {
		return (((rom_hdr[6]) >> 4) & 0x0f) | ((((rom_hdr[7]) >> 4) & 0x0f) << 4) | ((rom_hdr[8] & 0x0f) << 8);
	}
	else {
		return (((rom_hdr[6]) >> 4) & 0x0f) | ((((rom_hdr[7]) >> 4) & 0x0f) << 4);
	}
}

void cir_ines_set_mapper(uint16_t data) {
	// Set the Mapper number (0 - 4095)
	
	if (version == 2) {
		rom_hdr[8] &= ~0x0f;
		rom_hdr[8] |= (data & 0xff00) >> 8;
	}
	
	rom_hdr[7] &= ~0xf0;
	rom_hdr[7] |= (data >> 4) << 4;
	
	rom_hdr[6] &= ~0xf0;
	rom_hdr[6] |= (data & 0x000f) << 4;
}

int cir_ines_get_submapper() {
	// Read the NES 2.0 Submapper number
	int value = rom_hdr[8] >> 4;
	return value;
}

void cir_ines_set_submapper(uint8_t data) {
	// Set the NES 2.0 Submapper number (0 - 15)
	
	rom_hdr[8] &= ~0xf0;
	rom_hdr[8] |= data << 4;
}

int cir_ines_get_mirroring() {
	// Get mirroring
	
	if (rom_hdr[6] & 0x08) { // Four screen mode
		return 2;
	}
	else if (rom_hdr[6] & 0x01) { // Vertical
		return 1;
	}
	else { // Horizontal
		return 0;
	}
}

void cir_ines_set_mirroring(uint8_t data) {
	// Set mirroring
	
	if (data == 0) { // Horizontal
		rom_hdr[6] &= ~(1 << 3);
		rom_hdr[6] &= 1;
	}
	else if (data == 1) { // Vertical
		rom_hdr[6] &= ~(1 << 3);
		rom_hdr[6] |= 1;
	}
	else { // Four screen mode
		rom_hdr[6] &= ~1;
		rom_hdr[6] |= (1 << 3);
	}
}

int cir_ines_get_trainer() {
	// Check if there is a 512-byte trainer
	
	return rom_hdr[6] & 0x04 ? 1 : 0;
}

void cir_ines_set_trainer(uint8_t data) {
	// Set the 512-byte trainer bit
	
	if (data == 0) {
		rom_hdr[6] &= ~(1 << 2);
	}
	else {
		rom_hdr[6] |= 1 << 2;
	}
}

int cir_ines_get_system() {
	// Get the hardware version/system
	if (rom_hdr[7] & 0x02) { return 2; } // PlayChoice-10
	else if (rom_hdr[7] & 0x01) { return 1; } // VS. Unisystem
	else { return 0; } // Home Console
}

void cir_ines_set_system(uint8_t data) {
	// Set the hardware version/system
	
	if (data == 0) { // Home System
		rom_hdr[7] &= ~1;
		rom_hdr[7] &= ~(1 << 1);
	}
	else if (data == 1) { // VS. Unisystem
		rom_hdr[7] &= ~(1 << 1);
		rom_hdr[7] |= 1;
	}
	else { // PlayChoice-10
		rom_hdr[7] &= ~1;
		rom_hdr[7] |= 1 << 1;
	}
}

int cir_ines_get_tvsystem() {
	// Get the TV System
	
	if (version == 2) { // NES 2.0
		if (rom_hdr[12] & 0x02) {
			return 2; // NTSC/PAL
		}
		else if (rom_hdr[12] & 0x01) {
			return 1; // PAL
		}
		else { return 0; } // NTSC
	}
	else { // iNES
		if (rom_hdr[9] & 0x01) {
			return 1; // PAL
		}
		else { return 0; } // NTSC
	}
}

void cir_ines_set_tvsystem(uint8_t data) {
	// Set the TV System
	// Remember to deal with version here
	if (data == 0) { // NTSC
		rom_hdr[12] &= ~1;
	}
	else if (data == 1) { // PAL
		rom_hdr[12] |= 1;
	}
	else { // NTSC/PAL
		rom_hdr[12] &= ~1;
		rom_hdr[12] |= 1 << 1;
	}
}

int cir_ines_get_vsppu() {
	// Get VS. System PPU Hardware
	return rom_hdr[13] & 0x0f;
}

void cir_ines_set_vsppu(uint8_t data) {
	// Set VS. System PPU Hardware (0 - 12)
	rom_hdr[13] &= ~0x0f;
	rom_hdr[13] |= (data & 0x0f);
}

int cir_ines_get_vsmode() {
	// Get VS. System PPU Mode
	return (rom_hdr[13] >> 4) & 0x0f;
}

void cir_ines_set_vsmode(uint8_t data) {
	// Set VS. System PPU Mode (0-3)
	rom_hdr[13] &= ~0xf0;
	rom_hdr[13] |= (data & 0x0f) << 4;
}

// FDS

uint8_t cir_fds_get_disksides() {
	return rom_hdr[4];
}

uint8_t cir_fds_get_diskverification() {
	if (memcmp(&rom[17], "*NINTENDO-HVC*", 14)) {
		return 0;
	}
	else { return 1; }
}

uint8_t cir_fds_get_mfrcode() {
	return rom[31];
}

char *cir_fds_get_gamename() {
	static char gamename[4];
	gamename[0] = rom[32]; gamename[1] = rom[33];
	gamename[2] = rom[34]; gamename[3] = '\0';
	return gamename;
}

uint8_t cir_fds_get_gametype() {
	return rom[35];
}

uint8_t cir_fds_get_revision() {
	return rom[36];
}
