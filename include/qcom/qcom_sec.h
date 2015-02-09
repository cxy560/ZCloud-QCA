/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __QCOM_SEC_H__
#define __QCOM_SEC_H__

extern A_STATUS qcom_sec_set_wepkey(A_UINT32 keyindex, A_CHAR *pkey);
extern A_STATUS qcom_sec_get_wepkey(A_UINT32 keyindex, A_CHAR *pkey);
extern A_STATUS qcom_sec_set_wepkey_index(A_UINT32 keyindex);
extern A_STATUS qcom_sec_get_wepkey_index(A_UINT32 *pkeyindex);
extern A_STATUS qcom_sec_set_auth_mode(A_UINT32 mode);
extern A_STATUS qcom_sec_get_auth_mode(A_UINT32 *pmode);
extern A_STATUS qcom_sec_set_encrypt_mode(A_UINT32 mode);
extern A_STATUS qcom_sec_get_encrypt_mode(A_UINT32 *pmode);
extern A_STATUS qcom_sec_set_passphrase(A_CHAR *passphrase);
extern A_STATUS qcom_sec_get_passphrase(A_CHAR *passphrase);
extern A_STATUS qcom_sec_set_channel(A_UINT32 channel);
extern A_STATUS qcom_m4_handle_enable(A_UINT8 enable);
#endif
