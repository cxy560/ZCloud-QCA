/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#ifndef __QCOM_NVRAM_H__
#define __QCOM_NVRAM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Erase block size*/
#define NVM_BLOCK_SZ 0x1000

 /* Reads bytes of data from the flash
	return value: actual bytes read or -1 if error
*/
A_STATUS qcom_nvram_read(A_UINT32 offset, A_UCHAR *buf, A_UINT32 size);

/* Writes bytes of data read from the memory to the address in flash.
	return value: actual value written or -1 if error
*/
A_STATUS qcom_nvram_write(A_UINT32 offset, A_UCHAR  *buf, A_UINT32 size);

/* Erases a set of sectors.
	return value: actual bytes erased or -1 if error
*/
A_STATUS qcom_nvram_erase(A_UINT32 offset, A_UINT32 size);

/* Erases all sectors.
	return value: actual bytes erased or -1 if error
*/
A_STATUS qcom_nvram_erase_all();

/*
 * for the user's choice of different nvram area
 */
A_INT32 qcom_nvram_select(A_INT32 partition);


#ifdef __cplusplus
}
#endif

#endif

