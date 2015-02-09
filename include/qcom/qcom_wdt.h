/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#ifndef __QCOM_GPIO_H__
#define __QCOM_GPIO_H__
/**
 * This function is to feed watchdog. 
 * 
 */
void qcom_watchdog_feed( void );
/**
 * This function is to start watch dog timer. It can only be called once at the system beginning. When system hangs, 
 * the watch dog won't be feed and system will reset
 *   
 * @param enable		0, disable; 1-enable and use default watchdog feeder; 2 - enable and use user-defined watchdog feeder.
 * @param timeout       expiration time in seconds. If watch dog is not feed in x seconds, system will reset
 * 
 */
void qcom_watchdog(int enable, int timeout);
#endif
