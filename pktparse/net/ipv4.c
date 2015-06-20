/*
 * =====================================================================================
 *
 *       Filename:  parse_net.c
 *
 *    Description:  解析网络层协议
 *
 *        Version:  1.0
 *        Created:  05/22/2014 09:10:29 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "net/ipv4.h"
#include "net/host.h"
#include <assert.h>

/**
 * @brief 解析网络层ipv4数据
 *
 * @param devpair 接收/发送设备
 * @param size 网络层ipv4数据包的大小
 * @param data 网络层ipv4数据包的开始位置
 * @param info 从网络层ipv4数据包中解析出的传输层信息
 * @param info_as 分片重组后的传输层信息
 *
 * @return HostPair 接收/发送主机
 */
HostPair parse_ipv4(DevicePair devpair, u_int16 size, u_char *data, 
		TranInfo *info/**<[out] 解析出报文的传输层信息*/,
		TranInfo *info_as /**<[out] 分片重组后的传输层信息 */)
{
	assert(size != 0);
	assert(data != NULL);
	assert(info != NULL);

	Ipv4Hdr *hdr=(Ipv4Hdr *)data;

	info->data = data + (hdr->ihl)*4;
	info->size = ntohs(hdr->tot_len) - (hdr->ihl)*4; //减去头部的大小

	switch(hdr->procol){
		case 0x06:   //TCP
			info->type = EU_TRAN_TCP;
		case 0x11:   //UDP
			info->type = EU_TRAN_UDP;
		default:
			;
	}
	
	//TODO: IP分片重组
	//TODO: 如果是IP分片, 需要收取到所有分片包后，重组成一个新的IP包,从新的IP包中解析出info_as
	//TODO: 不同的操作系统，IP分片的重组策略不同

	HostPair hostpair;
	hostpair.proto = EU_NET_IPV4;

	hostpair.dhost = g_tree_lookup(devpair.ddev->ip4hosts, &(hdr->daddr));
	if(hostpair.dhost == NULL){
		Ip4Host *host = host_ip4_new(hdr->daddr);
		hostpair.dhost = host;
		g_tree_insert(devpair.ddev->ip4hosts, &(host->addr), host);
		//TODO if failed
	}

	hostpair.shost = g_tree_lookup(devpair.sdev->ip4hosts, &(hdr->saddr));
	if(hostpair.shost == NULL){
		Ip4Host *host = host_ip4_new(hdr->saddr);
		hostpair.shost = host;
		g_tree_insert(devpair.sdev->ip4hosts, &(host->addr), host);
		//TODO if failed
	}

	return  hostpair;
}

