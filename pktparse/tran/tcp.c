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

#include "tran/tcp.h"
#include "tran/port.h"
#include <assert.h>

/**
 * @brief 解析传输层tcp数据
 *
 * @param hostpair  接收/发送主机
 * @param size 传输层tcp数据包的大小 
 * @param data 传输层tcp数据包的开始位置
 * @param info 从传输层tcp数据包中解析出的应用层信息
 * @param info_as 重组后的应用层信息
 *
 * @return PortPair  接收/发送端口
 */
PortPair parse_tcp(ParseContext *context, HostPair hostpair, u_int16 size, 
		u_char *data, AppInfo *info, AppInfo *info_as)
{
	assert(size != 0);
	assert(data != NULL);
	assert(info != NULL);

	TcpHdr *hdr = (TcpHdr*)data;


	info->type = EU_APP_INVALID;
	info->data = data + hdr->doff*4;
	info->size = size - hdr->doff*4;

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
		portpair.dport = g_tree_lookup(dhost->tcpports, &(hdr->dest));
		if(portpair.dport == NULL){
			TcpPort *port = tcp_port_new(hdr->dest);
			g_tree_insert(dhost->tcpports, &(port->port), port);
		}

		Ip4Host *shost = (Ip4Host *)(hostpair.shost);
		portpair.sport = g_tree_lookup(shost->tcpports, &(hdr->source));
		if(portpair.sport == NULL){
			TcpPort *port = tcp_port_new(hdr->source);
			g_tree_insert(shost->tcpports, &(port->port), port);
		}
	}

	if(hostpair.proto == EU_NET_IPV6){
		//TODO
	}

	//TODO: find or create  tcpstat in  context->tcpstats
	
	//TODO: 根据tcpstat中的状态进行流重组
	//在接收端进行流重组，使用流重组后的数据构造info
	//TODO: 不同的操作系统，流重组的策略不同

	return portpair;
}
