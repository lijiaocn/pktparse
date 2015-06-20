#include "net/parsenet.h"
#include "net/ipv4.h"

DealStat parse_net(ParseContext *context, PktStat *pktstat)
{
	NetInfo *netinfo =  &(pktstat->info.net);
	TranInfo *traninfo = &(pktstat->info.tran);
	TranInfo *traninfo_as = &(pktstat->info.tran_as);

	switch (netinfo->type)
	{
		case EU_NET_INVALID:
			return EU_STOP;
		case EU_NET_IPV4 :  //!IPV4    0x0800
			pktstat->owner.hostpair = parse_ipv4(pktstat->owner.devpair, 
					netinfo->size, netinfo->data, traninfo, traninfo_as);
			return EU_CONTINUE;
		case EU_NET_IPV6 :  //!IPV6    0x86DD
			return EU_STOP;  
		case EU_NET_MAX: 
			return EU_STOP;
		default :
			return EU_STOP;
	}
}
