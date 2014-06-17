/*
 * =====================================================================================
 *
 *       Filename:  context.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/04/2014 01:58:55 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _CONTEXT_H__
#define _CONTEXT_H__

#include "pktstat.h"
#include "glib.h"

typedef enum{
	EU_STOP,
	EU_CONTINUE,
	EU_ERROR,
}DealStat;

typedef struct _ParseContext{
	DealStat (*before_link)(struct _ParseContext *context, PktStat *pktstat);   //!链路层解析前调用
	void (*record_link)(struct _ParseContext *context, PktStat *pktstat);       //!链路层解析后调用

	DealStat (*before_linkup)(struct _ParseContext *context, PktStat *pktstat); //!链路层上解析前调用
	void (*record_linkup)(struct _ParseContext *context, PktStat *pktstat);     //!链路层上解析后调用

	DealStat (*before_net)(struct _ParseContext *context, PktStat *pktstat);    //!网络层解析前调用
	void (*record_net)(struct _ParseContext *context, PktStat *pktstat);        //!网络层解析后调用

	DealStat (*before_tran)(struct _ParseContext *context, PktStat *pktstat);   //!传输层解析前调用
	void (*record_tran)(struct _ParseContext *context, PktStat *pktstat);       //!传输层解析后调用

	DealStat (*before_app)(struct _ParseContext *context, PktStat *pktstat);    //!应用层解析前调用
	void (*record_app)(struct _ParseContext *context, PktStat *pktstat);        //!应用层解析后调用

	GHashTable *devs;     //!依次可以查找到dev,host,port
	GHashTable *udpstats; //!使用PortPair查找
	GHashTable *tcpstats; //!使用PortPair查找

	//LinkUpProto protocol
	u_int32 arp:1;       //!是否进行arp解析
	u_int32 rarp:1;      //!是否进行rarp解析
	u_int32 mpls:1;      //!是否进行mpls解析
	u_int32 p8021q:1;    //!是否进行8021q解析
	u_int32 pppoed:1;    //!是否进行pppoed解析    
	u_int32 pppoes:1;    //!是否进行pppoes解析

	//TODO: NetProto
	//TODO: TranProto
	//TODO: AppProto
	//TODO: Service, 基与应用层协议的网路服务，例如基于http协议的邮件、通信等，具体到服务提供商
}ParseContext;

#endif
