/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
#include "swat_wmiconfig_common.h"

/* WLAN API */
void
swat_wmiconfig_information()
{
    A_UINT8 macAddr[6];
    A_CHAR ssid[33];
    A_UINT32 powermode;
    A_UINT8 chipState;
    A_UINT8 phymode;
    A_UINT16 channel = 0;
    QCOM_WLAN_DEV_MODE devMode = QCOM_WLAN_DEV_MODE_INVALID;
    QCOM_WLAN_DEV_MODE devMode_show = QCOM_WLAN_DEV_MODE_INVALID;
    A_CHAR *modeDesc[] = { "Station", "AP", "Unknown" };
    /* bpool_info_t bp_info; */
    A_UINT32 dev_mode;

    A_MEMSET(&macAddr, 0, sizeof (macAddr));
    qcom_mac_get((A_UINT8 *) & macAddr);

    /* Mode */
    qcom_op_get_mode(&dev_mode);
    devMode = dev_mode;

    /* SSID */
    A_MEMSET(ssid, 0, 33);
    qcom_get_state(&chipState);
    /* if (((ap_conf.bApStart == 1) && (devMode == 0)) */
        /* || ((ap_conf.bApStart == 0) && (devMode == 1))) { */
        if (chipState == 4)     //K_WLAN_LINK_STATE_CONNECTED_STATE
        {
            qcom_get_ssid(ssid);
        }
        devMode_show = devMode;
    /* } else { */
        /* devMode_show = 1 - ap_conf.bApStart; */
    /* } */
    SWAT_PTF("SSID         =   %s\n", ssid);

    /* PHY MODE */
    qcom_get_phy_mode(&phymode);
    if (phymode == 1) {
        SWAT_PTF("Phy Mode     =   a\n");
    } else if (phymode == 2) {
        SWAT_PTF("Phy Mode     =   g\n");
    } else if (phymode == 4) {
        SWAT_PTF("Phy Mode     =   b\n");
    } else {
        SWAT_PTF("Phy Mode     =   mixed\n");
    }

    /* Power Mode */
    /* qcom_power_get_mode(&powermode); */
    if (powermode == 1)         //REC_POWER
    {
        SWAT_PTF("Power Mode   =  Power Save\n");
    } else if (powermode == 2)  //MAX_PERF_POWER
    {
        SWAT_PTF("Power Mode   =  Max Perf\n");
    } else {
        SWAT_PTF("Power Mode   =   Invalid\n");
    }
    SWAT_PTF("Mac Addr     =   %x:%x:%x:%x:%x:%x\n",
             macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
    SWAT_PTF("Mode         =   %s\n", modeDesc[devMode_show]);

    /* Channel */
    qcom_get_channel(&channel);
    if (channel >= 2412) {
        channel = (((channel - 2412) / 5) + 1);
    }
    SWAT_PTF("Channel      =   %d\n", channel);
    
    {
      A_UINT32 size;
      A_UINT8 mac_cnt;
      A_UINT8 mac_reap;
      A_UINT8 htc_cnt; 
      A_UINT8 htc_reap;
      A_UINT8 fw_cnt;
      A_UINT8 fw_reap;
      A_UINT8 free_cnt;
      A_UINT32 htc_get;
      A_UINT32 htc_put;
      
      qcom_free_heap_size_get(&size);
      qcom_buffer_info_get(&mac_cnt, &mac_reap, &htc_cnt, &htc_reap, &fw_cnt, 
                           &fw_reap, &free_cnt, &htc_get, &htc_put);
      
      SWAT_PTF("Heapsize     =   %d\n", size);
      SWAT_PTF("Buffer : Mac (%d+%d), HTC (%d+%d), FW (%d+%d), Free %d, tx (alloc %d, free %d)\n",  \
             mac_cnt, mac_reap, htc_cnt, htc_reap, fw_cnt, fw_reap, free_cnt, htc_get, htc_put);
    }
}

void 
swat_wmiconfig_country_code_get(A_CHAR *acountry_code)
{

    qcom_get_country_code(acountry_code);
}

void
swat_wmiconfig_channel_set(A_UINT32 channel)
{
    qcom_set_channel(channel);
}

void
swat_wmiconfig_tx_power_set(A_UINT32 txPower)
{
    /* dbm */
    qcom_set_tx_power(txPower);
}

void
swat_wmiconfig_wifi_mode_set(A_UINT8 wifiMode)
{
    qcom_set_phy_mode(wifiMode);
}

void
swat_wmiconfig_connect_disc()
{
    qcom_disconnect();
}

void
swat_wmiconfig_all_bss_scan()
{
    qcom_scan_all_bss_start();
}

void
swat_wmiconfig_spec_bss_scan(A_CHAR * pSsid)
{
    qcom_scan_bss_start(pSsid);
}

void
swat_wmiconfig_connect_ssid(A_CHAR * ssid)
{
    A_UINT32 devMode;

    qcom_op_get_mode(&devMode);
    if (QCOM_WLAN_DEV_MODE_AP == devMode) {
        qcom_ap_start(ssid);
    } else {
        qcom_sta_connect_with_scan(ssid);
    }

    SWAT_PTF("Try to Connected\r\n");
}



void
swat_wmiconfig_listen_time_set(A_UINT32 time)
{
    qcom_sta_set_listen_time(time);
}

void
swat_wmiconfig_scan_ctl_set(A_UINT32 flag)
{
    if (flag == 0 || flag == 1)
        qcom_scan_set_mode(flag);
        /* 1 = foreground */
        /* 0 = background */
    else {
        SWAT_PTF("Invalid flag. Should be 1 or 0\n");
    }
}

void 
swat_wmiconfig_scan_para_set(A_UINT16 max_dwell_time, A_UINT16 pass_dwell_time)
{
    qcom_scan_set_para(max_dwell_time, pass_dwell_time);
}

void
swat_wmiconfig_bcon_int_set(A_UINT16 time_interval)
{
    qcom_ap_set_beacon_interval(time_interval);
}

void
swat_wmiconfig_country_code_set(A_CHAR *country_code)
{
    qcom_set_country_code(country_code);
}

void
swat_wmiconfig_inact_set(A_UINT16 inacttime)
{
    qcom_ap_set_inact_time(inacttime);
}


void
swat_wmiconfig_ap_hidden_set()
{
    qcom_ap_hidden_mode_enable(1);
}



void
swat_wmiconfig_devmode_get(A_UINT32 * wifiMode)
{
    A_UINT32 mode;

    /* 0: sta */
    /* 1:ap */
    qcom_op_get_mode(&mode);

    if (mode == 1) {
        *wifiMode = 0;          //ap
    } else if (mode == 0) {
        *wifiMode = 1;          //station
    } else {
        *wifiMode = 2;          //unknown
    }
}

void
swat_wmiconfig_rssi_get()
{
    A_UINT8 rssi;
    A_UINT8 chipState;
    QCOM_WLAN_DEV_MODE devMode;

    qcom_op_get_mode(&devMode);
    if(devMode == QCOM_WLAN_DEV_MODE_STATION){
        qcom_get_state(&chipState);
        if(chipState == 4){
            qcom_sta_get_rssi(&rssi);
            SWAT_PTF("indicator = %d dB\n", rssi);
        }else{
            SWAT_PTF("Not associate\n");
        }
    }else{
        SWAT_PTF("RSSI not in AP mode\n");
    }
}

void
swat_wmiconfig_allow_aggr(A_UINT16 tx_allow_aggr, A_UINT16 rx_allow_aggr)
{
    qcom_allow_aggr_set_tid(tx_allow_aggr, rx_allow_aggr);
}
    
    
void swat_wmiconfig_dev_mode_set(A_CHAR * devModeName)
{
  QCOM_WLAN_DEV_MODE devMode;
  
  if (!swat_strcmp(devModeName, "ap"))
  {
    devMode = QCOM_WLAN_DEV_MODE_AP;
  }
  else if (!swat_strcmp(devModeName, "station"))
  {
    devMode = QCOM_WLAN_DEV_MODE_STATION;
  }
  else
  {
    SWAT_PTF("unkown mode\n");
    return;
  }
  
  qcom_op_set_mode(devMode);
  
}
