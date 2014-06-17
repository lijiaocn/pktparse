/*
 * =====================================================================================
 *
 *       Filename:  pktparse_test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/14/2014 02:50:38 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include "pktparse.h"

int main(int argc, char const* argv[])
{
	
	int ret;

	printf("##########################################\n");
	printf("--- arp_test start! ---\n");
	printf("##########################################\n");
	printf("\n");
	ret = arp_test();
	printf("--- arp_test end! ---\n");
	if(ret == 0){
		printf("\narp_test is OK!\n");
	}

	printf("\n");

	printf("##########################################\n");
	printf("--- ip_test start! ---\n");
	printf("##########################################\n");
	printf("\n");
	ret = ip_test();
	printf("--- ip_test end! ---\n");
	if(ret == 0){
		printf("\nip_test is OK!\n");
	}

	printf("\n");

	return 0;
}

