/*
 * =====================================================================================
 *
 *       Filename:  display.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/14/2014 09:33:27 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include "public/utils/display.h"
#include "ljac.h"

//////////////////   display udp port   ////////////////

void display_udp_port(UdpPort *udpport)
{
	printf("port: %u actnum: %u tolnum: %u\n", udpport->port, udpport->actnum, udpport->tolnum);
	//TODO display TcpStat
	//TODO display AppHandle
}

gboolean _display_udp_port_tree(gpointer port, gpointer udpport, gpointer data)
{
	display_udp_port((UdpPort *)udpport);

	return FALSE;   //CONTINUE
}

void display_udp_ports(GTree *udpports)
{
	g_tree_foreach(udpports, _display_udp_port_tree, NULL);
}

//////////////////   display tcp port   ////////////////

void display_tcp_port(TcpPort *tcpport)
{
	printf("port: %u actnum: %u tolnum: %u\n", tcpport->port, tcpport->actnum, tcpport->tolnum);
	//TODO display TcpStat
	//TODO display AppHandle
}

gboolean _display_tcp_port_tree(gpointer port, gpointer tcpport, gpointer data)
{
	display_tcp_port((TcpPort *)tcpport);

	return FALSE;   //CONTINUE
}

void display_tcp_ports(GTree *tcpports)
{
	g_tree_foreach(tcpports, _display_tcp_port_tree, NULL);
}

//////////////////   display frag   ////////////////

void display_ip_frag(IpFrag *frag)
{
	printf("offset: %u valid_offset: %u valid_size: %u data_size: %u\n", 
			frag->offset, frag->valid_offset, frag->valid_size, frag->data_size);

	//TODO printf ipfrag->data
}

void _display_ip_frag_list(gpointer data, gpointer user_data)
{
	display_ip_frag((IpFrag *)data);
}

void display_ip_frags_hdr(IpFragsHdr *fragshdr)
{
	printf("identi: %u num: %u size: %u", fragshdr->identi, fragshdr->num,  fragshdr->size);
	g_list_foreach(fragshdr->list, _display_ip_frag_list, NULL);
}

gboolean _display_ip_frags_hdr_tree(gpointer identi, gpointer ipfrags_hdr, gpointer data)
{
	display_ip_frags_hdr((IpFragsHdr *)ipfrags_hdr);

	return FALSE;   //CONTINUE
}

void display_ip_frags(GTree *frags)
{
	g_tree_foreach(frags, _display_ip_frags_hdr_tree, NULL);
}

//////////////////   display ip6 host   ////////////////

void display_ip6host(Ip6Host *ip6host)
{
	lja_print_X("ip:", NULL," ", ip6host->addr, 16);
	printf("portnum: %u\n", ip6host->portnum);

	//display frags
	display_ip_frags(ip6host->frags);
	
	//display tcports
	display_tcp_ports(ip6host->tcpports);
	
	//display udports
	//
	display_udp_ports(ip6host->udpports);
}

gboolean _display_ip6host_tree(gpointer addr, gpointer host, gpointer data)
{
	display_ip6host((Ip6Host *)host);

	return FALSE;   //CONTINUE
}

void display_ip6hosts_tree(GTree *ip6hosts)
{
	g_tree_foreach(ip6hosts, _display_ip6host_tree, NULL);
}

//////////////////   display ip4 host   ////////////////

void display_ip4host(Ip4Host *ip4host)
{
	struct in_addr in;
	in.s_addr = ip4host->addr;
	printf("ip: %s portnum: %u\n",inet_ntoa(in), ip4host->portnum);

	//display frags
	display_ip_frags(ip4host->frags);
	
	//display tcports
	display_tcp_ports(ip4host->tcpports);
	
	//display udports
	display_udp_ports(ip4host->udpports);
}

gboolean _display_ip4host_tree(gpointer addr, gpointer host, gpointer data)
{
	display_ip4host((Ip4Host *)host);

	return FALSE;   //CONTINUE
}

void display_ip4hosts_tree(GTree *ip4hosts)
{
	g_tree_foreach(ip4hosts, _display_ip4host_tree, NULL);
}


///////////////////  display dev  //////////////////

void display_dev(Device *dev)
{
	lja_print_X("MAC: ", "\n"," ", dev->mac, 6);
	display_ip4hosts_tree(dev->ip4hosts);
	display_ip6hosts_tree(dev->ip6hosts);
}

void _display_dev_hash(gpointer key, gpointer value, gpointer user_data)
{
	display_dev((Device*)value);
}

void display_devs(ParseContext *context)
{
	g_hash_table_foreach(context->devs, _display_dev_hash, NULL);
}

