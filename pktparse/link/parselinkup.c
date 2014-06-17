/*
 * =====================================================================================
 *
 *       Filename:  parselink.c
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
#include "link/parselink.h"
#include "link/ethernet.h"

DealStat parse_linkup(ParseContext *context, PktStat *pktstat)
{
	DealStat stat;

	switch(pktstat->info.link.uptype)   //! need parse link up protocol
	{
		case EU_LINKUP_INVALID:
			goto LINKUP_CONTINUE;
		case EU_LINKUP_ARP:
			//TODO parse arp
			goto LINKUP_STOP;
		case EU_LINKUP_RARP:
			//TODO parse rarp
			goto LINKUP_STOP;
		case EU_LINKUP_MPLS:
			//TODO parse mpls
			goto LINKUP_STOP;
		case EU_LINKUP_P8021Q:
			//TODO parse p8021Q
			goto LINKUP_STOP;
		case EU_LINKUP_PPPOED:
			//TODO parse pppoed
			goto LINKUP_STOP;
		case EU_LINKUP_PPPOES:
			//TODO parse pppoes
			goto LINKUP_STOP;
		case EU_LINKUP_MAX:
			//TODO log
			goto LINKUP_STOP;
		default:
			goto LINKUP_STOP;
	}

LINKUP_CONTINUE:

	return EU_CONTINUE;

LINKUP_STOP:

	return EU_STOP;
}
