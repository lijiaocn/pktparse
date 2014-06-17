/*
 * =====================================================================================
 *
 *       Filename:  netstat.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/23/2014 02:26:38 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __ENTITY_H__
#define __ENTITY_H__

/** 
 * @file entity的组织结构
 *
 *                                  +---------+
 *                                  | Devices |
 *                                  +---------+
 *                                       |
 *                   +---------------------------------------+
 *                   |                                       |
 *              +----+------+    +-------+            +------+----+    +-------+
 *              | ip4hosts  |----+ frags |            | ip6hosts  |----+ frags |
 *              +-----+-----+    +-------+            +-----------+    +-------+
 *                    |                                    |                
 *            +-------+--------+                   +-------+--------+       
 *            |                |                   |                |       
 *       +----+-----+     +----+-----+        +----+-----+     +----+-----+ 
 *       | udpports |     | tcpports |        | udpports |     | tcpports | 
 *       +-----+----+     +-----+----+        +-----+----+     +-----+----+ 
 *             |                |                   |                |
 *        +----+----+      +----+----+         +----+----+      +----+----+
 *        | udpstat |      | tcpstat |         | udpstat |      | tcpstat |
 *        +-----+---+   +--+----+----+         +---+-----+  +---+----+----+
 *              |       |       |                  |        |        |
 *              +-------+  +----+------+           +--------+  +-----+-----+
 *              |          | seqcaches |           |           | seqcaches |
 *              |          +-----------+           |           +-----------+
 *        +-----+-------+                    +-----+-------+
 *        | app handler |                    | app handler |
 *        +-------------+                    +-------------+
 *
 *
 *      注意: udpstat从 conn_point_udp中查询获取, tcpstat从conn_point_tcp中查询获取
 *
 *
 */

#include "arch.h"
#include "netproto.h"
#include "glib.h"

/**
 * @brief TCP缓存的数据片
 *
 * 如果caches的seq发生重叠，通过修改cache中的begin和offset去掉重叠的部分.
 *
 * TODO: 重叠的处理策略: 
 *      策略1: 后来的数据替换之前的数据  
 *      策略2: 去掉后来的数据中的重叠的部分 
 */
typedef struct{
	u_int32 seq;            //!有效数据的第一个字节的seq, 注意是有效, 如果发生重叠, 直接修正seq
	u_int32 valid_offset;   //!有效数据相对于data的偏移位置
	u_int32 valid_size;     //!有效数据的大小
	u_int32 data_size;      //!data指向的数据的大小, 在使用g_slice_free1释放时需要用来计算整块内存的大小
	u_char data[];          //!缓存的数据, 最后一个字节必须是0, 在分配内存是保证，多出的1个字节不计入data_size
}TcpCache;

/* 
 * @brief 应用层数据处理句柄
 *
 */
typedef struct{
	AppProto type;     //!应用层协议类型
	u_char p[0];       //!应用层句柄数据, 根据协议类型的不同, p包含不同的数据
}AppHandle;

/**
 * @brief 接收/发送端口
 *
 */
typedef struct{
	void    *sport;  //!发送端口, tcp对应TcpPort *, udp对应UdpPort *
	void    *dport;  //!接收端口, tcp对应TcpPort *, udp对应UdpPort *
}PortPair;

/** 
 * @brief Tcp端状态
 *
 * TcpStat存放在哈希表中，使用源端口的内存地址(TcpPort *)和目的端口的内存地址作为key
 *
 * tcp_cache中的内容按照seq由小到大排序, 插入时从链表尾开始寻找插入位置
 *
 */
typedef struct{
	//key
	PortPair  portpair; //!源端口结构在内存中的地址和目的端口在内存中的地址 

	//控制信息

	//统计信息
	GTimeVal start;     //!收到第一个包文的时间
	GTimeVal last;      //!收到当前报文的时间

	//状态信息
	u_int32 wait_seq;    //!期待的下一个开始的seq
	u_int16 window;      //!接收窗口
	GTimeVal wait_start; //!开始等待下一个seq数据的时间
	
	//缓存数据
	//TODO 可以根据caches中seq发现恶意的连接, 例如发送大量的不连续的seq报文
	//TODO 等待超时或者caches中数据过多时，清理caches中的内容，并记录日志
	//TODO 如果caches的seq发生重叠，通过修改cache中的begin和offset去掉重叠的部分，而不进行内存的复制拷贝
	GList  *tcp_cache;  //!收取到的后发先至的内容,如果收到的TCP报文的seq大于wait_seq,该报文的内容需要插入到cache中

	//解析处理
	AppHandle  app;     //!应用层数据处理句柄
}TcpStat;

/**
 * @brief Udp端状态
 *
 * UdpStat存放在哈希表中，使用源端口的内存地址(TcpPort *)和目的端口的内存地址作为key
 *
 */
typedef struct{
	//key
	PortPair  portpair; //!源端口结构在内存中的地址和目的端口在内存中的地址 

	//控制信息

	//统计信息
	GTimeVal start;     //!收到第一个包文的时间
	GTimeVal last;      //!收到当前报文的时间

}UdpStat;

/** 
 * @brief 端口
 *
 * TODO 可以通过端口状态的异常情况，判断端口是否被复用
 *
 * TODO 如果Host的一个端口收到了大量的连接建立，意味着什么?
 *
 *      1 该端口提供的服务有大量用户，例如web服务器的80端口、邮件服务器的端口
 *      2 该端口提供的服务遭受到攻击，例如对使用22端口的ssh服务进行密码字典攻击
 *      3 该端口遭遇到dos攻击
 */
typedef struct{
	u_int16 port;        //!端口号
	u_int32 actnum;      //!接收的活动会话数量
	u_int32 tolnum;      //!接收的会话总数
	TcpStat *stat;       //!报文所属会话的TCP端的状态, 从TcpStat的哈希表中查找到的
	AppHandle app;       //!应用层数据处理句柄
}TcpPort;

/** 
 * @brief 端口
 *
 * TODO 可以通过端口状态的异常情况，判断端口是否被复用
 * TODO 端口扫描监测
 */
typedef struct{
	u_int16 port;        //!端口号
	u_int32 actnum;      //!接收的活动会话数量
	u_int32 tolnum;      //!接收的会话总数
	UdpStat *stat;       //!报文所属会话的UDP端的状态, 从UdpStat的哈希表中查找到的
	AppHandle app;       //!应用层数据处理句柄
}UdpPort;


/**
 * @brief IP分片
 *
 * IpFrag中的data成员中存放的是收到的IP分片包的的全部负载
 *
 * 因为IP分片之间存在数据重叠的情况，需要对重叠的部分进行取舍, 因此这些负载未必是全部有效的
 *
 * 有效的负载通过validoffset和validsize从data中提取
 *
 */
typedef struct{
	u_int16 offset;        //!该分片的负载在整个完整IP包的负载中偏移, 不带flags,如果重叠,直接修正offset的值
	u_int16 valid_offset;  //!有效数据相对于data的偏移
	u_int16 valid_size;    //!有效数据的大小
	u_int16 data_size;     //!data指向的内存块的大小,在使用g_slice_free1释放时需要用来计算整块内存的大小
	u_char  data[0];       //!IP分片负载数据
}IpFrag;

/**
 * @brief IP分片列表
 */
typedef struct{
	u_int16 identi;        //!IP包标记, key
	u_int16 num;           //!已经接收的分片数量
	u_int16 size;          //!已经接收的分片的总大小
	GList *list;           //!分片列表, 按照偏移值由小到大排序，插入时从链表尾开始寻找插入位置
}IpFragsHdr;

/** 
 * @brief 通过网络地址标识的主机
 *
 * TODO 端口扫描监测
 * TODO IP分片重组策略
 */
typedef struct{
	u_int32 addr;          //!主机的ipv4地址, key
	u_int32 portnum;       //!被访问的端口的总数
	GTree *frags;          //!主机上的IP分片, 用ident区分的列表, 存放IpFragsHdr
	GTree *tcpports;       //!主机上开放的TCP端口
	GTree *udpports;       //!主机上开放的UDP端口
}Ip4Host;

/** 
 * @brief 通过网络地址标识的主机
 *
 * TODO 端口扫描监测, 常用服务端口被遍历, 例如20 21 22 25 80 110 143 443 
 * TODO IP分片重组策略
 */
typedef struct{
	u_char addr[17];       //!主机的ipv6地址, key 最后一个字符必须是0, 以便于当做字符串使用, 在分配内存时就保证
	u_int32 portnum;       //!被访问的端口的总数
	GTree *frags;          //!主机上的IP分片
	GTree *tcpports;       //!主机上开放的TCP端口
	GTree *udpports;       //!主机上开放的UDP端口
}Ip6Host;

/**
 * @brief 接收/发送主机
 *
 */
typedef struct{
	NetProto proto;   //!EU_NET_IPV4  EU_NET_IPV6
	void     *shost;  //!发送主机, ipv4对应Ip4Host *, ipv6对应Ip6Host *
	void     *dhost;  //!接收主机, ipv4对应Ip4Host *, ipv6对应Ip6Host *
}HostPair;

/** 
 * @brief 通过MAC地址标识的设备
 *
 * hosts记录的是该设备接收到报文中的host信息
 * 如果一个发送到hostA的报文的目的MAC是设备D的MAC地址, 那么hostA就需要被添加到D的hosts中
 * 设备D可能是hostA上的网卡，也可能是中间的网络设备(例如网关、交换机等)的接口
 */
typedef struct{
	u_char mac[7];    //!设备MAC, 最后一个字符必须是0, 以便于当做字符串使用, 在分配内存时就保证
	GTree *ip4hosts;  //!经过该MAC的目的HOST
	GTree *ip6hosts;  //!经过该MAC的目的HOST
}Device;

/**
 * @brief 接收/发送网卡
 *
 */
typedef struct{
	Device   *sdev;   //!发送网卡
	Device   *ddev;   //!接收网卡
}DevicePair;
#endif
