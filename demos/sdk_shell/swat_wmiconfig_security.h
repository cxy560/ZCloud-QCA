/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#ifndef _SWAT_WMICONFIG_SECURITY_H_
#define _SWAT_WMICONFIG_SECURITY_H_

void swat_wmiconfig_wps_enable(A_UINT32 enable);

void swat_wmiconfig_wps_start(A_UINT32 connect, A_UINT32 mode, A_INT8* pPin);

void swat_wmiconfig_wep_key_set(A_CHAR* pKey, A_UINT8 key);

void swat_wmiconfig_wep_key_index_set(A_UINT8 key_index, A_UINT8 mode);

void swat_wmiconfig_wep_passowrd_set(A_CHAR* pPassowrd);


void swat_wmiconfig_ap_wps_set();

#endif

