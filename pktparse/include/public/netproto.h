/*
 * =====================================================================================
 *
 *       Filename:  protocol.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/22/2014 08:31:45 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "arch.h"

/**
 * @brief 链路层协议类型
 *
 */
typedef enum{
	EU_LINK_INVALID,
	EU_LINK_RFC894,   //!Ethernet rfc894
	EU_LINK_IEEE802,  //!IEEE802
	EU_LINK_MAX
}LinkProto;

/**
 * @brief 2.5层协议, 链路层上的非网络层协议
 *
 */
typedef enum{
	EU_LINKUP_INVALID,
	EU_LINKUP_ARP  ,    //!ARP     0x0806
	EU_LINKUP_RARP ,    //!RARP    0x8035
	EU_LINKUP_MPLS ,    //!MPLS    0x8847
	EU_LINKUP_P8021Q,   //!802.1Q  0x8100
	EU_LINKUP_PPPOED,   //!PPPoE   0x8863 - Discovery
	EU_LINKUP_PPPOES,   //!PPPoE   0x8864 - Session
	EU_LINKUP_MAX
}LinkUpProto;

/**
 * @brief 网络层协议类型
 */
typedef enum{
	EU_NET_INVALID,
	EU_NET_IPV4 ,  //!IPV4    0x0800
	EU_NET_IPV6 ,  //!IPV6    0x86DD
	EU_NET_MAX
}NetProto;

/**
 * @brief 传输层协议类型
 */
typedef enum{
	EU_TRAN_INVALID,
	EU_TRAN_TCP ,  //!TCP  0x06
	EU_TRAN_UDP ,  //!UDP  0x11
	EU_TRAN_MAX
}TranProto;

/**
 * @brief 应用层协议类型
 */
typedef enum{
	EU_APP_INVALID,
	EU_APP_HTTP ,        //!HTTP
	EU_APP_UNKNOWN,  //!未知的应用层协议
	EU_APP_MAX
}AppProto;

/**
 * @brief 数据链路层头
 */
typedef struct{
	u_char   dmac[6];  //!目的MAC
	u_char   smac[6];  //!源 MAC
	u_int16  type;     //!对于以太网是网络层类型,对于IEE802.2/IEE802.3是长度
}LinkHdr;

/**
 * @brief IEEE802.2和802.3的地址头
 */
typedef struct{
	u_char   dmac[6];  //!目的MAC
	u_char   smac[6];  //!源 MAC
	u_int16  len;      //!对于以太网是网络层类型,对于IEE802.2/IEE802.3是长度
	u_char   dsap;     //!目的服务访问点(Destination Service Access Point)
	u_char   ssap;     //!源服务访问点(Source Service Access Point)
	u_char   ctrl;     //!ctrl
	u_char   org[3];   //!org code
	u_int16  type;     //!网络层类型
}Ieee_8022_8023_Hdr;

/**
 * @brief IPv4报文头
 */
typedef struct{
#if ARCH_BIG_ENDIAN
	u_int8 ver:4; //!版本号
	u_int8 ihl:4; //!头部长度，计数单位为4byte，不得低于20byte
#elif ARCH_LITTLE_ENDIAN
	u_int8 ihl:4;    //!IP头部长度，计数单位为4byte，不得低于20byte
	u_int8 ver:4;    //!版本号
#endif

	u_int8  tos;      //!服务类型（type of service)
	u_int16 tot_len;  //!IP报文长度,计数单位为1byte
	u_int16 id;       //!IP报文id(identification)
	u_int16 frag_off; //!分片的偏移，以4byte为单位标志位，前三位是标志位[保留] [是否分片] [是否最后一个分片]
	u_int8  ttl;      //!ttl
	u_int8  procol;   //!传输层协议
	u_int16 check;    //!ip头部校验和
	u_int32 saddr;    //!源地址
	u_int32 daddr;    //!目的地址
	u_char option[0]; //!Options
}Ipv4Hdr;

/**
 * @brief TCP报头
 */
typedef struct{
	u_int16 source;   //!源端口
	u_int16 dest;     //!目的端口
	u_int32 seq;      //!TCP报文中第一个序列号
	u_int32 ack_seq;  //!确认序列号,ack_seq之前的数据已经被接收
#if ARCH_BIG_ENDIAN
	u_int16 doff:4;   //!TCP报头长度，计数单位是4byte，也是TCP数据部分相对报头开始出的偏移
	u_int16 res1:4;   //!保留标志位
	u_int16 cwr:1;    //!通知接收端已经收到了设置ECE标志的ACK
	u_int16 ece:1;    //!
	u_int16 urg:1;    //!是否有紧急指针
	u_int16 ack:1;    //!是否确认序列号
	u_int16 psh:1;    //!是否直接推送到应用程序
	u_int16 rst:1;    //!是否重建连接
	u_int16 syn:1;    //!建立连接
	u_int16 fin:1;    //!终止连接
#elif ARCH_LITTLE_ENDIAN
	u_int16 res1:4;   //!保留标志位
	u_int16 doff:4;   //!TCP报头长度，计数单位是4byte，也是TCP数据部分相对报头开始出的偏移
	u_int16 fin:1;    //!终止连接
	u_int16 syn:1;    //!建立连接
	u_int16 rst:1;    //!是否重建连接
	u_int16 psh:1;    //!是否直接推送到应用程序
	u_int16 ack:1;    //!是否确认序列号
	u_int16 urg:1;    //!是否有紧急指针
	u_int16 ece:1;    //!
	u_int16 cwr:1;    //!通知接收端已经收到了设置ECE标志的ACK
#endif
	u_int16 window;   //!发送方的窗口大小,8bit为单位
	u_int16 check;    //!校验码
	u_int16 urg_ptr;  //!紧急指针,紧临着紧急数据的第一个非紧急数据相对于序列号的正偏移
	u_char option[0]; //!options
}TcpHdr;

/**
 * @brief UDP报头
 */
typedef struct{
	u_int16 source;   //!源端口
	u_int16 dest;     //!目的端口
	u_int16 len;      //!整个upd报文的长度
	u_int16 check;    //!校验码
}UdpHdr;

#endif
