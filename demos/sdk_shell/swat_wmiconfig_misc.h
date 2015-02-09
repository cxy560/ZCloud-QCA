/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#ifndef _SWAT_WMICONFIG_MISC_H_
#define _SWAT_WMICONFIG_MISC_H_

void swat_wmiconfig_help();

void swat_wmiconfig_version(A_CHAR* pVersion);

void swat_wmiconfig_reset();

void swat_wmiconfig_prom_start(A_UINT32 seconds);

void swat_wmiconfig_prom_stop();

void swat_wmiconfig_promiscuous_test(A_UINT8 promiscuous);

#endif
