#ifndef PROCESS_H
#define PROCESS_H

#include "addrwatch.hpp"

void process_arp(struct pkt *p);
void process_ns(struct pkt *p);
void process_na(struct pkt *p);

#endif
