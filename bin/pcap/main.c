/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/29/2014 09:13:07 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

/**
 * @brief 打印设备列表
 *
 * @param devs pcap_if_t
 * @return 设备的数目
 */
int display_devs(pcap_if_t *devs)
{
	assert(devs != NULL);
	pcap_if_t *d;
	
	int i=0;
	for( d=devs ; d != NULL ; d=d->next )
	{
		printf("%d. %s",++i, d->name);
		if(NULL != d->description){
			printf(" (%s)\n", d->description);
		}else{
			printf(" (No description)\n");
		}
	}
	return i;
}

/**
 * @brief 打印一个设备信息
 *
 * @param dev 
 *
 */
void  display_dev(pcap_if_t *dev)
{
	assert(dev != NULL);

	printf("%s",dev->name);
	if(NULL != dev->description){
		printf(" (%s)\n", dev->description);
	}else{
		printf(" (No description)\n");
	}
	return ;
}

/**
 * @brief 打印pcap报文头
 *
 * @param hdr
 */
void display_pcap_pkthdr(struct pcap_pkthdr *hdr)
{
	assert(hdr != NULL);
	printf("%ld.%ld: %u of %u",hdr->ts.tv_sec,hdr->ts.tv_usec,hdr->caplen,hdr->len);
	return ;
}

/**
 * @brief 打印pcap捕获的数据包的内容
 *
 * @param hdr 捕获的包的pcap头
 * @param data 捕获的报文数据
 */
void display_pcap_data(struct pcap_pkthdr *hdr, const u_char *data)
{
	assert(hdr != NULL);
	assert(data != NULL);
	int i;
	for (i= 0; i < hdr->caplen; i++)
	{
		printf("%.2x",data[i]);
	}
	return ;
}


int main(int argc, char *argv[])
{
	pcap_if_t *devs;
	pcap_if_t *dev;
	pcap_t *handle;
	struct pcap_pkthdr *pcaphdr;
	u_char *pktdata;
	int count=5;

	int devnum;
	int chose = -1;
	int i=0;
	char errbuf[PCAP_ERRBUF_SIZE];
	int re;

	if(-1==pcap_findalldevs(&devs,errbuf)){
		fprintf(stderr,"Error %s %d\n",__FILE__,__LINE__);
	}
	devnum = display_devs(devs);

	printf("Chose a dev[0-%d]:",devnum);
	scanf("%d",&chose);
	while(chose < 1 || chose > devnum){
		printf("Chose a dev[1-%d]:",devnum);
		scanf("%d",&chose);
	}

	dev=devs;
	for(i=1; i<chose; i++)
	{
		dev=dev->next;
	}
	
	display_dev(dev);

	handle=pcap_open_live(dev->name,65535,1,1000,errbuf);
	if(handle == NULL)
	{
		fprintf(stderr,"Failed pcap_open_live %s:%d\n",__FILE__,__LINE__);
		goto EXIT;
	}
	
	for(i=0; i<count; i++)
	{
		re=pcap_next_ex(handle,&pcaphdr,(const u_char**)&pktdata);
		switch (re)
		{
			case 1 :
				printf("\n========== %d [NORMAL]  ==========\n",i);
				break;
			case 0 :
				printf("\n========== %d [EXPIRED] ==========\n",i);
				break;
			case -1:
				printf("\n========== %d [ERROR]   ==========\n",i);
				printf("%s\n",pcap_geterr(handle));
				continue;
				break;
			default :
				fprintf(stderr,"Failed pcap_next_ex unknown value %s:%d\n",__FILE__,__LINE__);
				goto EXIT;
				break;
		}

		if(pcaphdr != NULL && pktdata != NULL)
		{
			display_pcap_pkthdr(pcaphdr);
		}else if(pcaphdr == NULL)
		{
			printf("pcaphdr is NULL!\n");
		}else
		{
			printf("pktdata is NULL!\n");
		}
	}

EXIT:
	pcap_freealldevs(devs);
	return 0;
}
