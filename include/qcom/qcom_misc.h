/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#ifndef __QCOM_MISC_H__
#define __QCOM_MISC_H__

A_STATUS 
qcom_mac_get(A_UINT8 *pMac);

extern A_STATUS qcom_free_heap_size_get(A_UINT32 *psize);
extern A_STATUS qcom_buffer_info_get(A_UINT8 *pmac_cnt, A_UINT8 *pmac_reap, A_UINT8 *phtc_cnt, A_UINT8 *phtc_reap, \
       A_UINT8 *pfw_cnt, A_UINT8 *pfw_reap, A_UINT8 *pfree_cnt, A_UINT32 *phtc_get, A_UINT32 *phtc_put);

extern A_STATUS qcom_promiscuous_enable(A_UINT8 promiscuous);
typedef void (*WLAN_PROMISCUOUS_CB)(unsigned char *buf, int length);
extern A_STATUS qcom_set_promiscuous_rx_cb(A_UINT8 *promiscRxBuf, WLAN_PROMISCUOUS_CB cb);

extern char *qcom_firmware_version_get();

#endif
