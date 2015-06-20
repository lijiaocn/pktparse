/*
 * =====================================================================================
 *
 *       Filename:  host.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/05/2014 08:14:59 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "net/host.h"
#include "net/frags.h"
#include "tran/port.h"
#include "string.h"

Ip4Host *host_ip4_new(u_int32 addr)
{
	Ip4Host *host = g_slice_new0(Ip4Host);

	host->addr = addr;
	host->frags = g_tree_new_full(ip_frags_hdr_keycmp, NULL, NULL, ip_frags_hdr_free);
	host->tcpports = g_tree_new_full(port_keycmp, NULL, NULL, tcp_port_free);
	host->udpports = g_tree_new_full(port_keycmp, NULL, NULL, udp_port_free);

	return host;
}

Ip6Host *host_ip6_new(u_char addr[16])
{
	Ip6Host *host = g_slice_new0(Ip6Host);  //必须全部设置成0, 否则出错

	memcpy(host->addr, addr, 16);
	host->frags = g_tree_new_full(ip_frags_hdr_keycmp, NULL, NULL, ip_frags_hdr_free);
	host->tcpports = g_tree_new_full(port_keycmp, NULL, NULL, tcp_port_free);
	host->udpports = g_tree_new_full(port_keycmp, NULL, NULL, udp_port_free);

	return host;
}

void host_ip4_free(gpointer host)
{
	g_tree_destroy(((Ip4Host *)host)->frags);
	g_tree_destroy(((Ip4Host *)host)->tcpports);
	g_tree_destroy(((Ip4Host *)host)->udpports);

	g_slice_free(Ip4Host, host);
}

void host_ip6_free(gpointer host)
{
	g_tree_destroy(((Ip6Host *)host)->frags);
	g_tree_destroy(((Ip6Host *)host)->tcpports);
	g_tree_destroy(((Ip6Host *)host)->udpports);

	g_slice_free(Ip6Host, host);
}

/**
 * @brief hosts tree的比较函数
 *
 */
gint host_ip4_keycmp(gconstpointer a, gconstpointer b, gpointer user_data)
{
	u_int32 *tmpa = (u_int32 *)a;
	u_int32 *tmpb = (u_int32 *)b;

	return (tmpa - tmpb);
}

gint host_ip6_keycmp(gconstpointer a, gconstpointer b, gpointer user_data)
{
	return (g_str_hash(a) - g_str_hash(b));
}
