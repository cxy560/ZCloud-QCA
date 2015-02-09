/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
#include "swat_wmiconfig_common.h"

A_UINT32
_inet_addr(A_CHAR *str)
{
    A_UINT32 ipaddr;
    A_UINT32 data[4];
    A_UINT32 ret;

    ret = A_SSCANF(str, "%3d.%3d.%3d.%3d", data, data + 1, data + 2, data + 3);
    if (ret < 0) {
        return 0;
    } else {
        ipaddr = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
    }
    return ipaddr;
}

char str[20];

A_CHAR *
_inet_ntoa(A_UINT32 ip)
{
    A_MEMSET(str, 0, sizeof (str));
    qcom_sprintf(str, "%u.%u.%u.%u",
            (unsigned) (ip >> 24),
            (unsigned) ((ip >> 16) & 0xff), (unsigned) ((ip >> 8) & 0xff), (unsigned) (ip & 0xff));
    return str;
}
    void
swat_wmiconfig_ipstatic(A_CHAR * pIpAddr, A_CHAR * pNetMask, A_CHAR * pGateway)
{
    A_UINT32 address;
    A_UINT32 submask;
    A_UINT32 gateway;

    address = _inet_addr(pIpAddr);
    submask = _inet_addr(pNetMask);
    gateway = _inet_addr(pGateway);

    qcom_ip_address_set(address, submask, gateway);
}

    void
swat_wmiconfig_ipdhcp(void)
{
    /* 1: enbale; 0:disable */
    qcom_dhcpc_enable(1);
}

    void
swat_wmiconfig_dns(A_CHAR *name)
{
    A_UINT32 ipAddress;
    if (qcom_dnsc_get_host_by_name(name, &ipAddress)== A_OK) {
        A_CHAR *ipaddr;
        ipaddr = _inet_ntoa(ipAddress);
        SWAT_PTF("Get IP address of host %s \n", name);
        SWAT_PTF("ip address is %s\n", (char *)_inet_ntoa(ipAddress));
    } else {
        SWAT_PTF("The IP of host %s is not gotten\n", name);
    }

    return;
}

    void
swat_wmiconfig_dns_enable(A_CHAR *enable)
{
    int flag;
    if (!strcmp(enable, "start"))
        flag = 1;
    else if (!strcmp(enable, "stop"))
        flag = 0;
    else {
        SWAT_PTF("input paramenter should be start or stop !\n");
        return;
    }
    /* 1: enable; 0:diable; */
    qcom_dnsc_enable(flag);
    return;
}

    void
swat_wmiconfig_dns_svr_add(A_CHAR *ipaddr)
{
    A_UINT32 ip = 0;
    ip = _inet_addr(ipaddr);
    if (0 == ip) {
        SWAT_PTF("input ip addr is not valid!\n");
        return;
    }
    qcom_dnsc_add_server_address(ip);
    return;
}

    void
swat_wmiconfig_dns_svr_del(A_CHAR *ipaddr)
{
    A_UINT32 ip = 0;
    ip = _inet_addr(ipaddr);
    if (0 == ip) {
        SWAT_PTF("input ip addr is not valid!\n");
        return;
    }
    qcom_dnsc_del_server_address(ip);
    return;
}

    void
swat_wmiconfig_ipconfig(void)
{
    A_UINT8 macAddr[6];
    A_MEMSET(&macAddr, 0, sizeof (macAddr));
    A_UINT32 ipAddress;
    A_UINT32 submask;
    A_UINT32 gateway;
    A_UINT32 dns;

    qcom_mac_get((A_UINT8 *) & macAddr);
    qcom_ip_address_get( &ipAddress, &submask, &gateway);
    qcom_dns_server_address_get(&dns);

    SWAT_PTF(" mac addr    =   %x:%x:%x:%x:%x:%x\n",
            macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
    SWAT_PTF("IP:");
    IPV4_ORG_PTF(ipAddress);
    ORG_PTR(" Mask:");
    IPV4_PTF(submask);
    SWAT_PTF(", Gateway:");
    IPV4_PTF(gateway);
    SWAT_PTF(", Dns:");
    IPV4_PTF(dns);
}

    void
swat_wmiconfig_dhcp_pool(A_CHAR * pStartaddr, A_CHAR * pEndaddr, A_UINT32 leasetime)
{
    A_UINT32 startaddr, endaddr;

    startaddr = _inet_addr(pStartaddr);
    endaddr = _inet_addr(pEndaddr);

    qcom_dhcps_set_pool(startaddr, endaddr, leasetime);
}



