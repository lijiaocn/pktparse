#ifndef HOST_H
#define HOST_H

#include "public/netstat.h"

Ip4Host *host_ip4_new(u_int32 addr);
Ip6Host *host_ip6_new(u_char addr[16]);
void host_ip4_free(gpointer host);
void host_ip6_free(gpointer host);
gint host_ip4_keycmp(gconstpointer a, gconstpointer b, gpointer user_data);
gint host_ip6_keycmp(gconstpointer a, gconstpointer b, gpointer user_data);

#endif //HOST_H

