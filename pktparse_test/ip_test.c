#include "ip_test.h"
#include "pktparse.h"
#include "ljac.h"
#include <stdio.h>
#include <string.h>

u_char ip_pkt1[]={ 
	/* dmac */ 0x01,0x00,0x5e,0x02,0x0b,0x47,
	/* smad */ 0x08,0x00,0x27,0xd6,0x38,0xe8,
	/* type */ 0x08,0x00,
	/* ip content */
		/* version&len */  0x45,
		/* service */      0x00,
		/* tot len */      0x00,0x6c,
		/* identify */     0x00,0x00,
		/* flags &offset*/ 0x40,0x00,
		/* ttl */          0x01,
		/* protocol */     0x11,
		/* header checksum */ 0xf4,0xeb,
		/* udp content */
			/* sip */      0xac,0x13,0xde,0x38,
			/* dip */      0xef,0x02,0x0b,0x47,
			/* sport */    0x90,0x30,
			/* dport */    0x21,0xc9,
			/* length*/    0x00,0x58,
			/* checksum */ 0x15,0x61,
			/* data */     0x00,0x00,0x00,0x85,0x00,0x00,
			               0x00,0x09,0x6e,0x65,0x74,0x75,0x74,0x65, 0x6c,0x30,0x36,0x00,0x00,0x00,0x00,0x00,
			               0x00,0x1b,0x6a,0x76,0x6d,0x2e,0x6d,0x65, 0x74,0x72,0x69,0x63,0x73,0x2e,0x6d,0x65,
						   0x6d,0x4e,0x6f,0x6e,0x48,0x65,0x61,0x70, 0x55,0x73,0x65,0x64,0x4d,0x00,0x00,0x00,
						   0x00,0x00,0x00,0x00,0x00,0x02,0x25,0x73, 0x00,0x00,0x00,0x00,0x00,0x09,0x31,0x37,
						   0x2e,0x33,0x34,0x34,0x36,0x38,0x38,0x00, 0x00,0x00
	};

DealStat ip_before_link(ParseContext *context, PktStat *pktstat)
{
	printf("[before_link]\n");

	printf("pkt time: %u/sec %u/usec\n",pktstat->hdr.sec, pktstat->hdr.usec);
	lja_print_X("pkt data:\t","\n"," ",pktstat->data, pktstat->hdr.caplen);

	return EU_CONTINUE;
}

DealStat ip_before_linkup(ParseContext *context, PktStat *pktstat)
{
	LinkInfo *link = &(pktstat->info.link);
	printf("[before_linkup]\n");

	printf("link type: %s\n", get_link_name(link->type));
	printf("link size: %u\n", link->size);
	lja_print_X("link data:\t", "\n", " ", link->data, link->size);

	printf("link uptype: %s\n", get_linkup_name(link->uptype));
	printf("link upsize: %u\n", link->upsize);
	lja_print_X("link updata:\t", "\n", " ", link->updata, link->upsize);

	return EU_CONTINUE;
}

DealStat ip_before_net(ParseContext *context, PktStat *pktstat)
{
	NetInfo *net = &(pktstat->info.net);
	printf("[before_net]\n");

	printf("net type: %s\n", get_net_name(net->type));
	printf("net size: %u\n", net->size);
	lja_print_X("net data:\t", "\n", " ", net->data, net->size);

	return EU_CONTINUE;
}

DealStat ip_before_tran(ParseContext *context, PktStat *pktstat)
{
	TranInfo *tran = &(pktstat->info.tran);
	printf("[before_tran]\n");

	printf("tran type: %s\n", get_tran_name(tran->type));
	printf("tran size: %u\n", tran->size);
	lja_print_X("tran data:\t", "\n", " ", tran->data, tran->size);

	return EU_CONTINUE;
}

void ip_record_link(ParseContext *context, PktStat *pktstat)
{
	LinkInfo *link = &(pktstat->info.link);
	printf("[record_link]\n");
	printf("an %s pkt\n", get_link_name(link->type));
}

void ip_record_linkup(ParseContext *context, PktStat *pktstat)
{
	LinkInfo *link = &(pktstat->info.link);
	printf("[record_linkup]\n");
	printf("an %s pkt\n", get_linkup_name(link->uptype));
}

void ip_record_net(ParseContext *context, PktStat *pktstat)
{
	NetInfo *net = &(pktstat->info.net);
	printf("[record_net]\n");
	printf("an %s pkt\n", get_net_name(net->type));
}

void ip_record_tran(ParseContext *context, PktStat *pktstat)
{
	TranInfo *tran = &(pktstat->info.tran);
	printf("[record_tran]\n");
	printf("an %s pkt\n", get_tran_name(tran->type));
}

int ip_test(void)
{

	ParseContext context;
	PktStat pktstat;

	int ret;

	ret = pktparse_init(&context);
	if(ret == -1){
		fprintf(stderr, "pktparse_init failed\n");
		return -1;
	}

	context.before_link = ip_before_link;
	context.record_link = ip_record_link;

	context.before_linkup = ip_before_linkup;
	context.record_linkup = ip_record_linkup;

	context.before_net = ip_before_net;
	context.record_net = ip_record_net;

	context.before_tran = ip_before_tran;
	context.record_tran = ip_record_tran;

	memset(&pktstat, 0, sizeof(pktstat));
	pktstat.hdr.sec = 1;
	pktstat.hdr.usec = 2;
	pktstat.hdr.caplen = sizeof(ip_pkt1);
	pktstat.hdr.len = pktstat.hdr.caplen;
	pktstat.data = ip_pkt1;

	pktparse_pkt(&context, &pktstat);

	//TODO check dev
	
	printf("[sys stat]\n");
	display_devs(&context);

	pktparse_free(&context);

	return 0;
}
