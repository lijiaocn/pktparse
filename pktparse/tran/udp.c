/*
 * =====================================================================================
 *
 *       Filename:  parse_tran.c
 *
 *    Description:  解析传输层协议
 *
 *        Version:  1.0
 *        Created:  05/22/2014 09:11:09 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "tran/udp.h"
#include "tran/port.h"
#include <assert.h>

/**
 * @brief 解析传输层udp数据
 *
 * @param hostpair  接收/发送主机
 * @param size 传输层udp数据包的大小 
 * @param data 传输层udp数据包的开始位置
 * @param info 从传输层udp数据包中解析出的应用层信息
 * @param info_as 重组后的应用层信息
 *
 * @return PortPair  接收/发送端口
 */
PortPair parse_udp(ParseContext *context, HostPair hostpair, u_int16 size, 
		u_char *data, AppInfo *info, AppInfo *info_as)
{
	assert(size != 0);
	assert(data != NULL);
	assert(info != NULL);

	UdpHdr *hdr=(UdpHdr*)data;

	info->type = EU_APP_INVALID;
	info->data = data + 8;
	info->size = hdr->len;

	//TODO: find port2 from hostpair

	/*
		TODO: 获取应用层的协议类型
		方案1: 跟据端口判断
		方案2: 扫描报文内容
		方案3: 建立状态机
	*/

	PortPair portpair;
	portpair.dport = NULL;
	portpair.sport = NULL;

	if(hostpair.proto == EU_NET_IPV4){
		Ip4Host *dhost = (Ip4Host *)(hostpair.dhost);
		portpair.dport = g_tree_lookup(dhost->udpports, &(hdr->dest));
		if(portpair.dport == NULL){
			TcpPort *port = tcp_port_new(hdr->dest);
			g_tree_insert(dhost->udpports, &(port->port), port);
			dhost->portnum++;
		}

		Ip4Host *shost = (Ip4Host *)(hostpair.shost);
		portpair.sport = g_tree_lookup(shost->udpports, &(hdr->source));
		if(portpair.sport == NULL){
			TcpPort *port = tcp_port_new(hdr->source);
			g_tree_insert(shost->udpports, &(port->port), port);
			shost->portnum++;
		}
	}


	if(hostpair.proto == EU_NET_IPV6){
		//TODO
	}

	//TODO: find udpstat from context->udpstats
	//udp没有重组的概念, info_as中不填写内容
	
	return ;
}
