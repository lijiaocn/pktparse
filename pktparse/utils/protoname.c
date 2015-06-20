/*
 * =====================================================================================
 *
 *       Filename:  protoname.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/17/2014 05:59:43 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "public/netproto.h"

char *g_link_proto_name[]={
	"invalid",
	"rfc894",
	"ieee802",
	"max"
};

char *g_linkup_proto_name[]={
	"invalid",
	"arp",
	"rarp",
	"mpls",
	"802.1Q",
	"pppoed",
	"pppoes",
	"max"
};

char *g_net_proto_name[]={
	"invalid",
	"ipv4",
	"ipv6",
	"max"
};

char *g_tran_proto_name[]={
	"invalid",
	"tcp",
	"udp",
	"max"
};

char *g_app_proto_name[]={
	"invalid",
	"http",
	"unknow",
	"max"
};

inline char* get_link_name(LinkProto i)
{
	return g_link_proto_name[i];
}

inline char* get_linkup_name(LinkProto i)
{
	return g_linkup_proto_name[i];
}

inline char* get_net_name(LinkProto i)
{
	return g_net_proto_name[i];
}

inline char* get_tran_name(LinkProto i)
{
	return g_tran_proto_name[i];
}

inline char* get_app_name(LinkProto i)
{
	return g_app_proto_name[i];
}
