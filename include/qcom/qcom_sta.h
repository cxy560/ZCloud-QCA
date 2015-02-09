/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef _QCOM_STA_H_
#define _QCOM_STA_H_

extern A_STATUS qcom_sta_connect_with_scan(A_CHAR ssid[]);
extern A_STATUS qcom_sta_connect_without_scan(A_CHAR ssid[]);
extern A_STATUS qcom_sta_reconnect_start(A_UINT32 seconds);
extern A_STATUS qcom_sta_reconnect_stop(void);
extern A_STATUS qcom_sta_get_rssi(A_UINT8 *prssi);
extern A_STATUS qcom_sta_set_listen_time(A_UINT32 time);
extern A_STATUS qcom_sta_get_listen_time(A_UINT32 *ptime);
#endif
