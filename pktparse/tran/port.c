/*
 * =====================================================================================
 *
 *       Filename:  port.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/06/2014 09:03:36 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "tran/port.h"

TcpPort *tcp_port_new(u_int16 port)
{
	TcpPort *tcpport = g_slice_new0(TcpPort);

	tcpport->port = port;

	return tcpport;
}

void tcp_port_free(gpointer port)
{
	tcp_stat_free(((TcpPort *)port)->stat);
	//TODO
	//app_handle_free(((TcpPort *)port)->app);

	g_slice_free(TcpPort, port);
}

UdpPort *udp_port_new(u_int16 port)
{
	UdpPort *udpport = g_slice_new0(UdpPort);

	udpport->port = port;

	return udpport;
}

void udp_port_free(gpointer port)
{
	udp_stat_free(((UdpPort *)port)->stat);
	//TODO
	//app_handle_free(((UdpPort *)port)->app);

	g_slice_free(UdpPort, port);
}

gint port_keycmp(gconstpointer a, gconstpointer b, gpointer user_data)
{
	u_int16 *tmpa = (u_int16 *)a;
	u_int16 *tmpb = (u_int16 *)b;

	return (tmpa - tmpb);
}
