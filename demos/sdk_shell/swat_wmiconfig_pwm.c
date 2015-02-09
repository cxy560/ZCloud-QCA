/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
#include "swat_wmiconfig_common.h"

void
swat_wmiconfig_power_mode_set(A_UINT32 powerMode)
{
     qcom_power_set_mode(powerMode);
}

void
swat_wmiconfig_pmparams(A_UINT16 idlePeriod,
                        A_UINT16 psPollNum, A_UINT16 dtimPolicy,
                        A_UINT16 tx_wakeup_policy, A_UINT16 num_tx_to_wakeup,
                        A_UINT16 ps_fail_event_policy)
{

     qcom_power_set_parameters(idlePeriod, psPollNum, dtimPolicy, tx_wakeup_policy, 
                                 num_tx_to_wakeup, ps_fail_event_policy); 
}

void
swat_wmiconfig_suspenable()
{
    /* 1: enable */
    /* 0: disable */
     qcom_suspend_enable(1);
}

void
swat_wmiconfig_suspstart(A_UINT32 susp_time)
{
     qcom_suspend_start(susp_time);
}
