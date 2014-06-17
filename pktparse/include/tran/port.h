#ifndef PORT_H
#define PORT_H

#include "public/netstat.h"

TcpPort *tcp_port_new(u_int16 port);
void tcp_port_free(gpointer port);
UdpPort *udp_port_new(u_int16 port);
void udp_port_free(gpointer port);
gint port_keycmp(gconstpointer a, gconstpointer b, gpointer user_data);

#endif //PORT_H

