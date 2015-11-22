/*
 * Cirugía - Copyright (C) R. Danbrook 2015
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

#include "hash.h"
#include "ips.h"
#include "rom.h"

// Global Variables
uint8_t *rom = NULL;		// NES ROM file
uint8_t *romheader = NULL;	// The NES ROM Header
uint8_t *rom_nh = NULL;		// NES ROM file without header
uint8_t *diffrom = NULL;	// NES ROM to diff against
int32_t romsize = 0;		// Size of the ROM with header
int32_t diffsize = 0;		// Size of the Diff ROM with header
int32_t nhsize = 0;			// Size of the ROM without header

int cir_rom_load(const char *filepath) {
	// Load a ROM
	
	FILE *file;
	long filesize; // File size in bytes
	size_t result;
	
	file = fopen(filepath, "rb");
	
	if (!file) { return 0; }
	
	fseek(file, 0, SEEK_END);
	filesize = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	romsize = filesize;
	nhsize = filesize - HEADERSIZE; // Size of the ROM without the header
	
	rom = malloc(filesize * sizeof(uint8_t));
	
	if (rom == NULL) { return 0; }
	
	result = fread(rom, sizeof(uint8_t), filesize, file);
	if (result != filesize) { return 0; }
	
	fclose(file);
	
	return 1;
}

int cir_rom_load_diff(const char *filepath) {
	// Load a Diff ROM
	
	FILE *file;
	long filesize; // File size in bytes
	size_t result;
	
	file = fopen(filepath, "rb");
	
	if (!file) { return 0; }
	
	fseek(file, 0, SEEK_END);
	filesize = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	diffsize = filesize;
	
	diffrom = malloc(filesize * sizeof(uint8_t));
	
	if (diffrom == NULL) { return 0; }
	
	result = fread(diffrom, sizeof(uint8_t), filesize, file);
	if (result != filesize) { return 0; }
	
	fclose(file);
	
	return 1;
}

int cir_rom_write(const char *filepath) {
	// Write a ROM
	
	FILE *file;
	uint8_t *outfile = NULL;
	
	// Open the file for writing
	file = fopen(filepath, "wb");
	
	if (!file) { return 0; }
	
	// Stitch the pieces together
	outfile = malloc((nhsize + HEADERSIZE) * sizeof(uint8_t));
	
	for (int i = 0; i < HEADERSIZE; i++) { outfile[i] = romheader[i]; }
	for (int j = 0; j < nhsize; j++) { outfile[j + HEADERSIZE] = rom_nh[j]; }
	
	// Write the file
	fwrite(outfile, sizeof(uint8_t), HEADERSIZE + nhsize, file);
	
	// Close the file
	fclose(file);
	
	// Free the malloc
	free(outfile);
	
	return 1;
}

uint32_t cir_rom_get_crc() {
	// Get the CRC32 checksum of the ROM minus the header
	uint32_t crc = 0;
	crc = crc32(crc, rom_nh, nhsize);
	return crc;
}

const char* cir_rom_get_sha1() {
	// Get the SHA1 checksum of the ROM minus the header
	uint8_t *temprom = NULL;
	temprom = malloc(nhsize * sizeof(uint8_t));
	for (int i = 0; i < nhsize; i++) { temprom[i] = rom_nh[i]; }
	const char *retval = sha1(temprom, nhsize);
	free(temprom);
	return retval;
}

void cir_rom_split_header_rom() {
	// Split the header from the ROM
	
	// Copy header to a malloc
	romheader = malloc(HEADERSIZE * sizeof(uint8_t));
	for (int i = 0; i < HEADERSIZE; i++) {
		romheader[i] = rom[i];
	}
	
	// Copy the ROM data to a malloc
	rom_nh = malloc(nhsize * sizeof(uint8_t));
	for (int i = 0; i < nhsize; i++) {
		rom_nh[i] = rom[i + HEADERSIZE];
	}
}

void cir_rom_cleanup() {
	// Free mallocs
	free(rom);
	free(romheader);
	free(rom_nh);
	free(diffrom);
}
