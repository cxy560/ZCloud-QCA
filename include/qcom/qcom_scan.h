/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef _QCOM_SCAN_H_
#define _QCOM_SCAN_H_

typedef struct _qcom_scan_info
{
    unsigned char channel;
    unsigned char ssid_len;
    unsigned char rssi;
    unsigned char security_enabled;
    unsigned short beacon_period;
    unsigned char preamble;
    unsigned char bss_type;
    unsigned char bssid[6];
    unsigned char ssid[32];
    unsigned char rsn_cipher;
    unsigned char rsn_auth;
    unsigned char wpa_cipher;
    unsigned char wpa_auth;
    unsigned short caps;
    unsigned char wep_support;
    unsigned char reserved; //keeps struct on 4-byte boundary
} QCOM_BSS_SCAN_INFO, * QCOM_BSS_SCAN_INFO_PTR;

extern A_STATUS qcom_scan_set_mode(A_UINT32 mode);
extern A_STATUS qcom_scan_get_mode(A_UINT32 *pmode);
extern A_STATUS qcom_scan_set_para(A_UINT16 max_dwell_time, A_UINT16 pass_dwell_time);
extern A_STATUS qcom_scan_bss_start(A_CHAR ssid[]);
extern A_STATUS qcom_scan_all_bss_start(void);
extern A_STATUS qcom_scan_get_bss_number(A_UINT32 *pnumber);
extern A_STATUS qcom_scan_get_bss_info(A_UINT32 id, QCOM_BSS_SCAN_INFO *pbuf);

#endif


