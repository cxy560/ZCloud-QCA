/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */
  
#ifndef __TX_ALLOC_API_H__
#define __TX_ALLOC_API_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bpool_info_s {
    unsigned char htc_cnt;
    unsigned char  htc_reap;
    unsigned char  mac_cnt;
    unsigned char  mac_reap;
    unsigned char  fw_cnt;
    unsigned char  fw_reap;
    unsigned char  free_cnt;
    unsigned char  tx_buf_queued;
    unsigned char  rx_buf_queued;
    unsigned char  resv[3];
    unsigned int  htc_get;
    unsigned int  htc_put;
} bpool_info_t;

void* cdr_tx_pkt_alloc(int len);
void  cdr_bpool_info_get(bpool_info_t *bp_info);

#ifdef __cplusplus
}
#endif

#define TX_BUFF_FAIL -100
#define TXBUF_ALLOC_TIMEOUT 4000

#endif
