/*
 * =====================================================================================
 *
 *       Filename:  parsetran.c
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

#include "tran/parsetran.h"
#include "tran/tcp.h"
#include "tran/udp.h"

DealStat parse_tran(ParseContext *context, PktStat *pktstat)
{
	TranInfo *traninfo = &(pktstat->info.tran);
	TranInfo *traninfo_as = &(pktstat->info.tran_as);

	AppInfo *appinfo = &(pktstat->info.app);
	AppInfo *appinfo_as = &(pktstat->info.app_as);

	switch (traninfo->type)
	{
		case EU_TRAN_TCP :
			pktstat->owner.portpair = parse_tcp(context, pktstat->owner.hostpair, 
					traninfo->size, traninfo->data, appinfo, appinfo_as);
			goto CONTINUE;
		case EU_TRAN_UDP :
			pktstat->owner.portpair = parse_udp(context, pktstat->owner.hostpair, 
					traninfo->size, traninfo->data, appinfo, appinfo_as);
			goto CONTINUE;
		default :
			goto STOP;
	}

STOP:
	return EU_STOP;
	
CONTINUE:

	return EU_CONTINUE;
}
