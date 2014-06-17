/*
 * =====================================================================================
 *
 *       Filename:  device.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/05/2014 06:13:09 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "link/device.h"
#include "net/host.h"
#include "string.h"

Device *device_new(u_char mac[])
{
	Device *dev = g_slice_new0(Device);

	memcpy(dev->mac, mac, 6);
	dev->ip4hosts = g_tree_new_full(host_ip4_keycmp, NULL, NULL, host_ip4_free);
	dev->ip6hosts = g_tree_new_full(host_ip6_keycmp, NULL, NULL, host_ip6_free);
	
	return dev;
}

void device_free(gpointer dev)
{
	g_tree_destroy(((Device *)dev)->ip4hosts);
	g_tree_destroy(((Device *)dev)->ip6hosts);

	g_slice_free(Device, dev);
}

gboolean device_equal(gconstpointer a, gconstpointer b)
{
	if(strncmp((u_char*)a, (u_char *)b, 6)){
		return FALSE;
	}
	return TRUE;
}

guint device_hash(gconstpointer a)
{
	return *((u_int32*)(((char *)a)+2));
}
