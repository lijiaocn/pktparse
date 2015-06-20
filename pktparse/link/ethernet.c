/*
 * =====================================================================================
 *
 *       Filename:  parse_link.c
 *
 *    Description:  链路层协议解析
 *
 *        Version:  1.0
 *        Created:  05/22/2014 09:08:33 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <assert.h>
#include "link/ethernet.h"
#include "link/device.h"

/**
 * @brief 解析链路层的IEEE802.2/802.3
 *
 * @param size  IEEE802.2/802.3中的长度字段指定的长度
 * @param data  IEEE802.2/802.3数据包开始位置(长度字段以后)
 * @param link  链路层信息
 * @param net  从数据包的链路层中解析出的网络层信息
 *
 * 测试用例: 
 *   ieee802.2/802.3封装的IPV4 ARP RARP IPV6 MPLS PPPOE(discovery and Session) 802.1Q 
 */
void __parse_ieee_8022_8023(u_int16 size, u_char *data, LinkInfo *link, NetInfo *net/**<[out] 解析出报文的网络层信息*/)
{
	assert(size != 0);
	assert(data != NULL);
	assert(link != NULL);
	assert(net != NULL);

	link->updata = data+8;  //default position: add dsap(1)+ssap(1)+cntl(3)+oprcode(3)
	link->upsize = size-8;  //default size: 

	net->data = link->updata;    //默认没有2.5层协议, 直接得到网络层数据
	net->size = link->upsize;

	Ieee_8022_8023_Hdr *hdr=(Ieee_8022_8023_Hdr *)data;

	switch (htons(hdr->type)){
		case 0x0800 :  //IPv4
			net->type = EU_NET_IPV4;
			break;
		case 0x0806 :  //ARP
			link->uptype = EU_LINKUP_ARP;
			break;
		case 0x8035 :  //RAPP
			link->uptype = EU_LINKUP_RARP;
			break;
		case 0x86DD :  //IPv6
			net->type = EU_NET_IPV6;
			break;
		case 0x8847 :  //MPLS Label
			link->uptype = EU_LINKUP_MPLS;
			break;
		case 0x8863 :  //PPPoe - Discovery
			link->uptype = EU_LINKUP_PPPOED;
			break;
		case 0x8864 :  //PPPoe - Session
			link->uptype = EU_LINKUP_PPPOES;
			break;
		case 0x8100 :  //802.1Q tag
			link->uptype = EU_LINKUP_P8021Q;
			break;
		default :      //IEEE802.2/802.3 rfc1084
			;
	}
	return ;
}

/**
 * @brief 解析链路层数据包。
 *
 * @param devs  按照MAC地址区分的设备列表
 * @param size  数据包大小
 * @param data  数据包开始位置
 * @param link  解析出的链路层信息, 如果2.5层协议无效，表示没有2.5层数据, 
 *                如果2.5层协议有效，需要从2.5层协议中解析出网络层信息
 * @param net  没有2.5层协议且有网络层数据时，解出的网络层信息
 *
 * @return DevicePair 发送/接收设备 
 *
 * 测试用例: 
 *   以太网协议封装的IPV4 ARP RARP IPV6 MPLS PPPOE(discovery and Session) 802.1Q 
 *   ieee802.2/802.3封装的IPV4 ARP RARP IPV6 MPLS PPPOE(discovery and Session) 802.1Q 
 *
 */
DevicePair parse_ethernet(GHashTable *devs, u_int16 size, u_char *data, 
		LinkInfo *link/**<[out] 解析出的链路层信息*/, NetInfo *net/**<[out] 解析出报文的网络层信息*/)
{
	assert(data != NULL);
	assert(link != NULL);
	assert(net != NULL);

	LinkHdr *hdr = (LinkHdr*)data;
	u_char* ret = NULL;

	link->type = EU_LINK_RFC894;     //默认值
	link->size = size;
	link->data = data;   

	link->uptype = EU_LINKUP_INVALID;
	link->upsize = size-18;  //default size: -(dmac(6)+smac(6)+type(2)+crc(4))
	link->updata = data+14;  //default position: dmac(6)+smac(6)+type(2)

	net->type = EU_NET_INVALID;   
	net->size = link->upsize;  //默认没有2.5层协议, 直接网络层
	net->data = link->updata;

#ifdef DEBUG
	display_mac(hdr->dmac);
	printf(" <- ");
	printf("Src MAC: ");
	display_mac(hdr->smac);
#endif 

	switch (ntohs(hdr->type))
	{
		case 0x0800 :  //IPv4
			net->type = EU_NET_IPV4;
			break;
		case 0x0806 :  //ARP
			link->uptype = EU_LINKUP_ARP;
			break;
		case 0x8035 :  //RAPP
			link->uptype = EU_LINKUP_RARP;
			break;
		case 0x86DD :  //IPv6
			net->type = EU_NET_IPV6;
			break;
		case 0x8847 :  //MPLS Label
			link->uptype = EU_LINKUP_MPLS;
			break;
		case 0x8863 :  //PPPoe - Discovery
			link->uptype = EU_LINKUP_PPPOED;
			break;
		case 0x8864 :  //PPPoe - Session
			link->uptype = EU_LINKUP_PPPOES;
			break;
		case 0x8100 :  //802.1Q tag
			link->uptype = EU_LINKUP_P8021Q;
			break;
		default :      //IEEE802.2/802.3 rfc1084, 如果不是Ethernet的类型，就是IEEE802.2标准
			link->type = EU_LINK_IEEE802;
			__parse_ieee_8022_8023(ntohs(hdr->type) ,link->updata, link, net);
			break;
	}

	DevicePair  devpair;
	
	devpair.ddev = g_hash_table_lookup(devs, hdr->dmac);
	if(devpair.ddev == NULL){
		devpair.ddev = device_new(hdr->dmac);
		//TODO  if failed
		g_hash_table_insert(devs, devpair.ddev->mac, devpair.ddev);
	}

	devpair.sdev = g_hash_table_lookup(devs, hdr->smac);
	if(devpair.sdev == NULL){
		devpair.sdev = device_new(hdr->smac);
		//TODO  if failed
		g_hash_table_insert(devs, devpair.sdev->mac, devpair.sdev);
	}

	return devpair;
}

