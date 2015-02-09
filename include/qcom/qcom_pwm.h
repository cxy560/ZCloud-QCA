/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __QCOM_PWM_H__
#define __QCOM_PWM_H__

/**
 *This function is to enable and start PWM module with the default configuration.
 *The default configuration of PWM module is that:
 *      the clock is (1/256)*Crystal,
 *      the duty cycle is 0.
 */
void qcom_pwm_start(void);

/**
 *This function is to stop PWM module.
 *
 */
void qcom_pwm_stop(void);

/**
 * This function is to set the clock frequence of PWM module. Clock source for PWM
 * module is Crystal.
 * The val should be a value from 0 to 255.
 * The clock frequence of PWM module is: Crystal/(val+1).
 */
int qcom_pwm_set_clock(int val);

/**
 * This function is to set the duty cycle of PWM module.
 * The val should be a value from 0 to 255.
 */
int qcom_pwm_set_duty_cycle(int val);

/**
 * This function is to select the pin which is used for PWM output.
 */
void qcom_pwm_select_pin(int pin);

/**
 * This function is to close the pin which has been selected for PWM output.
 */
void qcom_pwm_deselect_pin(int pin);
#endif
