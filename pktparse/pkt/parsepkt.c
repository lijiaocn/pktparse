/*
 * =====================================================================================
 *
 *       Filename:  parsepkt.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/29/2014 09:34:54 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "public/parsepkt.h"

#include "link/parselink.h"
#include "link/ethernet.h"
#include "link/device.h"

#include "net/parsenet.h"
#include "net/host.h"
#include "net/ipv4.h"

#include "tran/parsetran.h"
#include "tran/port.h"
#include "tran/tcp.h"
#include "tran/udp.h"
#include "tran/tcpstat.h"
#include "tran/udpstat.h"

#include <string.h>

int pktparse_init(ParseContext *context)
{
	memset(context, 0, sizeof(ParseContext));

	context->devs = g_hash_table_new_full(device_hash, device_equal, NULL, device_free);
	if(context->devs == NULL){
		return -1;
	}

	context->udpstats = g_hash_table_new_full(udp_stat_hash, udp_stat_equal, NULL, udp_stat_free);
	if(context->devs == NULL){
		return -1;
	}

	context->tcpstats = g_hash_table_new_full(tcp_stat_hash, tcp_stat_equal, NULL, tcp_stat_free);
	if(context->devs == NULL){
		return -1;
	}
}

void pktparse_free(ParseContext *context)
{
	g_hash_table_destroy(context->devs);
	g_hash_table_destroy(context->udpstats);
	g_hash_table_destroy(context->tcpstats);
}

void pktparse_pkt(ParseContext *context, PktStat *pktstat)
{
	DealStat dealstat;

	// now, pktstat just has:
	//    pktstat->hdr  pktstat->data
	if(context->before_link != NULL){
		dealstat = context->before_link(context, pktstat);
		if(dealstat == EU_STOP){
			return;
		}
	}

	dealstat = parse_link(context, pktstat);

	if(context->record_link != NULL){
		context->record_link(context, pktstat);
	}

	if(dealstat == EU_STOP){
		return;
	}

	// parse linkup protocol
	if(pktstat->info.link.uptype != EU_LINKUP_INVALID){

		if(context->before_linkup != NULL){
			dealstat = context->before_linkup(context, pktstat);
			if(dealstat == EU_STOP){
				return;
			}
		}

		dealstat = parse_linkup(context, pktstat);

		if(context->record_linkup != NULL){
			context->record_linkup(context, pktstat);
		}

		if(dealstat == EU_STOP){
			return;
		}

	}

	// now, pktstat has:
	//    pktstat->hdr  pktstat->data 
	//    pktstat->owner.devpair pktstat->info.link pktstat->info.net
	if(context->before_net != NULL){
		dealstat = context->before_net(context, pktstat);
		if(dealstat == EU_STOP){
			return;
		}
	}

	dealstat = parse_net(context, pktstat);

	if(context->record_net != NULL){
		context->record_net(context, pktstat);
	}

	if(dealstat == EU_STOP){
		return;
	}

	//now, pktstat has:
	//   pktstat->hdr  pktstat->data 
	//   pktstat->owner.devpair pktstat->info.link pktstat->info.net
	//   pktstat->owner.hostpair pktstat->info.tran pktstat->info.tran_as
	if(context->before_tran != NULL){
		dealstat = context->before_tran(context, pktstat);
		if(dealstat == EU_STOP){
			return;
		}
	}

	dealstat = parse_tran(context, pktstat);

	if(context->record_tran != NULL){
		context->record_tran(context, pktstat);
	}

	if(dealstat == EU_STOP){
		return;
	}

#if 0
	//now, pktstat has:
	//   pktstat->hdr  pktstat->data 
	//   pktstat->owner.devpair pktstat->info.link pktstat->info.net
	//   pktstat->owner.hostpair pktstat->info.tran pktstat->info.tran_as
	//   pktstat->owner.portpair pktstat->info.app pktstat->info.app_as
	dealstat = context->before_app(context, pktstat);
	if(dealstat == EU_STOP){
		return;
	}

	//TODO parse app
#endif
}
