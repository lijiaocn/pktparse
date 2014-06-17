/*
 * =====================================================================================
 *
 *       Filename:  parselink.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/29/2014 09:34:54 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef PARSELINK_H
#define PARSELINK_H

#include "public/context.h"
#include "public/pktstat.h"

DealStat parse_link(ParseContext *context, PktStat *pktstat);
DealStat parse_linkup(ParseContext *context, PktStat *pktstat);

#endif //PARSELINK_H

