#ifndef PARSEPKT_H
#define PARSEPKT_H

#include "context.h"
#include "pktstat.h"

int pktparse_init(ParseContext *context);
void pktparse_free(ParseContext *context);
void pktparse_pkt(ParseContext *context, PktStat *pktstat);

#endif //PARSEPKT_H

