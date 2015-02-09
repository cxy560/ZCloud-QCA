/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_system.h"

void user_main(void);

void
app_start(void)
{
    qcom_sys_start(user_main);
}


