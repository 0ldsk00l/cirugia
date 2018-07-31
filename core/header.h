#ifndef HEADER_H
#define HEADER_H

int cir_header_validate();

int cir_ines_get_version();
int cir_ines_get_prgrom();
int cir_ines_get_prgram_present();
int cir_ines_get_prgram();
int cir_ines_get_prgnvram();
int cir_ines_get_chrram();
int cir_ines_get_chrnvram();
int cir_ines_get_chrrom();
int cir_ines_get_mapper();
int cir_ines_get_submapper();
int cir_ines_get_mirroring();
int cir_ines_get_trainer();
int cir_ines_get_system();
int cir_ines_get_tvsystem();
int cir_ines_get_vsppu();
int cir_ines_get_vsmode();

void cir_ines_set_version(uint8_t data);
void cir_ines_set_prgrom(uint16_t data);
void cir_ines_set_prgram_present(uint8_t data);
void cir_ines_set_prgram(uint8_t data);
void cir_ines_set_prgnvram(uint8_t data);
void cir_ines_set_chrram(uint8_t data);
void cir_ines_set_chrnvram(uint8_t data);
void cir_ines_set_chrrom(uint16_t data);
void cir_ines_set_mapper(uint16_t data);
void cir_ines_set_submapper(uint8_t data);
void cir_ines_set_mirroring(uint8_t data);
void cir_ines_set_trainer(uint8_t data);
void cir_ines_set_system(uint8_t data);
void cir_ines_set_tvsystem(uint8_t data);
void cir_ines_set_vsppu(uint8_t data);
void cir_ines_set_vsmode(uint8_t data);

uint8_t cir_fds_get_disksides();
uint8_t cir_fds_get_diskverification();
uint8_t cir_fds_get_mfrcode();
char *cir_fds_get_gamename();
uint8_t cir_fds_get_gametype();
uint8_t cir_fds_get_revision();

#endif
