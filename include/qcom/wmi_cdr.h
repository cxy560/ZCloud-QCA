/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 *
 * This file contains the definitions of the WMI protocol specified in the
 * Wireless Module Interface (WMI).  It includes definitions of all the
 * commands and events. Commands are messages from the host to the WM.
 * Events and Replies are messages from the WM to the host.
 *
 * Ownership of correctness in regards to commands
 * belongs to the host driver and the WMI is not required to validate
 * parameters for value, proper range, or any other checking.
 *
 */

#ifndef _WMI_CDR_H_
#define _WMI_CDR_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "qcom/stdint.h"
#include "qcom/socket.h"
#include "cdr_api.h"
#include "wmi.h"

#ifndef CDR_WMI_SUPPORT
#define CDR_WMI_SUPPORT
#endif

#define WMI_SYNC_FLAG int

#define CDR_WMI_EVENT_MSG_SIZE   512 //64
#define CDR_WMI_EVENT_MSG_COUNT  8

typedef struct _wmi_device_cdr_s {
	 A_UINT16  devId;
} WMI_STRUCT_CDR;

typedef struct _CDR_wmi_event_msg_s {
    struct cdr_req_s cdr;
    WMI_CDR_HDR hdr;
    A_UINT8  *pEvtBuffer;
    int       Length;
} CDR_WMI_EVENT_MSG;

typedef int (*t_wmi_event_dispatch_handler)(void     *pContext,   /* app supplied context */
                                                                  A_UINT16  EventID,    /* event ID */
                                                                  A_UINT16  info,       /* the info param passed to WMI_SendEvent */
                                                                  A_UINT8  *pEvtBuffer, /* pointer to event data, if needed */
                                                                  int       Length);
extern void cdr_wmi_event_handler_init(t_wmi_event_dispatch_handler cb);

extern WMI_STRUCT_CDR *
CDR_wmi_dev_init(A_UINT16 devId);

extern WMI_STRUCT_CDR *
CDR_wmi_dev_get(A_UINT16 devId);

extern int
CDR_wmi_event_send(void *pContext,
                       A_UINT16  EventID,    /* event ID */
                       A_UINT16  info,       /* the info param passed to WMI_SendEvent */
                       A_UINT8  *pEvtBuffer, /* pointer to event data, if needed */
                       int       Length);    /* length of event data, if needed */

extern A_STATUS
CDR_wmi_listen_interval_cmd(WMI_STRUCT_CDR *wmip, A_UINT32 time);

extern A_STATUS
CDR_wmi_ap_set_beacon_int_cmd(WMI_STRUCT_CDR *wmip, A_UINT16 time_interval);

extern A_STATUS
CDR_wmi_ap_set_country_cmd(WMI_STRUCT_CDR *wmip, char* country_code);

extern A_STATUS
CDR_wmi_addKey_cmd(WMI_STRUCT_CDR *wmip, A_UINT8 keyIndex, CRYPTO_TYPE keyType,
               A_UINT8 keyUsage, A_UINT8 keyLength, A_UINT8 *keyRSC,
               A_UINT8 *keyMaterial, A_UINT8 key_op_ctrl, A_UINT8 *macAddr,
               WMI_SYNC_FLAG sync_flag);

extern A_STATUS
CDR_wmi_set_passphrase_cmd(WMI_STRUCT_CDR * wmip, A_UCHAR * ssid, A_UINT8 ssidLength, A_UINT8 * passphrase, A_UINT8 passLength);

extern A_STATUS
CDR_wmi_scanparams_cmd(WMI_STRUCT_CDR *wmip, A_UINT16 fg_start_sec,
                   A_UINT16 fg_end_sec, A_UINT16 bg_sec,
                   A_UINT16 minact_chdw_msec, A_UINT16 maxact_chdw_msec,
                   A_UINT16 pas_chdw_msec,
                   A_UINT8 shScanRatio, A_UINT8 scanCtrlFlags,
                   A_UINT32 max_dfsch_act_time, A_UINT16 maxact_scan_per_ssid);

extern A_STATUS
CDR_wmi_bssfilter_cmd(WMI_STRUCT_CDR *wmip, A_UINT8 filter, A_UINT32 ieMask);

extern A_STATUS
CDR_wmi_probedSsid_cmd(WMI_STRUCT_CDR *wmip, A_UINT8 index, A_UINT8 flag,
                   A_UINT8 ssidLength, A_UCHAR *ssid);

extern A_STATUS
CDR_wmi_startscan_cmd(WMI_STRUCT_CDR *wmip, WMI_SCAN_TYPE scanType,
                  A_BOOL forceFgScan, A_BOOL isLegacy,
                  A_UINT32 homeDwellTime, A_UINT32 forceScanInterval,
                  A_INT8 numChan, A_UINT16 *channelList);

extern A_STATUS
CDR_wmi_disconnect_cmd(WMI_STRUCT_CDR *wmip);

extern A_STATUS
CDR_wmi_set_pmk_cmd(WMI_STRUCT_CDR *wmip, A_UINT8 *pmk, A_UINT8 pmk_len);

extern A_STATUS
CDR_wmi_connect_cmd(WMI_STRUCT_CDR *wmip, NETWORK_TYPE netType,
                DOT11_AUTH_MODE dot11AuthMode, AUTH_MODE authMode,
                CRYPTO_TYPE pairwiseCrypto, A_UINT8 pairwiseCryptoLen,
                CRYPTO_TYPE groupCrypto, A_UINT8 groupCryptoLen,
                int ssidLength, A_UCHAR *ssid,
                A_UINT8 *bssid, A_UINT16 channel, A_UINT32 ctrl_flags);

extern A_STATUS
CDR_wmi_set_txPwr_cmd(WMI_STRUCT_CDR *wmip, A_UINT8 dbM);

extern A_STATUS
CDR_wmi_set_roam_ctrl_cmd(WMI_STRUCT_CDR * wmip, A_UINT8 roamCtrlType, A_UINT8 *buf);

extern A_STATUS
CDR_wmi_set_channelParams_cmd(WMI_STRUCT_CDR *wmip, A_UINT8 scanParam,
                          WMI_PHY_MODE mode, A_INT8 numChan,
                          A_UINT16 *channelList);

extern A_STATUS
CDR_wmi_set_ht_cap_cmd(WMI_STRUCT_CDR * wmip,
    A_UINT8  band,
    A_UINT8  enable,
    A_UINT8  chan_width_40M_supported,
    A_UINT8  short_GI_20MHz,
    A_UINT8  short_GI_40MHz,
    A_UINT8  intolerance_40MHz,
    A_UINT8  max_ampdu_len_exp
);


extern A_STATUS
CDR_wmi_reconnect_cmd(WMI_STRUCT_CDR *wmip, A_UINT8 *bssid, A_UINT16 channel);

extern A_STATUS
CDR_wmi_set_channel_cmd(WMI_STRUCT_CDR *wmip, A_UINT16 channel);

extern A_STATUS
CDR_wmi_abort_scan_cmd(WMI_STRUCT_CDR *wmip);

extern A_STATUS
CDR_wmi_pmparams_cmd(WMI_STRUCT_CDR *wmip, A_UINT16 idlePeriod,
                 A_UINT16 psPollNum, A_UINT16 dtimPolicy,
                 A_UINT16 tx_wakeup_policy, A_UINT16 num_tx_to_wakeup,
                 A_UINT16 ps_fail_event_policy);
extern A_STATUS
CDR_wmi_susp_enable_cmd(WMI_STRUCT_CDR *wmip);

extern A_STATUS
CDR_wmi_susp_start_cmd(WMI_STRUCT_CDR *wmip, A_UINT32 time);

extern A_STATUS
CDR_wmi_rssi_get_cmd(WMI_STRUCT_CDR *wmip);

	
extern A_STATUS
CDR_wmi_powermode_cmd(WMI_STRUCT_CDR *wmip, A_UINT8 powerMode);

extern A_STATUS
CDR_wmi_wps_start_cmd(WMI_STRUCT_CDR * wmip, WMI_WPS_START_CMD * wps_start);


#ifdef CONFIG_WLAN_AP
extern void
CDR_wmi_ap_profile_commit(WMI_STRUCT_CDR *wmip,
                      A_UINT8 networkType,
                      A_UINT8 dot11AuthMode,
                      A_UINT8 authMode,
                      A_UINT8 pairwiseCryptoType,
                      A_UINT8 pairwiseCryptoLen,
                      A_UINT8 groupCryptoType,
                      A_UINT8 groupCryptoLen,
                      A_UINT8 ssidLength, A_UCHAR * ssid, A_UINT16 channel, A_UINT8 * bssid,
                      A_UINT32 ctrl_flags);

extern void
CDR_wmi_allow_aggr(WMI_STRUCT_CDR * wmip, A_UINT16 tx_allow_aggr, A_UINT16 rx_allow_aggr);

#endif 

extern A_STATUS
CDR_wmi_sock_stack_init_cmd(WMI_STRUCT_CDR * wmip);

extern A_STATUS
CDR_wmi_ip_config_cmd(WMI_STRUCT_CDR * wmip, A_UINT32  mode, A_UINT32 ipv4,A_UINT32 subnetMask, A_UINT32 gateway4  );
extern A_STATUS
CDR_wmi_ipconfig_dhcp_pool_cmd(WMI_STRUCT_CDR * wmip, A_UINT32 startaddr, A_UINT32 endaddr, A_INT32 leasetime);

extern A_STATUS
CDR_wmi_dns_get_cmd(WMI_STRUCT_CDR *wmip, A_UINT32 mode, A_UINT32 domain, char *hostname);

extern A_STATUS
CDR_wmi_dns_en_cmd(WMI_STRUCT_CDR *wmip, A_INT32 command);

extern A_STATUS
CDR_wmi_dns_srv_cmd(WMI_STRUCT_CDR * wmip, ip_addr serv_ip, int mode);

extern A_STATUS
CDR_wmi_ping_cmd(WMI_STRUCT_CDR * wmip, A_UINT32 ipv4_addr, A_UINT32 size);

extern void
CDR_wmi_set_promiscuous_mode(WMI_STRUCT_CDR * wmip, A_UINT8 enable);

extern A_STATUS
CDR_wmi_ap_set_num_sta_cmd(WMI_STRUCT_CDR * wmip, A_UINT8 num_sta);

extern A_STATUS
CDR_wmi_ap_hidden_ssid_cmd(WMI_STRUCT_CDR * wmip, A_UINT8 hidden_ssid);

extern A_STATUS
CDR_wmi_ap_set_inact_time_cmd(WMI_STRUCT_CDR * wmip, A_UINT16 inact_time);

#ifdef __cplusplus
}
#endif

#endif /* _WMI_H_ */

