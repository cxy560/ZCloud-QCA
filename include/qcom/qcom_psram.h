/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#ifndef __QCOM_PSRAM_H__
#define __QCOM_PSRAM_H__

#ifdef __cplusplus
extern "C" {
#endif

 /* Reads bytes of data from the flash
	return value: number of bytes read
*/
A_INT32 qcom_psram_read(A_UINT32 offset, A_UCHAR *buf, A_UINT32 size);

/* Writes bytes of data read from the memory to the address in flash.
	return value: number of bytes written
*/
A_INT32 qcom_psram_write(A_UINT32 offset, A_UCHAR  *buf, A_UINT32 size);


#ifdef __cplusplus
}
#endif

#endif

