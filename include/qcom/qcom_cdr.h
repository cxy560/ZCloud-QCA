/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __QCOM_CDR__H__
#define __QCOM_CDR__H__

#include "qcom/stdint.h"

extern void qcom_wlan_connect(NETWORK_TYPE netType, DOT11_AUTH_MODE dot11AuthMode, AUTH_MODE authMode,
             CRYPTO_TYPE pairwiseCrypto, A_UINT8 pairwiseCryptoLen, CRYPTO_TYPE groupCrypto,
             A_UINT8 groupCryptoLen, int ssidLength, A_UCHAR * ssid,
             A_UINT8 * bssid, A_UINT16 channel, A_UINT32 ctrl_flags);
extern void qcom_reconnect(A_UINT8 *bssid, A_UINT16 channel);
extern void qcom_wlan_disconnect();
extern void qcom_start_scan(int scanType, int forceFgScan);
extern void qcom_set_scan_params(int fgScan, A_UINT16 max_dwell_time, A_UINT16 pass_dwell_time);
extern void qcom_set_bss_filter(int filter);
extern void qcom_set_probed_ssid(int index, int flag, char *ssid, int ssidLength);
extern void qcom_set_listen_time(A_UINT16 time);
extern void qcom_set_ap_beacon_interval(A_UINT16 beacon_interval);
extern void qcom_rssi_get(A_UINT16 *rssi);
extern void qcom_hw_set_phy_mode(int phyMode);
extern void qcom_set_power_mode(int powerMode);
extern void qcom_wmi_setPowerParams_cmd(A_UINT16 idlePeriod,
                            A_UINT16 psPollNum, A_UINT16 dtimPolicy,
                            A_UINT16 tx_wakeup_policy, A_UINT16 num_tx_to_wakeup,
                            A_UINT16 ps_fail_event_policy);
extern void qcom_add_cipher_key(A_UINT8 keyIndex, CRYPTO_TYPE keyType, A_UINT8 keyUsage, A_UINT8 keyLength,
                    A_UINT8 * keyRSC, A_UINT8 * key, A_UINT8 key_op_ctrl, A_UINT8 * macAddr,
                    int sync_flag);
extern void qcom_add_wep_key(int keyIndx, char *key, int keyLength);
extern void qcom_wlan_set_tx_power(int dbM);
extern void qcom_wlan_set_roam_ctrl(A_UINT8 roamCtrlType, A_UINT8 *buf);
extern void qcom_sta_wlan_abort_scan(void);
extern void qcom_ap_hidden_ssid(A_UINT8 hidden_ssid);
extern void qcom_ap_set_num_sta(A_UINT8 num_sta);
extern void qcom_ap_config_commit(A_UINT8 networkType,
                      A_UINT8 dot11AuthMode,
                      A_UINT8 authMode,
                      A_UINT8 pairwiseCryptoType,
                      A_UINT8 pairwiseCryptoLen,
                      A_UINT8 groupCryptoType,
                      A_UINT8 groupCryptoLen,
                      A_UINT8 ssidLength, A_UCHAR * ssid, A_UINT16 channel, A_UINT8 * bssid,
                      A_UINT32 ctrl_flags);
extern void qcom_set_ap_country(char *country_code);
extern void qcom_allow_aggr(A_UINT16 tx_allow_aggr, A_UINT16 rx_allow_aggr);
extern void qcom_set_ht_cap(A_UINT8 band,
                A_UINT8 enable,
                A_UINT8 chan_width_40M_supported,
                A_UINT8 short_GI_20MHz,
                A_UINT8 short_GI_40MHz, A_UINT8 intolerance_40MHz, A_UINT8 max_ampdu_len_exp);
extern void qcom_set_pmk(A_UINT8 * pmk, A_UINT8 pmk_len);
extern void qcom_hw_set_channel(A_UINT16 channel);
extern void qcom_set_passphrase(A_UCHAR * ssid, A_UINT8 ssidLength, A_UINT8 * passphrase, A_UINT8 passLength);
extern void qcom_wlan_suspend_enable(void);
extern void qcom_wlan_suspend_start(A_UINT32 time);

extern void qcom_set_promiscuous_mode(A_UINT8 enable);
extern void qcom_switch_mode(A_UINT8 newOpMode, A_UINT8 newSubOpMode);
extern void qcom_dev_connected(A_UINT8 *state);

extern void qcom_sock_stack_init(void);
extern void qcom_dns_get_request(A_UINT32 mode, A_UINT32 domain, char *hostname);
extern void qcom_enable_dns_client(A_INT32 command);
extern void qcom_dns_server(ip_addr serv_ip, int mode);
extern void qcom_ip_config_request(A_UINT32 mode, A_UINT32 ipv4, A_UINT32 subnetMask, A_UINT32 gateway4);
extern void qcom_ipconfig_dhcp_pool(A_UINT32 startaddr, A_UINT32 endaddr, A_INT32 leasetime);
extern void qcom_ping_request(A_UINT32 ipv4_addr, A_UINT32 size);

extern void qcom_get_bp_info(bpool_info_t *bp_info);
extern void qcom_reg_read(A_UINT32 address, A_UINT32 *pvalue);
extern void qcom_reg_write(A_UINT32 address, A_UINT32 value);

extern void qcom_get_rssi_tmp(A_UINT8 * rssi);
extern void qcom_mac_read(A_UINT8 *pMac);
extern void qcom_reconnect_tmp(A_UINT8 * bssid, A_UINT16 channel);
extern void qcom_wlan_wakemgr_power_down(unsigned int wm_time_cn);

extern void qcom_mem_heap_get_free_size(A_UINT32 *psize);
extern int qcom_dhcp_clean_from_mac(void *hwaddr, A_UINT8 len);
extern void qcom_set_promiscuous_rx_cb_internel(A_UINT8 *promiscRxBuf, WLAN_PROMISCUOUS_CB cb);

extern void qcom_set_ap_inact_time(A_UINT16 inact_time);
#endif
