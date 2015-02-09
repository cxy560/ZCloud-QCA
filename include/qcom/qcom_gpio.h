/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#ifndef __QCOM_GPIO_H__
#define __QCOM_GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    QCOM_GPIO_PIN_INT_NONE = 0,
    QCOM_GPIO_PIN_INT_RISING_EDGE,
    QCOM_GPIO_PIN_INT_FALLING_EDGE,
    QCOM_GPIO_PIN_INT_BOTH_EDGE,
    QCOM_GPIO_PIN_INT_LEVEL_LOW,
    QCOM_GPIO_PIN_INT_LEVEL_HIGH,
} QCOM_GPIO_PIN_INTERRUPT_MODE;

typedef enum {
    QCOM_GPIO_PIN_PULLNONE = 0,
    QCOM_GPIO_PIN_PULLUP,
    QCOM_GPIO_PIN_PULLDOWN
} QCOM_GPIO_PIN_PULL_TYPE;

typedef enum {
    QCOM_GPIO_PIN_4MA_DRIVE = 0,
    QCOM_GPIO_PIN_8MA_DRIVE,
    QCOM_GPIO_PIN_12MA_DRIVE,
    QCOM_GPIO_PIN_24MA_DRIVE	
} QCOM_GPIO_PIN_STRENGH_TYPE;
typedef enum {
    QCOM_GPIO_PIN_DRIVER_PUSHPULL = 0,
    QCOM_GPIO_PIN_DRIVER_OPENDRAIN
} QCOM_GPIO_PIN_DRIVER_TYPE;

typedef struct _qcom_gpio_interrupt_info_t {
    int         pin;
    void        (*gpio_pin_int_handler_fn)(void *arg);
    void        *arg;
    unsigned int internal_flags;
} qcom_gpio_interrupt_info_t;

/* configure a GPIO pin's config value, direction and pullup/down type */
/* combo API, includes gpio_configure_mux/ direction /gpio_configure_pulltype */
A_STATUS qcom_gpio_config_pin(A_UINT32 pin, A_CHAR config_val, A_INT32 input, QCOM_GPIO_PIN_PULL_TYPE pull_type) ;

/*set the GPIO direction for specific PIN*/
void
qcom_gpio_set_pin_dir(A_INT32 co_pin, A_INT32 dir_cfg) ;

/* set output high, note: pin must be configured as an output */
void qcom_gpio_set_pin_high(A_UINT32 pin);

/* set output low note: pin must be configured as an output */
void qcom_gpio_set_pin_low(A_UINT32 pin);

/* get input state of pin, TRUE = H, FALSE = L */
A_BOOL qcom_gpio_get_pin_value(A_UINT32 pin);

/* register for GPIO pin interrupts */
void qcom_gpio_register_pin_interrupt(qcom_gpio_interrupt_info_t * gpio_interrupt_info);

/* de-register previously registered interrupts */
void qcom_gpio_deregister_pin_interrupt(qcom_gpio_interrupt_info_t *gpio_interrupt_info);

/* set the interrupting mode for the GPIO pin, must have a registered handler */
void qcom_gpio_set_interrupt_mode(qcom_gpio_interrupt_info_t *gpio_int_info, QCOM_GPIO_PIN_INTERRUPT_MODE Mode);

/*enable GPIO interrupts on specific GPIO pin*/
void qcom_gpio_start_interrupt(qcom_gpio_interrupt_info_t *gpio_int_info);

/*set pin hw pad config*/
void qcom_gpio_set_pin_pad(A_UINT32 pin,QCOM_GPIO_PIN_PULL_TYPE pull_type,QCOM_GPIO_PIN_STRENGH_TYPE strengh_type,QCOM_GPIO_PIN_DRIVER_TYPE driver_type);

#ifdef __cplusplus
}
#endif

#endif

