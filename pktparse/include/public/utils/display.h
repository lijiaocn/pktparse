#ifndef DISPLAY_H
#define DISPLAY_H

#include "public/context.h"
#include "public/netstat.h"

void display_udp_port(UdpPort *udpport);
void display_udp_ports(GTree *udpports);

void display_tcp_port(TcpPort *tcpport);
void display_tcp_ports(GTree *tcpports);

void display_ip_frag(IpFrag *frag);
void display_ip_frags_hdr(IpFragsHdr *fragshdr);
void display_ip_frags(GTree *frags);

void display_ip6host(Ip6Host *ip6host);
void display_ip6hosts_tree(GTree *ip6hosts);

void display_ip4host(Ip4Host *ip4host);
void display_ip4hosts_tree(GTree *ip4hosts);

void display_dev(Device *dev);
void display_devs_hash(ParseContext *context);

#endif //DISPLAY_H

