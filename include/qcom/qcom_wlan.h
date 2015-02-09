/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef _QCOM_WLAN_H__
#define _QCOM_WLAN_H__

typedef enum {
    QCOM_WLAN_DEV_MODE_STATION = 0,
    QCOM_WLAN_DEV_MODE_AP,
    QCOM_WLAN_DEV_MODE_INVALID
}QCOM_WLAN_DEV_MODE;

extern A_STATUS qcom_op_set_mode(A_UINT32 mode);
extern A_STATUS qcom_op_get_mode(A_UINT32 *pmode);

extern A_STATUS qcom_disconnect(void);
extern A_STATUS qcom_set_country_code(A_CHAR *pcountry_code);
extern A_STATUS qcom_get_country_code(A_CHAR *pcountry_code);
extern A_STATUS qcom_sta_reconnect_enable(A_BOOL state);
typedef enum {
    QCOM_11A_MODE = 0x1,
    QCOM_11B_MODE = 0x2,
    QCOM_11G_MODE = 0x3,
    QCOM_11N_MODE = 0x4,
    QCOM_HT40_MODE = 0x5,
} QCOM_WLAN_DEV_PHY_MODE;
extern A_STATUS qcom_set_phy_mode(A_UINT8 phymode);
extern A_STATUS qcom_get_phy_mode(A_UINT8 *pphymode);

//channel value start from 1
extern A_STATUS qcom_set_channel(A_UINT16 channel);
extern A_STATUS qcom_get_channel(A_UINT16 *pchannel);
extern A_STATUS qcom_get_channel_freq(A_UINT16 *pch_freq);

extern A_STATUS qcom_set_tx_power(A_UINT32 dbm);
extern A_STATUS qcom_get_tx_power(A_UINT32 *pdbm);

extern A_STATUS qcom_roam_mode_enable(A_UINT8 enable);
extern A_STATUS qcom_roam_para_set(A_UINT16 scan_perid, A_INT16 scan_threshold, A_INT16 rom_threshold, A_UINT8 rssi_floor);

extern A_STATUS qcom_allow_aggr_set_tid(A_UINT16 tx_allow_aggr, A_UINT16 rx_allow_aggr);
extern A_STATUS qcom_allow_aggr_get_tid(A_UINT16 *ptx_allow_aggr, A_UINT16 *prx_allow_aggr);

extern A_STATUS qcom_set_connect_callback(void *callback);

extern A_STATUS qcom_get_ssid(A_CHAR *pssid);
extern A_STATUS qcom_get_state(A_UINT8 *pstate);

extern A_STATUS qcom_init_dev_context(void);



#define DEV_CHANNEL_NUM_MAX_5G      57
#define DEV_CHANNEL_NUM_MAX_24      14

extern A_UINT16 freq_bands[];

#define MIN_HZ_24G 2412
#define MAX_HZ_24G 2484
#define MIN_HZ_5G CHNNEL_5G_PART1_START_HZ
#define MAX_HZ_5G CHNNEL_5G_PART2_END_HZ

#define MIN_NO_24G 1
#define MAX_NO_24G 14
#define MIN_NO_5G CHNNEL_5G_PART2_START_NO          /* 16 */
#define MAX_NO_5G CHNNEL_5G_PART1_END_NO

#define CHNNEL_5G_PART1_START_HZ 4915
#define CHNNEL_5G_PART1_END_HZ   4980
#define CHNNEL_5G_PART2_START_HZ 5080
#define CHNNEL_5G_PART2_END_HZ   5825

#define CHNNEL_5G_HZ_2_NO_P1(_HZ) (CHNNEL_5G_PART1_START_NO + (_HZ - CHNNEL_5G_PART1_START_HZ)/5)
#define CHNNEL_5G_HZ_2_NO_P2(_HZ) (CHNNEL_5G_PART2_START_NO + (_HZ - CHNNEL_5G_PART2_START_HZ)/5)
#define CHNNEL_5G_PART1_START_NO  (183)
#define CHNNEL_5G_PART1_END_NO    CHNNEL_5G_HZ_2_NO_P1(CHNNEL_5G_PART1_END_HZ)
#define CHNNEL_5G_PART2_START_NO  (16)   /* skip 5G channel 7-12, since confilict with 2.4G and no country use it */   
#define CHNNEL_5G_PART2_END_NO    CHNNEL_5G_HZ_2_NO_P2(CHNNEL_5G_PART2_END_HZ)

#define CHANNEL_HZ_IS_24G(_HZ)    ((_HZ >= MIN_HZ_24G) && (_HZ <= MAX_HZ_24G))
#define CHANNEL_HZ_IS_5G_P1(_HZ)  ((_HZ >= CHNNEL_5G_PART1_START_HZ ) && (_HZ <= CHNNEL_5G_PART1_END_HZ ))

#define CHANNEL_HZ_IS_5G_P2(_HZ)  ((_HZ>=CHNNEL_5G_PART2_START_HZ ) && (_HZ<=CHNNEL_5G_PART2_END_HZ ))

#define CHANNEL_HZ_IS_5G(_HZ)     (CHANNEL_HZ_IS_5G_P1(_HZ) || CHANNEL_HZ_IS_5G_P2(_HZ))
#define CHNNEL_5G_HZ_2_NO(_HZ)    (CHANNEL_HZ_IS_5G_P1(_HZ)?CHNNEL_5G_HZ_2_NO_P1(_HZ):CHNNEL_5G_HZ_2_NO_P2(_HZ))

#define CHANNEL_NO_IS_24G(_NO)    ((_NO >= MIN_NO_24G) && (_NO <= MAX_NO_24G))                                                                           
#define CHANNEL_NO_IS_5G_P1(_NO)  ((_NO >= CHNNEL_5G_PART1_START_NO ) && (_NO <= CHNNEL_5G_PART1_END_NO ))      
#define CHNNEL_5G_NO_2_HZ_P1(_NO) (CHNNEL_5G_PART1_START_HZ + (_NO - CHNNEL_5G_PART1_START_NO)*5)
#define CHANNEL_NO_IS_5G_P2(_NO)  ((_NO >= CHNNEL_5G_PART2_START_NO ) && (_NO <= CHNNEL_5G_PART2_END_NO ))      
#define CHNNEL_5G_NO_2_HZ_P2(_NO) (CHNNEL_5G_PART2_START_HZ + (_NO - CHNNEL_5G_PART2_START_NO)*5)                                         
#define CHANNEL_NO_IS_5G(_NO)     (CHANNEL_NO_IS_5G_P1(_NO) || CHANNEL_NO_IS_5G_P2(_NO))                    
#define CHNNEL_5G_NO_2_HZ(_NO)    (CHANNEL_NO_IS_5G_P1(_NO)?CHNNEL_5G_NO_2_HZ_P1(_NO):CHNNEL_5G_NO_2_HZ_P2(_NO))


#endif
