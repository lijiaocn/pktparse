#ifndef TCPSTAT_H
#define TCPSTAT_H

#include "public/netstat.h"

TcpCache *tcp_cache_new(u_int32 seq, u_char *payload, u_int32 size);
TcpStat *tcp_stat_new(PortPair portpair);
void tcp_cache_free(gpointer cache);
void tcp_stat_free(gpointer stat);
guint tcp_stat_hash(gconstpointer key);
gboolean tcp_stat_equal(gconstpointer a, gconstpointer b);

#endif //TCPSTAT_H

