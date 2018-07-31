/*
 * Cirugía - Copyright (C) R. Danbrook 2015-2018
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
#include <getopt.h>

#include "cirugia.h"
#include "header.h"
#include "ips.h"
#include "rom.h"

static const char *mfrcodes[256] = {
	[0x00] = "Unlicensed",
	[0x01] = "Nintendo",
	[0x08] = "Capcom",
	[0x0a] = "Jaleco",
	[0x18] = "Hudson Soft",
	[0x49] = "Irem",
	[0x4a] = "Gakken",
	[0x8b] = "BulletProof Software (BPS)",
	[0x99] = "Pack-In-Video",
	[0x9b] = "Tecmo",
	[0x9c] = "Imagineer",
	[0xa2] = "Scorpion Soft",
	[0xa4] = "Konami",
	[0xa6] = "Kawada Co., Ltd.",
	[0xa7] = "Takara",
	[0xa8] = "Royal Industries",
	[0xac] = "Toei Animation",
	[0xaf] = "Namco",
	[0xb1] = "ASCII Corporation",
	[0xb2] = "Bandai",
	[0xb3] = "Soft Pro Inc.",
	[0xb6] = "HAL Laboratory",
	[0xbb] = "Sunsoft",
	[0xbc] = "Toshiba EMI",
	[0xc0] = "Taito",
	[0xc1] = "Sunsoft / Ask Co., Ltd.",
	[0xc2] = "Kemco",
	[0xc3] = "Square",
	[0xc4] = "Tokuma Shoten",
	[0xc5] = "Data East",
	[0xc6] = "Tonkin House/Tokyo Shoseki",
	[0xc7] = "East Cube",
	[0xca] = "Konami/Ultra/Palcom",
	[0xcb] = "NTVIC/VAP",
	[0xcc] = "Use Co., Ltd.",
	[0xce] = "Pony Canyon / FCI",
	[0xd1] = "Sofel",
	[0xd2] = "Bothtec, Inc.",
	[0xdb] = "Hiro Co., Ltd.",
	[0xe7] = "Athena",
	[0xeb] = "Atlus",
};

extern int version;

int cir_cli_header_parse() {
	// Check the ROM header information
	
	if (version == 2) {
		fprintf(stdout, "Header Type: NES 2.0\n");
	}
	else {
		fprintf(stdout, "Header Type: iNES\n");
	}
	
	// Get the mapper number
	int mappernum = cir_ines_get_mapper();
	fprintf(stdout, "Mapper: %d (0x%.2x)\n", mappernum, mappernum);
	
	// Get the submapper number (NES 2.0)
	if (version == 2) {
		fprintf(stdout, "Submapper: %d (0x%.2x)\n", cir_ines_get_submapper(), cir_ines_get_submapper());
	}
	
	// Get the PRG ROM size
	fprintf(stdout, "PRG ROM size in bytes: %d\n", cir_ines_get_prgrom() * 16384);
	//fprintf(stdout, "PRG ROM size in kbytes: %d\n", cir_ines_get_prgrom() * 16);
	
	// Check if PRG RAM or battery is present
	if (cir_ines_get_prgram_present()) {
		fprintf(stdout, "PRG RAM size in bytes: %d\n", cir_ines_get_prgram() * 8192);
		//fprintf(stdout, "PRG RAM size in kbytes: %.2f\n", cir_ines_get_prgram() / 1024.0);
		
		if (version == 2) {
			fprintf(stdout, "PRG NVRAM size in bytes: %d\n", cir_ines_get_prgnvram());
		}
	}
	
	// Check the CHR ROM size or detect CHR RAM
	if (cir_ines_get_chrrom()) {
		fprintf(stdout, "CHR ROM size in bytes: %d\n", cir_ines_get_chrrom() * 8192);
		//fprintf(stdout, "CHR ROM size in kbytes: %d\n", cir_ines_get_chrrom() * 8);
	}
	else {
		if (version == 2) {
			fprintf(stdout, "CHR RAM size in bytes: %d\n", cir_ines_get_chrram());
			fprintf(stdout, "CHR NVRAM size in bytes: %d\n", cir_ines_get_chrnvram());
		}
		else { fprintf(stdout, "CHR RAM: Present\n"); }
	}
	
	// Check the mirroring
	int mirroring = cir_ines_get_mirroring();
	
	if (mirroring == 2) {
		fprintf(stdout, "Mirroring: Four screen\n");
	}
	else if (mirroring == 1) {
		fprintf(stdout, "Mirroring: Vertical\n");
	}
	else {
		fprintf(stdout, "Mirroring: Horizontal\n");
	}
	
	// Check if there's a trainer
	fprintf(stdout, "512-byte trainer: %s\n", cir_ines_get_trainer() ? "Present" : "None");
	
	int system = cir_ines_get_system();
	if (system == 2) { // PC-10
		fprintf(stdout, "System: PlayChoice-10\n");
	}
	else if (system == 1) { // VS. System
		fprintf(stdout, "System: VS. System\n");
		
		// Detect VS. System hardware if it's NES 2.0
		if (version == 2) {
			// Check the VS. System's PPU hardware
			int vsppu = cir_ines_get_vsppu();
			
			if (vsppu == 0) {
				fprintf(stdout, "VS. System PPU: RP2C03B\n");
			}
			else if (vsppu == 1) {
				fprintf(stdout, "VS. System PPU: RP2C03G\n");
			}
			else if (vsppu == 2) {
				fprintf(stdout, "VS. System PPU: RP2C04-0001\n");
			}
			else if (vsppu == 3) {
				fprintf(stdout, "VS. System PPU: RP2C04-0002\n");
			}
			else if (vsppu == 4) {
				fprintf(stdout, "VS. System PPU: RP2C04-0003\n");
			}
			else if (vsppu == 5) {
				fprintf(stdout, "VS. System PPU: RP2C04-0004\n");
			}
			else if (vsppu == 6) {
				fprintf(stdout, "VS. System PPU: RC2C03B\n");
			}
			else if (vsppu == 7) {
				fprintf(stdout, "VS. System PPU: RC2C03C\n");
			}
			else if (vsppu == 8) {
				fprintf(stdout, "VS. System PPU: RC2C05-01\n");
			}
			else if (vsppu == 9) {
				fprintf(stdout, "VS. System PPU: RC2C05-02\n");
			}
			else if (vsppu == 10) {
				fprintf(stdout, "VS. System PPU: RC2C05-03\n");
			}
			else if (vsppu == 11) {
				fprintf(stdout, "VS. System PPU: RC2C05-04\n");
			}
			else if (vsppu == 12) {
				fprintf(stdout, "VS. System PPU: RC2C05-05\n");
			}
			
			// Check the VS. System's PPU Mode
			int vsmode = cir_ines_get_vsmode();
			
			if (vsmode == 0) {
				fprintf(stdout, "VS. System Mode: Standard\n");
			}
			else if (vsmode == 1) {
				fprintf(stdout, "VS. System Mode: RBI Baseball\n");
			}
			else if (vsmode == 2) {
				fprintf(stdout, "VS. System Mode: TKO Boxing\n");
			}
			else if (vsmode == 3) {
				fprintf(stdout, "VS. System Mode: Super Xevious\n");
			}
		}
	}
	else { // Home Console
		fprintf(stdout, "System: Home Console\n");
	}
	
	// TV System
	int tvsystem = cir_ines_get_tvsystem();
	if (tvsystem == 0) {
		fprintf(stdout, "TV System: NTSC\n");
	}
	else if (tvsystem == 1) {
		fprintf(stdout, "TV System: PAL\n");
	}
	else if (tvsystem == 2) {
		fprintf(stdout, "TV System: NTSC/PAL\n");
	}
	
	return 1;
}

void cir_cli_show_usage() {
	// Show usage
	fprintf(stdout, "Cirugía - A Surgical Tool for NES ROMs\n");
	fprintf(stdout, "Usage: cirugia [options] [FILE]\n");
	fprintf(stdout, "\nOptions:\n");
	fprintf(stdout, "  -v        Set Version (1-2)\n");
	fprintf(stdout, "  -m        Set Mapper (0-4095)\n");
	fprintf(stdout, "  -s        Set Submapper (0-15)\n");
	fprintf(stdout, "  -i        Set Mirroring (0-2)\n");
	fprintf(stdout, "  -j        Set PRG RAM Present (0-1)\n");
	fprintf(stdout, "  -t        Set Trainer (0-1)\n");
	fprintf(stdout, "  -q        Set System Type (0-2)\n");
	fprintf(stdout, "  -r        Set TV System/Region (0-2)\n");
	fprintf(stdout, "  -k        Set VS PPU Chip (0-12)\n");
	fprintf(stdout, "  -l        Set VS PPU Mode (0-3)\n");
	fprintf(stdout, "  -b        Set PRG ROM size (0-4095)\n");
	fprintf(stdout, "  -f        Set PRG RAM size (0-14)\n");
	fprintf(stdout, "  -g        Set PRG NVRAM size (0-14)\n");
	fprintf(stdout, "  -c        Set CHR ROM size (0-4095)\n");
	fprintf(stdout, "  -d        Set CHR RAM size (0-14)\n");
	fprintf(stdout, "  -e        Set CHR NVRAM size (0-14)\n");
	fprintf(stdout, "  -a        Apply IPS Patch (filename)\n");
	fprintf(stdout, "  -p        IPS Output File (filename)\n");
	fprintf(stdout, "  -x        Diff ROM (filename)\n");
	fprintf(stdout, "  -o        Output File (filename)\n");
}

int main(int argc, char* argv[]) {
	// The main function
	
	int patchfile = 0;
	int writefile = 0;
	int patchout = 0;
	int diff = 0;
	char patchfilepath[256];
	char outfilepath[256];
	char patchoutpath[256];
	char diffpath[256];
	
	if (argv[1] == NULL) {
		cir_cli_show_usage();
		exit(0);
	}
	
	// Load the ROM
	if (!cir_rom_load(argv[argc -1])) {
		fprintf(stderr, "FAIL: Unable to open %s\n", argv[argc -1]);
		exit(2);
	}
	
	// Split the header and the ROM
	cir_rom_split_header_rom();
	
	int imagetype = cir_header_validate();
	
	if (imagetype == 1) { // iNES or NES 2.0
		// Check header version
		version = cir_ines_get_version();
		
		// Get the CRC32 and SHA1 checksums
		fprintf(stdout, "CRC:  %X\n", cir_rom_get_crc());
		fprintf(stdout, "SHA1: %s\n", cir_rom_get_sha1());
		
		// Parse CLI options
		int c;
		while ((c = getopt(argc, argv, "a:b:c:d:e:f:g:i:j:k:l:m:o:p:q:r:s:t:v:x:")) != -1) {
			switch (c) {
				case 'a': // Apply IPS patch
					if (optarg) {
						patchfile = 1;
						snprintf(patchfilepath, sizeof(patchfilepath), "%s", optarg);
					}
					else {
						fprintf(stderr, "ERROR: -a must specify filename\n");
					}
					break;
				case 'b': // Set the PRG ROM size (0 - 4095)
					if (atoi(optarg) >= 0 && atoi(optarg) < 4095) {
						cir_ines_set_prgrom(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -b must be 0 to 4095: skipping\n");
					}
					break;
				case 'c': // Set the CHR ROM size (0 - 4095)
					if (atoi(optarg) >= 0 && atoi(optarg) < 4095) {
						cir_ines_set_chrrom(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -c must be 0 to 4095: skipping\n");
					}
					break;
				case 'd': // Set the CHR RAM size (0 - 14)
					if (atoi(optarg) >= 0 && atoi(optarg) < 15) {
						cir_ines_set_chrram(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -d must be 0 to 14: skipping\n");
					}
					break;
				case 'e': // Set the CHR NVRAM size (0 - 14)
					if (atoi(optarg) >= 0 && atoi(optarg) < 15) {
						cir_ines_set_chrnvram(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -e must be 0 to 14: skipping\n");
					}
					break;
				case 'f': // Set the PRG RAM size (0 - 14)
					if (atoi(optarg) >= 0 && atoi(optarg) < 15) {
						cir_ines_set_prgram(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -f must be 0 to 14: skipping\n");
					}
					break;
				case 'g': // Set the PRG NVRAM size (0 - 14)
					if (atoi(optarg) >= 0 && atoi(optarg) < 15) {
						cir_ines_set_prgnvram(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -g must be 0 to 14: skipping\n");
					}
					break;
				case 'i': // Set Mirroring (0 - 2)
					if (atoi(optarg) >= 0 && atoi(optarg) < 3) {
						cir_ines_set_mirroring(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -i must be 0 to 2: skipping\n");
					}
					break;
				case 'j': // Set the PRG RAM present bit (0 - 1)
					if (atoi(optarg) == 0 && atoi(optarg) == 1) {
						cir_ines_set_prgram_present(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -j must be 0 to 1: skipping\n");
					}
					break;
				case 'k': // Set the VS PPU Chip (0 - 12)
					if (atoi(optarg) >= 0 && atoi(optarg) < 13) {
						cir_ines_set_vsppu(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -k must be 0 to 12: skipping\n");
					}
					break;
				case 'l': // Set the VS PPU Mode (0 - 3)
					if (atoi(optarg) >= 0 && atoi(optarg) < 4) {
						cir_ines_set_vsmode(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -l must be 0 to 3: skipping\n");
					}
					break;
				case 'm': // Set the mapper (0 - 4095)
					if (atoi(optarg) >= 0 && atoi(optarg) < 4096) {
						cir_ines_set_mapper(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -m must be 0 to 4096: skipping\n");
					}
					break;
				case 'o': // Write a new file
					if (optarg) {
						writefile = 1;
						snprintf(outfilepath, sizeof(outfilepath), "%s", optarg);
					}
					else {
						fprintf(stderr, "ERROR: -o must specify filename\n");
					}
					break;
				case 'p': // IPS patch output file path
					if (optarg) {
						patchout = 1;
						snprintf(patchoutpath, sizeof(patchoutpath), "%s", optarg);
					}
					else {
						fprintf(stderr, "ERROR: -p must specify filename\n");
					}
					break;
				case 'q': // Set System (0 - 2)
					if (atoi(optarg) >= 0 && atoi(optarg) < 3) {
						cir_ines_set_system(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -i must be 0 to 2: skipping\n");
					}
					break;
				case 'r': // Set TV System (0 - 2)
					if (atoi(optarg) >= 0 && atoi(optarg) < 3) {
						cir_ines_set_tvsystem(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -i must be 0 to 2: skipping\n");
					}
					break;
				case 's': // Set the mapper (0 - 15)
					if (atoi(optarg) >= 0 && atoi(optarg) < 16) {
						cir_ines_set_submapper(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -s must be 0 to 15: skipping\n");
					}
					break;
				case 't': // Set the trainer bit (0/1)
					if (atoi(optarg) == 0 || atoi(optarg) == 1) {
						cir_ines_set_trainer(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -t must be 0 or 1: skipping\n");
					}
					break;
				case 'v': // Set header version (1 - 2)
					if (atoi(optarg) == 1 || atoi(optarg) == 2) {
						cir_ines_set_version(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -v must be 1 or 2: skipping\n");
					}
					break;
				case 'x': // Diff ROM (filename)
					if (optarg) {
						diff = 1;
						snprintf(diffpath, sizeof(diffpath), "%s", optarg);
					}
					else {
						fprintf(stderr, "ERROR: -x must specify filename\n");
					}
					break;
				default:
					break;
			}
		}
		
		// If there is a ROM to diff against, load it
		if (diff) {
			if (!cir_rom_load_diff(diffpath)) {
				fprintf(stderr, "FAIL: Unable to open %s\n", diffpath);
				diff = 0;
			}
			else {
				// Do the actual diffing
				cir_ips_diff();
			}
		}
		
		// If there is an IPS patch file to be written, write it
		if (patchout) {
			if (diff) {
				if (!cir_ips_write(patchoutpath)) {
					fprintf(stderr, "FAIL: Unable to open %s\n", patchoutpath);
				}
			}
			else {
				fprintf(stderr, "FAIL: Must compare files to create patch\n");
			}
		}
		
		// Patch the file and write if it was specified
		if (patchfile) {
			if (!cir_ips_load(patchfilepath)) {
				fprintf(stderr, "FAIL: Unable to open %s\n", patchfilepath);
			}
			else {
				// Parse (don't apply) the IPS patch to find out stats
				cir_ips_parse(0);
				// Write the file if it was specified
				if (!cir_ips_rom_write(outfilepath)) {
					fprintf(stderr, "FAIL: Unable to write %s\n", outfilepath);
				}
			}
		}
		// Otherwise just write a file if it was specified
		else if (writefile) {
			if (!cir_rom_write(outfilepath)) {
				fprintf(stderr, "FAIL: Unable to write %s\n", outfilepath);
			}
		}
		
		// Spit out the new information
		cir_cli_header_parse();
	}
	else if (imagetype == 2) { // FDS Disk Image
		fprintf(stdout, "Header Type: FDS Image\n");
		
		// Number of disk sides
		fprintf(stdout, "Disk sides: %d\n", cir_fds_get_disksides());
		
		// Disk Verification string
		if (cir_fds_get_diskverification()) {
			fprintf(stdout, "Disk Verification: Pass\n");
		}
		else {
			fprintf(stderr, "Disk Verification: Fail\n");
		}
		
		// Manufacturer Code
		uint8_t mfrcode = cir_fds_get_mfrcode();
		fprintf(stdout, "Manufacturer: %02X - %s\n", mfrcode, mfrcodes[mfrcode]);
		
		// 3-letter Game Name
		fprintf(stdout, "Game Name: %s\n", cir_fds_get_gamename());
		
		// Game Type
		switch (cir_fds_get_gametype()) {
			case 0x20: fprintf(stdout, "Game Type: Normal\n"); break;
			case 0x45: fprintf(stdout, "Game Type: Event\n"); break;
			case 0x52: fprintf(stdout, "Game Type: Reduction in Price\n"); break;
			default: fprintf(stderr, "FAIL: Game Type\n");
		}
		
		// Game Revision
		fprintf(stdout, "Game Revision: %d\n", cir_fds_get_revision());
		
		// Side Number
		fprintf(stdout, "Disk Number: %d, Side %s\n", cir_fds_get_disknum() + 1, cir_fds_get_sidenum() == 0x00 ? "A" : "B");
		
		// Disk Type
		fprintf(stdout, "Disk Type: %s\n", cir_fds_get_disktype() == 0x00 ? "FMC" : "FSC");
		
		// Boot Read File Code
		fprintf(stdout, "Boot Read File Code: 0x%02X\n", cir_fds_get_bootreadfile());
		
		// Manufacture Date - BCD Date Format is nonsense
		char mfrdate[3] = {0};
		cir_fds_get_mfrdate(&mfrdate[0]);		
		if (mfrdate[0] != 0) { fprintf(stdout, "Manufacture Date: %d-%02x-%02x\n", (((mfrdate[0] >> 4) * 10) + (mfrdate[0] & 0xf)) + 1925, mfrdate[1], mfrdate[2]); }
		
		// Rewritten Date - BCD Date Format again...
		char rwdate[3] = {0};
		cir_fds_get_rwdate(&rwdate[0]);
		if (rwdate[0] != 0) { fprintf(stdout, "Rewritten Date: %d-%02x-%02x\n", (((rwdate[0] >> 4) * 10) + (rwdate[0] & 0xf)) + 1925, rwdate[1], rwdate[2]); }
		
		// Rewrite Count
		fprintf(stdout, "Rewrite Count: %X\n", cir_fds_get_rwcount());
		
		// Actual Side Number
		fprintf(stdout, "Actual Disk Side: %s\n", cir_fds_get_sidenum_actual() == 0x00 ? "A" : "B");
		
		// Price
		fprintf(stdout, "Price Code: 0x%02X\n", cir_fds_get_price());
	}
	else { fprintf(stdout, "FAIL: No Header or Invalid Image\n"); }
	
	// Exit
	cir_rom_cleanup();
	return 0;
}
