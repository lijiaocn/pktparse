/*
 * =====================================================================================
 *
 *       Filename:  frags.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/06/2014 06:27:12 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "net/frags.h"
#include "string.h"

IpFrag *ip_frag_new(u_int16 offset, u_char *payload, u_int16 size)
{
	IpFrag *frag = g_slice_alloc0(sizeof(IpFrag)+size+1);  //最后一个字节确保是0

	frag->offset = offset;
	frag->valid_size = size;   //默认全部有效, 在插入hdr时纠正有效值
	frag->data_size = size;
	memcpy(frag->data, payload, size);

	return frag;
}

void ip_frag_free(gpointer frag)
{
	gsize size = ((IpFrag*)frag)->data_size + sizeof(IpFrag) + 1;

	g_slice_free1(size, frag);
}

IpFragsHdr *ip_frags_hdr_new(u_int16 identi)
{
	IpFragsHdr *list = g_slice_new0(IpFragsHdr);

	list->identi = identi;

	return list;
}

void ip_frags_hdr_free(gpointer hdr)
{
	//释放分片
	g_list_free_full(((IpFragsHdr *)hdr)->list, ip_frag_free);

	//释放头
	g_slice_free(IpFragsHdr, hdr);
}


gint ip_frags_hdr_keycmp(gconstpointer a , gconstpointer b, gpointer user_data)
{
	u_int16 tmpa = *(u_int16*)a;
	u_int16 tmpb = *(u_int16*)b;

	return (tmpa - tmpb);
}
