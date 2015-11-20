#ifndef IPS_H
#define IPS_H

#define IPS_HEADER_SIZE 5
#define IPS_RECORD_SIZE 5

int cir_ips_load(const char *filepath);
int cir_ips_validate();
int cir_ips_apply();

#endif
