#ifndef SHM_CLIENT_H
#define SHM_CLIENT_H

#include "shm.hpp"
#include "common.hpp"

extern const char *pkt_origin_str[];
extern const char *pkt_origin_desc[];

void main_loop(entry_callback_t cb, void *arg);

#endif
