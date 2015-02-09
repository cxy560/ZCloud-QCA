/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */
#ifndef _QCOM_AP_H_
#define _QCOM_AP_H_

extern A_STATUS qcom_ap_start(A_CHAR ssid[]);
extern A_STATUS qcom_ap_set_beacon_interval(A_UINT16 beacon_interval);
extern A_STATUS qcom_ap_get_beacon_interval(A_UINT16 *pbeacon_interval);
extern A_STATUS qcom_ap_hidden_mode_enable(A_BOOL enable);
extern A_STATUS qcom_ap_set_max_station_number(A_UINT32 sta_num);
extern A_STATUS qcom_ap_set_max_station_number(A_UINT32 sta_num);
extern A_STATUS qcom_ap_set_flag(A_UINT32 flg);
extern A_UINT8 qcom_ap_get_numconn(void);
extern A_STATUS qcom_ap_set_inact_time(A_UINT16 inact_time);
#endif
