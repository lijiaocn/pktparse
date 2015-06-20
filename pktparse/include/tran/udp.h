#ifndef UDP_H
#define UDP_H

#include "public/pktstat.h"
#include "public/context.h"

/**
 * @brief 解析传输层udp数据
 *
 * @param host2  接收/发送主机
 * @param size 传输层udp数据包的大小 
 * @param data 传输层udp数据包的开始位置
 * @param info 从传输层udp数据包中解析出的应用层信息
 * @param info_as 重组后的应用层信息
 *
 * @return Port2  接收/发送端口
 */
PortPair parse_udp(ParseContext *context, HostPair hostpair, u_int16 size, 
		u_char *data, AppInfo *info, AppInfo *info_as);

#endif //UDP_H

