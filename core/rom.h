#ifndef ROM_H
#define ROM_H

#define HEADERSIZE 16

int cir_rom_load(const char *filepath);
int cir_rom_write(const char *filepath, int patch);
uint32_t cir_rom_get_crc();
const char* cir_rom_get_sha1();
void cir_rom_split_header_rom();
void cir_rom_cleanup();

#endif
