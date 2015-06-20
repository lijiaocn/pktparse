#include "arp_test.h"
#include "pktparse.h"
#include "ljac.h"
#include <stdio.h>
#include <string.h>

u_char arp_pkt1[]={ 
	/* dmac */ 0xff,0xff,0xff,0xff,0xff,0xff,
	/* smad */ 0x00,0x25,0x22,0xe0,0x24,0x94,
	/* type */ 0x08,0x06,
	/* arp content */
		/* hardware type */ 0x00,0x01,
		/* protocol type */ 0x08,0x00,
		/* hardware size */ 0x06,
		/* protocol size */ 0x04,
		/* opcode */ 0x00,0x01,
		/* sender mac */0x00,0x25,0x22,0xe0,0x24,0x94,
		/* sender ip */ 0xac,0x13,0x00,0x79,
		/* target mac */0x00,0x00,0x00,0x00,0x00,0x00,
		/* target ip*/0xac,0x13,0x00,0x01,
	/* pading */0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	};

DealStat arp_before_link(ParseContext *context, PktStat *pktstat)
{
	printf("[before_link]\n");

	printf("pkt time: %u/sec %u/usec\n",pktstat->hdr.sec, pktstat->hdr.usec);
	lja_print_X("pkt data:\t","\n"," ",pktstat->data, pktstat->hdr.caplen);

	return EU_CONTINUE;
}

DealStat arp_before_linkup(ParseContext *context, PktStat *pktstat)
{
	LinkInfo *link = &(pktstat->info.link);
	printf("[before_linkup]\n");

	printf("link type: %s\n", get_link_name(link->type));
	printf("link size: %d\n", link->size);
	lja_print_X("link data:\t","\n"," ",link->data, link->size);

	printf("link uptype: %s\n", get_linkup_name(link->uptype));
	printf("link upsize: %d\n", link->upsize);
	lja_print_X("link updata:\t","\n"," ",link->updata, link->upsize);

	return EU_CONTINUE;
}

void arp_record_link(ParseContext *context, PktStat *pktstat)
{
	LinkInfo *link = &(pktstat->info.link);
	printf("[record_link]\n");
	printf("an %s pkt\n", get_link_name(link->type));
}

void arp_record_linkup(ParseContext *context, PktStat *pktstat)
{
	LinkInfo *link = &(pktstat->info.link);
	printf("[record_linkup]\n");
	printf("an %s pkt\n", get_linkup_name(link->uptype));
}

int arp_test(void)
{

	ParseContext context;
	PktStat pktstat;

	int ret;

	ret = pktparse_init(&context);
	if(ret == -1){
		fprintf(stderr, "pktparse_init failed\n");
		return -1;
	}

	context.before_link = arp_before_link;
	context.record_link = arp_record_link;

	context.before_linkup = arp_before_linkup;
	context.record_linkup = arp_record_linkup;

	memset(&pktstat, 0, sizeof(pktstat));
	pktstat.hdr.sec = 1;
	pktstat.hdr.usec = 2;
	pktstat.hdr.caplen = sizeof(arp_pkt1);
	pktstat.hdr.len = pktstat.hdr.caplen;
	pktstat.data = arp_pkt1;

	pktparse_pkt(&context, &pktstat);

	//TODO check dev
	
	printf("[sys stat]\n");
	display_devs(&context);

	pktparse_free(&context);

	return 0;
}
