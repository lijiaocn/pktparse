#ifndef ETHERNET_H
#define ETHERNET_H

#include "public/pktstat.h"

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
		LinkInfo *link/**<[out] 解析出的链路层信息*/, NetInfo *net/**<[out] 解析出报文的网络层信息*/);

#endif //ETHERNET_H

