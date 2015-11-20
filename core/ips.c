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

#include "ips.h"

uint8_t *ipspatch = NULL;
uint8_t *rom_patched = NULL;
int32_t patchedsize = 0;

extern uint8_t *rom;
extern int32_t romsize;

int cir_ips_load(const char *filepath) {
	// Load an IPS patch
	
	FILE *file;
	long filesize; // File size in bytes
	size_t result;
	
	file = fopen(filepath, "rb");
	
	if (!file) { return 0; }
	
	fseek(file, 0, SEEK_END);
	filesize = ftell(file);
	fseek(file, 0, SEEK_SET);

	ipspatch = malloc(filesize * sizeof(uint8_t));
	
	if (ipspatch == NULL) { return 0; }
	
	result = fread(ipspatch, sizeof(uint8_t), filesize, file);
	if (result != filesize) { return 0; }
	
	fclose(file);
	
	return 1;
}

int cir_ips_rom_write(const char *filepath) {
	// Write a patched ROM - IPS must be parsed before calling this
	
	FILE *file;
	
	// Allocate memory for the patched rom
	rom_patched = malloc((patchedsize) * sizeof(uint8_t));
	for (int i = 0; i < romsize; i++) {
		rom_patched[i] = rom[i];
	}
	
	// Parse and apply changes
	cir_ips_parse(1);
	
	// Open the file for writing
	file = fopen(filepath, "wb");
	
	if (!file) { return 0; }
	
	// Write the file
	fwrite(rom_patched, sizeof(uint8_t), patchedsize, file);
	
	// Close the file
	fclose(file);
	
	// Free the malloc
	free(rom_patched);
	
	return 1;
}

int cir_ips_validate() {
	// Check for a valid IPS header
	
	if (ipspatch[0] == 0x50 && ipspatch[1] == 0x41 &&
		ipspatch[2] == 0x54 && ipspatch[3] == 0x43 &&
		ipspatch[4] == 0x48) {
		return 1;
	}
	
	return 0;
}

int cir_ips_parse(int apply) {
	// Parse the IPS patch and apply if specified
	
	uint8_t record[IPS_RECORD_SIZE];
	uint32_t filepos = 0;
	uint32_t offset = 0;
	uint16_t length = 0;
	uint16_t rlelength = 0;
	int bytecount = 0;
	int patchcount = 0;
	int rle = 0;
	
	// Check if the patch is valid
	if (!cir_ips_validate()) {
		/*printf("Patch is invalid\n");*/
		return 0;
	}
	//else { printf("Patch is valid\n"); }
	
	// Patch size should be at least as big as the original ROM
	if (romsize > patchedsize) {
		patchedsize = romsize;
	}
	
	filepos += IPS_HEADER_SIZE;
	
	while (1) {
		for (int i = 0; i < IPS_RECORD_SIZE; i++) {
			record[i] = ipspatch[i + filepos];
		}
		
		if (record[0] == 0x45 && record[1] == 0x4f && record[2] == 0x46) {
			//printf("EOF\n");
			break;
		}
		
		offset = 0;
		offset += (record[0] << 16);
		offset += (record[1] << 8);
		offset += record[2];
		
		length = 0;
		length += (record[3] << 8);
		length += record[4];

		//printf("Offset: %x\n", offset);
		
		if (length == 0) {
			rlelength = 0;
			rlelength += (ipspatch[filepos + IPS_RECORD_SIZE] << 8);
			rlelength += ipspatch[filepos + IPS_RECORD_SIZE + 1];
			
			rle++;
			bytecount += rlelength;
			
			// Increment the length variable to the next IPS record
			length += 3;
			
			//printf("Length: %x (RLE)\n\n", rlelength);
			
			if (offset + rlelength > patchedsize) {
				patchedsize = offset + rlelength;
			}
			
			if (apply) {
				// Do the dirty work
				for (int i = 0; i < rlelength; i++) {
					rom_patched[offset + i] = ipspatch[filepos + IPS_RECORD_SIZE + 2];
				}
			}
		}
		else {
			bytecount += length;
			//printf("Length: %x\n\n", length);
			
			if (offset + length > patchedsize) {
				patchedsize = offset + length;
			}
			
			if (apply) {
				// Do the dirty work
				for (int i = 0; i < length; i++) {
					rom_patched[offset + i] = ipspatch[filepos + IPS_RECORD_SIZE + i];
				}
			}
		}
		
		filepos += IPS_RECORD_SIZE + length;
		patchcount++;
	}
	
	//printf("Patched size: %d\n", patchedsize);
	//printf("Bytes replaced:\t%d\n", bytecount);
	//printf("All patches:\t%d\n", patchcount);
	//printf("RLE patches:\t%d\n", rle);
	
	return 1;
}
