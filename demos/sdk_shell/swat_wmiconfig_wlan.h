/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#ifndef _SWAT_WMICONFIG_WLAN_H_
#define _SWAT_WMICONFIG_WLAN_H_

void swat_wmiconfig_country_code_get(A_CHAR *acountry_code);

void swat_wmiconfig_information();

void swat_wmiconfig_channel_set(A_UINT32 channel);

void swat_wmiconfig_dev_mode_set(A_CHAR* devModeName);

void swat_wmiconfig_tx_power_set(A_UINT32 txPower);

void swat_wmiconfig_wifi_mode_set(A_UINT8 wifiMode);

void swat_wmiconfig_11n_mode_enable(A_UINT32 flg);

void swat_wmiconfig_connect_disc();

void swat_wmiconfig_radio_onoff(A_UINT32 state);

void swat_wmiconfig_all_bss_scan();

void swat_wmiconfig_spec_bss_scan(A_CHAR* pSsid);

void swat_wmiconfig_connect_ssid(A_CHAR* pSsid);
void swat_wmiconfig_listen_time_set(A_UINT32 time);
void swat_wmiconfig_scan_ctl_set(A_UINT32 flag);
void swat_wmiconfig_scan_para_set(A_UINT16 max_dwell_time, A_UINT16 pass_dwell_time);

void swat_wmiconfig_bcon_int_set(A_UINT16 time_interval);

void swat_wmiconfig_country_code_set(char* country_code);

void swat_wmiconfig_inact_set(A_UINT16 inacttime);


void swat_wmiconfig_ap_hidden_set();

void swat_wmiconfig_rssi_get();

void swat_wmiconfig_devmode_get(A_UINT32 *wifiMode);


void swat_wmiconfig_allow_aggr(A_UINT16 tx_allow_aggr, A_UINT16 rx_allow_aggr);
#endif





