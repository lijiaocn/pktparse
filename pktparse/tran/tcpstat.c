/*
 * =====================================================================================
 *
 *       Filename:  tcp_stat.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/07/2014 03:37:41 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "tran/tcpstat.h"
#include "string.h"

TcpCache *tcp_cache_new(u_int32 seq, u_char *payload, u_int32 size)
{
	TcpCache *cache = (TcpCache *)g_slice_alloc0(sizeof(TcpCache)+size+1);
	cache->seq = seq;
	cache->valid_size = size;
	cache->data_size = size;
	memcpy(cache->data, payload, size);

	return cache;
}

TcpStat *tcp_stat_new(PortPair portpair)
{
	TcpStat *stat =  g_slice_new0(TcpStat);
	stat->portpair = portpair;

	return stat;
}

void tcp_cache_free(gpointer cache)
{
	gsize size = sizeof(TcpCache) + ((TcpCache*)cache)->data_size + 1;
	g_slice_free1(size, cache);
}

void tcp_stat_free(gpointer stat)
{
	g_list_free_full(((TcpStat *)stat)->tcp_cache, tcp_cache_free);
	g_slice_free(TcpStat, stat);
}

guint tcp_stat_hash(gconstpointer key)
{
	guint index1 = (u_int64)(((PortPair*)key)->sport) & 0xFFFF;
	guint index2 = (u_int64)(((PortPair*)key)->dport) & 0xFFFF0000;

	return (index1 + index2);
}

gboolean tcp_stat_equal(gconstpointer a, gconstpointer b)
{
	if((((PortPair*)a)->sport == ((PortPair *)b)->sport)
		&&(((PortPair*)a)->dport == ((PortPair *)b)->dport)){

		return TRUE;
	}
	return FALSE;
}
