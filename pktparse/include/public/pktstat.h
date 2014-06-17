/*
 * =====================================================================================
 *
 *       Filename:  pktInfo.h
 *
 *    Description:  报文数据
 *
 *        Version:  1.0
 *        Created:  05/PairPair/Pair014 08:Pair4:13 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __PKT_INfO_H__
#define __PKT_INfO_H__

#include "netproto.h"
#include "netstat.h"

/**
 * @brief 报文的隶属数据
 * 
 */
typedef struct{
	DevicePair devpair;   //!网卡
	HostPair   hostpair;  //!主机
	PortPair   portpair;  //!端口
}PktOwner;

/**
 * @brief 从原始报文中解析出的链路层信息 
 *
 */
typedef struct{
	LinkProto type;    //!链路层协议类型
	u_int16 size;      //!链路层信息大小，实际就是报文的大小
	u_char *data;      //!链路层开始的数据,实际就是报文的开始地址

	u_char *updata;        //!Pair.5层协议开始数据
	LinkUpProto uptype; //!Pair.5层协议
	u_int16 upsize;        //!Pair.5层协议数据大小
}LinkInfo;

/**
 * @brief 从数据链路层解析出的网络层信息 
 *
 */
typedef struct{
	NetProto type;  //!网络层协议类型
	u_int16 size;     //!网络层信息大小
	u_char *data;     //!网络层开始的数据 
}NetInfo;

/**
 * @brief 从网络层解析出的传输层信息
 *
 */
typedef struct{
	u_char *data;       //!传输层开始的数据 
	TranProto type;   //!传输层协议类型
	u_int16 size;       //!传输层信息大小
}TranInfo;

/**
 * @brief 从传输层解析出的应用层信息
 *
 */
typedef struct{
	u_char *data;       //!应用层开始的数据
	TranProto type;   //!应用层协议类型
	u_int16 size;       //!应用层信息大小
}AppInfo;

/**
 * @brief 报文信息
 *
 *    信息    包含此信息的报文
 *    link        所有报文
 *    tran        非IP分片包、IP分片的第一个分片
 *    app         非IP分片包、IP分片的第一个分片, 不关心TCP中的序号
 *    tran_as     IP分片包重组完成后的的新包
 *    app_as      乱序的报文
 *
 */
typedef struct{
	LinkInfo  link;      //!报文的链路层信息, 记录type, 开始位置, 大小
	NetInfo   net;       //!报文的网络层信息, 记录type, 开始位置, 大小
	TranInfo  tran;      //!报文的传输层信息, 记录type, 开始位置, 大小
	TranInfo  tran_as;   //!分片重组后的传输层信息, 记录type, 开始位置，大小
	AppInfo   app;       //!报文的应用层信息, 记录type, 开始位置, 大小
	AppInfo   app_as;    //!流重组后的应用层信息, 记录type, 开始位置, 大小
}PktInfo;

/**
 *
 * @brief 报文头与pcap的报文头保持一致
 *
 * 该结构体的成员不能调整，这样可以在写入pcap文件时，直接写入该结构体
 *
 * Global Header: 
 * typedef struct pcap_hdr_s { 
 *      guint32 magic_number; 
 *      guint16 version_major;
 *      guint16 version_minor; 
 *      gint32 thiszone; 
 *      guint32 sigfigs; 
 *      guint32 snaplen; 
 *      guint32 network; 
 * } pcap_hdr_t;
 *
 * Packet Header:
 * typedef struct pcaprec_hdr_s { 
 *      guint32 ts_sec; 
 *      guint32 ts_usec; 
 *      guint32 incl_len; 
 *      guint32 orig_len; 
 * } pcaprec_hdr_t;
 *
 */
typedef struct{
	u_int32 sec;         //!报文时间, 秒
	u_int32 usec;        //!报文时间，毫秒
	u_int32 caplen;      //!捕获的报文长度, 例如 tcpdump -s 96 只保存前96个字节
	u_int32 len;         //!报文的真正长度
}PktHdr;

/**
 * @brief 报文状态
 *
 */
typedef struct{
	PktHdr  hdr;         //!报文头
	u_char *data;        //!捕获的报文
	PktInfo info;        //!报文各层信息
	PktOwner owner;      //!报文隶属者
}PktStat;

#endif
