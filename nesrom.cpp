#include <fstream>
#include <iostream>
#include <vector>

#include "hasher.h"
#include "nesrom.h"

bool nesrom::load() {
    std::vector<uint8_t> ident{ 0x4e, 0x45, 0x53, 0x1a }; // N, E, S, EOF

    std::ifstream is(filename, std::ios::in | std::ios::binary);
    std::vector<uint8_t> filedata((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
    is.close();

    if (!std::equal(filedata.begin(), filedata.begin() + 4, ident.begin())) {
        std::cout << "Invalid Identification String\n";
        return false;
    }

    // PRG ROM Size in 16K Blocks
    hdr.prgsize = filedata[4];

    // CHR ROM Size in 8K Blocks
    hdr.chrsize = filedata[5]; // 0 means CHR RAM for iNES

    // Mapper
    hdr.mapper = (filedata[7] & 0xf0) | ((filedata[6] & 0xf0) >> 4);

    // Mirroring
    if (filedata[6] & 0x08) {
        hdr.mirroring = header::MIRRORING_4;
    }
    else {
        hdr.mirroring = filedata[6] & 0x01;
    }

    // Battery
    hdr.battery = filedata[6] & 0x02;

    // Trainer
    hdr.trainer = filedata[6] & 0x04;

    // System type: 0 = NES/Famicom, 1 = Vs. System, 2 = PC10, 3 = Extended
    hdr.systype = filedata[7] & 0x03;

    // Deduce version: iNES or NES 2.0
    hdr.version = (filedata[7] & 0x0c) == 0x08 ? 2 : 1;

    if (hdr.version == 2) {
        // Mapper
        hdr.mapper |= ((filedata[8] & 0x0f) << 8);

        // Submapper
        hdr.submapper = ((filedata[8] & 0xf0) >> 4);

        // PRG ROM Size
        hdr.prgsize |= ((filedata[9] & 0x0f) << 8);

        // CHR ROM Size
        hdr.chrsize |= ((filedata[9] & 0xf0) << 4);

        // PRG RAM Shift Count (64 << shiftcount)
        hdr.prgramsize = filedata[10] & 0x0f;

        // PRG NVRAM Shift Count (64 << shiftcount)
        hdr.prgnvramsize = (filedata[10] & 0xf0) >> 4;

        // CHR RAM Size
        hdr.chrramsize = filedata[11] & 0x0f;

        // CHR NVRAM Shift Count (64 << shiftcount)
        hdr.chrnvramsize = (filedata[11] & 0xf0) >> 4;

        // CPU/PPU Timing
        hdr.timing = filedata[12] & 0x03;

        if (hdr.systype == 1) { // Vs. System Type
            hdr.vsppu = filedata[13] & 0x0f;
            hdr.vshardware = (filedata[13] & 0xf0) >> 4;
        }
        else if (hdr.systype == 3) { // Extended System Type
            hdr.extsys = filedata[13] & 0x0f;
        }
        else {
            hdr.vsppu = 0;
            hdr.vshardware = 0;
            hdr.extsys = 0;
        }

        // Miscellaneous ROMs
        hdr.miscroms = filedata[14] & 0x03;

        // Default Expansion Device
        hdr.expdev = filedata[15] & 0x3f;
    }
    else {
        // TV System
        hdr.tvsystem = filedata[9] & 0x01;
        hdr.submapper = 0;
        hdr.prgramsize = 0;
        hdr.prgnvramsize = 0;
        hdr.chrramsize = 0;
        hdr.chrnvramsize = 0;
        hdr.timing = 0;
        hdr.vsppu = 0;
        hdr.vshardware = 0;
        hdr.extsys = 0;
        hdr.miscroms = 0;
        hdr.expdev = 0;
    }

    // Copy PRG and CHR
    int prgend = 16 + (hdr.prgsize * 16384);
    prg.assign(filedata.begin() + 16, filedata.begin() + prgend);
    if (hdr.chrsize) chr.assign(filedata.begin() + prgend, filedata.end());

    // CRC32 Checksums
    crc = crc32(0, filedata.data() + 16, filedata.size() - 16);
    prgcrc = crc32(0, prg.data(), prg.size());
    chrcrc = crc32(0, chr.data(), chr.size());

    // SHA1 Checksum
    SHA1 checksum;
    checksum.update(std::string(filedata.begin() + 16, filedata.end()));
    sha = checksum.final();

    return true;
}

void nesrom::save() {
    // Empty 16-byte header with N, E, S, EOF
    std::vector<uint8_t> newhdr{ 0x4e, 0x45, 0x53, 0x1a, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    // PRG ROM Size in 16K Blocks
    newhdr[4] = hdr.prgsize & 0xff;

    // CHR ROM Size in 8K Blocks
    newhdr[5] = hdr.chrsize & 0xff; // 0 means CHR RAM for iNES

    // Mapper
    newhdr[6] |= ((hdr.mapper & 0x0f) << 4);
    newhdr[7] |= (hdr.mapper & 0xf0);

    // Mirroring
    if (hdr.mirroring == header::MIRRORING_4) {
        newhdr[6] |= 0x08;
    }
    else {
        newhdr[6] |= hdr.mirroring;
    }

    // Battery
    if (hdr.battery) newhdr[6] |= 0x02;

    // Trainer
    if (hdr.trainer) newhdr[6] |= 0x04;

    // System type: 0 = NES/Famicom, 1 = Vs. System, 2 = PC10, 3 = Extended
    newhdr[7] |= hdr.systype;

    // TV System
    if (hdr.version == 1) newhdr[9] = hdr.tvsystem & 0x01;

    // Version: iNES or NES 2.0
    if (hdr.version == 2) {
        // Set the 2.0 identifier
        newhdr[7] |= 0x08;

        // Mapper
        newhdr[8] |= ((hdr.mapper >> 8) & 0x0f);

        // Submapper
        newhdr[8] |= ((hdr.submapper << 4) & 0xf0);

        // PRG ROM Size
        newhdr[9] |= ((hdr.prgsize >> 8) & 0x0f);

        // CHR ROM Size
        newhdr[9] |= ((hdr.chrsize >> 4) & 0xf0);

        // PRG RAM Shift Count (64 << shiftcount)
        newhdr[10] |= hdr.prgramsize & 0x0f;

        // PRG NVRAM Shift Count (64 << shiftcount)
        newhdr[10] |= ((hdr.prgnvramsize & 0x0f) << 4);

        // CHR RAM Size
        newhdr[11] |= hdr.chrramsize & 0x0f;

        // CHR NVRAM Shift Count (64 << shiftcount)
        newhdr[11] |= ((hdr.chrnvramsize & 0x0f) << 4);

        // CPU/PPU Timing
        newhdr[12] = hdr.timing & 0x03;

        if (hdr.systype == 1) { // Vs. System Type
            newhdr[13] |= hdr.vsppu & 0x0f;
            newhdr[13] |= (hdr.vshardware & 0x0f) << 4;
        }
        else if (hdr.systype == 3) { // Extended System Type
            newhdr[13] = hdr.extsys & 0x0f;
        }

        // Miscellaneous ROMs
        newhdr[14] = hdr.miscroms & 0x03;

        // Default Expansion Device
        newhdr[15] = hdr.expdev & 0x3f;
    }

    std::ofstream os(filename, std::ios::out | std::ios::binary);
    if (os.is_open()) {
        os.write((const char*)newhdr.data(), newhdr.size());
        os.write((const char*)prg.data(), prg.size());
        os.write((const char*)chr.data(), chr.size());
        os.close();
    }
}

void nesrom::save(std::string fn) {
    filename = fn;
    save();
}
