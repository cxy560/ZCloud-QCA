/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
#include "swat_wmiconfig_common.h"

void
swat_wmiconfig_wps_enable(A_UINT32 enable)
{
    /* 1 : enable */
    /* 0 : disable */
    extern void qcom_wps_enable(int enable);
     qcom_wps_enable(enable); 
}

void
swat_wmiconfig_wps_start(A_UINT32 connect, A_UINT32 mode, A_INT8 * pPin)
{
    extern void qcom_wps_start(int connect, int mode, char *pin);
     qcom_wps_start(connect, mode, (char *)pPin); 
}

void
swat_wmiconfig_ap_wps_set()
{
    extern void qcom_wps_enable(int enable);
     qcom_wps_enable(1); 
}

void
swat_wmiconfig_wep_key_set(A_CHAR * pKey, A_UINT8 key_index)
{
    qcom_sec_set_wepkey(key_index, pKey);
}

void
swat_wmiconfig_wep_key_index_set(A_UINT8 key_index, A_UINT8 mode)
{
    /* confirm with guangde */
    qcom_sec_set_wepkey_index(key_index);
}

void
swat_wmiconfig_wep_passowrd_set(A_CHAR * passwd)
{
    /* qcom_sec_set_passphrase(passwd, A_STRLEN(passwd)); */
    qcom_sec_set_passphrase(passwd);
}
