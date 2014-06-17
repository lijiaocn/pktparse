#ifndef IPV4_H
#define IPV4_H

#include "public/pktstat.h"

/**
 * @brief 解析网络层ipv4数据
 *
 * @param dev2 接收/发送设备
 * @param size 网络层ipv4数据包的大小
 * @param data 网络层ipv4数据包的开始位置
 * @param info 从网络层ipv4数据包中解析出的传输层信息
 * @param info_as 分片重组后的传输层信息
 *
 * @return HostPair 接收/发送主机
 */
HostPair parse_ipv4(DevicePair dev2, u_int16 size, u_char *data, 
		TranInfo *info/**<[out] 解析出报文的传输层信息*/,
		TranInfo *info_as /**<[out] 分片重组后的传输层信息 */);

#endif //IPV4_H

