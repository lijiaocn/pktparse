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

DealStat parse_link(ParseContext *context, PktStat *pktstat)
{
	pktstat->owner.devpair = parse_ethernet(context->devs, pktstat->hdr.caplen, 
			pktstat->data, &(pktstat->info.link), &(pktstat->info.net));

	return EU_CONTINUE;
}
