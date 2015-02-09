/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#include "qcom_common.h"

//extern int A_PRINTF(const char *fmt, ...);
TX_EVENT_FLAGS_GROUP event_flags_0;
static qcom_timer_t shell_timer;
extern void qcom_msleep(int ms);
extern int qcom_task_start(void (*fn) (unsigned int), unsigned int arg, int stk_size, int tk_ms);
void
task_execute_cli_cmd()
{
    void _console_process_char(void);
    UINT status;
    ULONG actual_flags;
    
    /* task resource recycle */
    void qcom_task_timer_handler(unsigned int alarm, void *data);
    qcom_timer_init(&shell_timer, qcom_task_timer_handler, NULL, 3 * 1000, PERIODIC);
    qcom_timer_start(&shell_timer);

    status = tx_event_flags_create(&event_flags_0, "event flags 0");
    A_PRINTF("create event 0x%x.\n", status);

    while (1) {
        status = tx_event_flags_get(&event_flags_0, 0x1, TX_OR_CLEAR,
                                    &actual_flags, TX_WAIT_FOREVER);
        /* Check status.  */
        if (status != TX_SUCCESS) {
            A_PRINTF("take event failed, 0x%x\n", status);
            qcom_thread_msleep(10000);
            continue;
        }

        _console_process_char();
    }
}

void
wakeup_task_execute_cli_cmd()
{
    UINT status;

    status = tx_event_flags_set(&event_flags_0, 0x1, TX_OR);
    /* Check status.  */
    if (status != TX_SUCCESS) {
        A_PRINTF("wake up cli failed.\n");
    }

}

void
start_task_exec_cli_cmd(ULONG index)
{
    UINT status;
    A_PRINTF("start task to execute cli command.\n");
    /* added by xzm 2013-7-9 */
    status = tx_event_flags_create(&event_flags_0, "event flags 0");
    A_PRINTF("create event 0x%x.\n", status);
    qcom_task_start(task_execute_cli_cmd, index, 3072, 50);
}

