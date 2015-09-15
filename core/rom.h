#ifndef ROM_H
#define ROM_H

#define HEADERSIZE 16

void cir_rom_load(const char *filepath);
int cir_rom_write(const char *filepath);
uint32_t cir_rom_get_crc();
void cir_rom_split_header_rom();
void cir_rom_cleanup();

#endif
