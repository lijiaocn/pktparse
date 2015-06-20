/*
 * =====================================================================================
 *
 *       Filename:  udp_stat.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/07/2014 04:54:09 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "tran/udpstat.h"

UdpStat *udp_stat_new(PortPair portpair)
{
	UdpStat *stat =  g_slice_new0(UdpStat);
	stat->portpair = portpair;

	return stat;
}

void udp_stat_free(gpointer stat)
{
	g_slice_free(UdpStat, stat);
}

guint udp_stat_hash(gconstpointer key)
{
	guint index1 = (u_int64)(((PortPair*)key)->sport) & 0xFFFF;
	guint index2 = (u_int64)(((PortPair*)key)->dport) & 0xFFFF0000;

	return (index1 + index2);
}

gboolean udp_stat_equal(gconstpointer a, gconstpointer b)
{
	if((((PortPair*)a)->sport == ((PortPair *)b)->sport)
		&&(((PortPair*)a)->dport == ((PortPair *)b)->dport)){

		return TRUE;
	}
	return FALSE;
}
