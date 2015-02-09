/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */
  
#ifndef __QCOM_PWR__
#define __QCOM_PWR__

A_STATUS qcom_power_set_mode(A_UINT32 mode);
A_STATUS qcom_power_get_mode(A_UINT32 *pmode);
A_STATUS qcom_suspend_enable(A_BOOL enable);
A_STATUS qcom_suspend_start(A_UINT32 time);
A_STATUS qcom_power_set_parameters(A_UINT16 idlePeriod, A_UINT16 psPollNum, 
    A_UINT16 dtimPolicy, A_UINT16 tx_wakeup_policy, A_UINT16 num_tx_to_wakeup, 
    A_UINT16 ps_fail_event_policy);
int qcom_wlan_power_wakeup_start(A_UINT32 ms);
int qcom_wlan_power_wakeup_stop(void);
void qcom_wlan_wakeup_gpio_handler(void *pcontext);
A_STATUS qcom_wlan_wakeup_gpio_initconfig(void);


#endif /* __QCOM_PWR__ */

