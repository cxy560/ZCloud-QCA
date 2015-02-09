/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#ifndef _SWAT_WMICONFIG_PWR_H_
#define _SWAT_WMICONFIG_PWR_H_

void swat_wmiconfig_power_mode_set(A_UINT32 powerMode);

void swat_wmiconfig_pmparams(A_UINT16 idlePeriod,
                 A_UINT16 psPollNum, A_UINT16 dtimPolicy,
                 A_UINT16 tx_wakeup_policy, A_UINT16 num_tx_to_wakeup,
                 A_UINT16 ps_fail_event_policy);

void swat_wmiconfig_suspenable();

void swat_wmiconfig_suspstart(A_UINT32 susp_time);

void swat_wmiconfig_suspstart(A_UINT32 susp_time);

#endif

