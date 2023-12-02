#ifndef OUTPUT_SHM_H
#define OUTPUT_SHM_H

#include "shm.hpp"
#include "addrwatch.hpp"

#define DEFAULT_SHM_LOG_SIZE 1024

void output_shm_init();
void output_shm_reload();
void output_shm_save(struct pkt *p, char *mac_str, char *ip_str);
void output_shm_close();

#endif
