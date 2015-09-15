#ifndef HEADER_H
#define HEADER_H

int cir_header_validate();
int cir_header_get_version();
int cir_header_get_prgrom();
int cir_header_get_prgram_present();
int cir_header_get_prgram();
int cir_header_get_prgnvram();
int cir_header_get_chrram();
int cir_header_get_chrnvram();
int cir_header_get_chrrom();
int cir_header_get_mapper();
int cir_header_get_submapper();
int cir_header_get_mirroring();
int cir_header_get_trainer();
int cir_header_get_system();
int cir_header_get_tvsystem();
int cir_header_get_vsppu();
int cir_header_get_vsmode();

void cir_header_set_version(uint8_t data);
void cir_header_set_prgrom(uint16_t data);
void cir_header_set_prgram_present(uint8_t data);
void cir_header_set_prgram(uint8_t data);
void cir_header_set_prgnvram(uint8_t data);
void cir_header_set_chrram(uint8_t data);
void cir_header_set_chrnvram(uint8_t data);
void cir_header_set_chrrom(uint16_t data);
void cir_header_set_mapper(uint16_t data);
void cir_header_set_submapper(uint8_t data);
void cir_header_set_mirroring(uint8_t data);
void cir_header_set_trainer(uint8_t data);
void cir_header_set_system(uint8_t data);
void cir_header_set_tvsystem(uint8_t data);
void cir_header_set_vsppu(uint8_t data);
void cir_header_set_vsmode(uint8_t data);

#endif
