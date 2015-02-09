/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#ifndef _SWAT_WMICONFIG_NETWORK_H_
#define _SWAT_WMICONFIG_NETWORK_H_

void swat_wmiconfig_ipstatic(A_CHAR* pIpAddr, A_CHAR* pNetMask, A_CHAR* pGateway);

void swat_wmiconfig_ipdhcp(void);

void swat_wmiconfig_ipconfig(void);

void swat_wmiconfig_dns(char* name);

void swat_wmiconfig_dns2(char* name, int af);

void swat_wmiconfig_dns_enable(char* enable);

void swat_wmiconfig_dns_svr_add(char* ipaddr);

void swat_wmiconfig_dns_svr_del(char* ipaddr);

void swat_wmiconfig_dhcp_pool(A_CHAR *pStartaddr, A_CHAR *pEndaddr, A_UINT32 leasetime);

A_CHAR * _inet_ntoa(A_UINT32 ip);
A_UINT32 _inet_addr(A_CHAR *str);

#endif
