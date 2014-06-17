#ifndef UDPSTAT_H
#define UDPSTAT_H

#include "public/netstat.h"

UdpStat *udp_stat_new(PortPair portpair);
void udp_stat_free(gpointer stat);
guint udp_stat_hash(gconstpointer key);
gboolean udp_stat_equal(gconstpointer a, gconstpointer b);

#endif //UDPSTAT_H

