/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __WIFI_API_H__
#define __WIFI_API_H__

#include "wmi.h"

struct qcom_request_s {
    unsigned int cmd;
};

void qcom_print_string(char *str);
void qcom_print_bssid(A_UINT8 *bssid);
#define QCOM_WIFI_CMD_PRINT_STRING 1
#define QCOM_WIFI_CMD_PRINT_BSSID  2
struct qcom_print_string_request_s {
    unsigned int cmd; /* QCOM_WIFI_CMD_PRINT_STRING */
    char *str;
    unsigned int len;
};

struct qcom_print_bssid_request_s {
    unsigned int cmd; /* QCOM_WIFI_CMD_PRINT_BSSID */
    A_UINT8 *bssid; 
};


extern void qcom_set_scan_params(int fgScan, A_UINT16 max_dwell_time, A_UINT16 pass_dwell_time);

extern void qcom_wakeupmgr_setting(A_UINT32 options, A_UINT32 wake_msec);
extern void qcom_wakeupmgr_register_backup();
extern void qcom_wakeupmgr_recall();
extern void qcom_wakeupmgr_set_testvalue();

#endif /* __WIFI_API_H__ */

