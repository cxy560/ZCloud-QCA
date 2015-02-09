/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */
  
#ifndef __QCOM_NETWORK__
#define __QCOM_NETWORK__

A_STATUS qcom_ip_address_set(A_UINT32 address, A_UINT32 submask, A_UINT32 gateway);
A_STATUS qcom_ip_address_get(A_UINT32 *paddress, A_UINT32 *psubmask, A_UINT32 *pgateway);
A_STATUS qcom_dns_server_address_get(A_UINT32 *pdns);
A_STATUS qcom_ip_ping(A_UINT32 host, A_UINT32 size, A_UINT32 count, A_UINT32 interval);
A_STATUS qcom_dhcpc_enable(A_BOOL enable);
A_STATUS qcom_dhcps_enable(A_BOOL enable);
A_STATUS qcom_dhcps_set_pool(A_UINT32 startip, A_UINT32 endip, A_UINT32 leasetime);
A_STATUS qcom_dhcps_get_pool(A_UINT32 *pstartip, A_UINT32 *pendip, A_UINT32 *pleasetime);
A_STATUS qcom_dnsc_enable(A_BOOL enable);
A_STATUS qcom_dnsc_add_server_address(A_UINT32 ipaddress);
A_STATUS qcom_dnsc_del_server_address(A_UINT32 ipaddress);
A_STATUS qcom_dnsc_get_host_by_name(A_CHAR *pname, A_UINT32 *pipaddress);
A_STATUS qcom_set_dhcpc_hostname(A_UCHAR* pstr);
A_STATUS qcom_get_dhcpc_hostname(A_UCHAR* pstr);

void qcom_wmi_event_socket_process(void *pevent_buff);

#endif /* __QCOM_NETWORK__ */

