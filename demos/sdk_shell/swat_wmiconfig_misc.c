/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
#include "swat_wmiconfig_common.h"

void
swat_wmiconfig_help()
{
#if 1
    SWAT_PTF("Usage: wmiconfig [<command>]\n");
    SWAT_PTF("Commands:\n");
    SWAT_PTF("    --version = Displays versions\n");
    SWAT_PTF("    --reset   = Reset board\n");
    SWAT_PTF("\n");

    SWAT_PTF("WPA Configuration:\n");
    SWAT_PTF("    --p <passphrase>\n");
    SWAT_PTF("    --wpa <ver> <ucipher> <mcipher>\n");
    SWAT_PTF("    --connect <ssid>\n");
    SWAT_PTF("            where  <p>      : Passphrase for WPA\n");
    SWAT_PTF("                   <ver>    : 1-WPA, 2-RSN\n");
    SWAT_PTF("                   <ucipher>: TKIP or CCMP\n");
    SWAT_PTF("                   <mcipher>: TKIP or CCMP\n");
    SWAT_PTF("                   <ssid>   : SSID of network\n");
    SWAT_PTF("\n");

    SWAT_PTF("WEP Configuration:\n");
    SWAT_PTF("    --wepkey <key_index> <key>\n");
    SWAT_PTF("    --wep <def_keyix> <mode>\n");
    SWAT_PTF("            where  <key_index>: Entered WEP key index\n");
    SWAT_PTF("                   <key>      : WEP key\n");
    SWAT_PTF("                   <def_keyix>: Default WEP key index\n");
    SWAT_PTF("                   <mode>     : open or shared\n");
    SWAT_PTF("\n");

    SWAT_PTF("WPS Configuration:\n");
    SWAT_PTF("    --wps <connect> <mode>\n");
    SWAT_PTF("            where  <connect>  : 0 - No Attempts to connect after wps\n");
    SWAT_PTF("                              : 1 - Attempts to connect after wps\n");
    SWAT_PTF("                   <mode>     : pin or push [pin:<=8 characters]\n");
    SWAT_PTF("\n");

    SWAT_PTF("    --disc       = Disconect from current AP\n");
    SWAT_PTF("    --wmode   <> = Set mode <b,g,n,ht40>\n");
    SWAT_PTF("    --pwrmode    <>   = set power mode 1=Power save, 0= Max Perf \n");
    SWAT_PTF("    --channel <> = Set channel hint 1-13\n");
    SWAT_PTF("    --listen <>     = Set listen interval\n");
    SWAT_PTF("    --mode <ap <hidden> <wps>| station>\n");
    SWAT_PTF
        ("    --scanctrl <0|1> <0|1> = Control firmware scan behavior.Disable/enable foreground OR background scanning\n");
    SWAT_PTF
        ("    --setscanpara <max_dwell_time <pass_dwell_time> = set the max dwell time(ms), pass dwell time(ms)\n");
    SWAT_PTF
        ("    --allow_aggr <tx_tid_mask> <rx_tid_mask> Enables aggregation based on the provided bit mask where each bit represents a TID valid TID's are 0-7\n");
    SWAT_PTF("\n");

    SWAT_PTF("AP Configuration:\n");
    SWAT_PTF("    --ap bconint <>   = set ap beacon interval\n");
    SWAT_PTF("    --ap country <>   = set ap country code\n");
	SWAT_PTF("    --ap inact <minutes>   = set ap inactive times\n");
    SWAT_PTF("\n");

    SWAT_PTF("DNS Client:\n");
    SWAT_PTF("    --ip_gethostbyname [<host name>]  = resolve hostname\n");
#ifdef IP_V6
    SWAT_PTF
        ("    --ip_resolve_hostname [<host name> <domain_type]  = resolve hostname for domain_type (ipv4 =2, ipv6 =3)\n");
    SWAT_PTF
        ("    --ip_gethostbyname2 [<host name> <domain_type]  = resolve hostname for domain_type (ipv4 =2 ipv6 =3)\n");
#endif
    SWAT_PTF("    --ip_dns_client [<start/stop>] 		= Start/Stop the DNS Client\n");
    SWAT_PTF("    --ip_dns_server_addr <ip addr> 		= Address of the DNS Server\n");
    SWAT_PTF
        ("    --ip_dns_delete_server_addr <ip addr>		   = Address of the DNS Server to be deleted \n");
    SWAT_PTF("\n");

    SWAT_PTF("IP Configuration:\n");
    SWAT_PTF("    --ipconfig = Show IP parameters\n");
    SWAT_PTF
        ("    --ipstatic <IP Address> <Subnet Mask> <Default Gateway> = Set static IP parameters\n");
    SWAT_PTF("    --ipdhcp = Run DHCP client\n");
    SWAT_PTF("    --ipdhcppool <start ipaddr> <End ipaddr> <Lease time> = set ip dhcp pool \n");

    SWAT_PTF("\n");

    SWAT_PTF("    --pmparams        = Set Power management parameters\n");
    SWAT_PTF("           --idle <time in ms>                        Idle period\n");
    SWAT_PTF("           --np   < >                                 PS Poll Number\n");
    SWAT_PTF("           --dp <1=Ignore 2=Normal 3=Stick 4=Auto>    DTIM Policy\n");
    SWAT_PTF("           --txwp <1=wake 2=do not wake>              Tx Wakeup Policy\n");
    SWAT_PTF("           --ntxw < >                                 Number of TX to Wakeup\n");
    SWAT_PTF("           --psfp <1=send fail event 2=Ignore event>  PS Fail Event Policy\n");

    SWAT_PTF("    --pwrmode    <>   = set power mode 1=Power save, 0= Max Perf \n");
    SWAT_PTF("    --rssi       <>   = prints Link Quality (SNR) \n");
    SWAT_PTF("    --settxpower <> = Set transmit power 1-17 dbM\n");
    SWAT_PTF("    --suspend        = Enable device suspend mode(before connect command)\n");
    SWAT_PTF("    --suspstart <time>    = Suspend device for specified time in milliseconds\n");

    SWAT_PTF("    --driver [mode]     \n");
    SWAT_PTF("					mode = down - Sets the driver State 0 unloads the driver \n");
    SWAT_PTF("					       up   - Reloads the driver\n");
	SWAT_PTF("    --promisc [enable] \n");
    SWAT_PTF("					enable = 1 - Enable wlan promiscurous test \n");
    SWAT_PTF("					         0 - Disable wlan promiscusour test\n");	
    SWAT_PTF("wmiconfig --test spi_flash read <cs_pin> <addr(DEC)>\n");
    SWAT_PTF("wmiconfig --wdt <state> <timeout>\n\tstate:<0>disable <1>1000ms timer feed <2>cust feed\n");
    SWAT_PTF("wmiconfig --i2c read <addr(DEC)>\nwmiconfig --psram write <addr(DEC)> <val(DEC)>\n");
    SWAT_PTF("wmiconfig --psram read <addr(DEC)>\nwmiconfig --psram write <addr(DEC)> <val(DEC)>\n");
    SWAT_PTF("wmiconfig --hostname get\nwmiconfig --hostname set <name> \n");

#endif
}

void
swat_wmiconfig_version(A_CHAR * pVersion)
{
    SWAT_PTF("Host version : Hostless\n");
    SWAT_PTF("Target version    : QCM\n");
    SWAT_PTF("Firmware version  : %s\n", pVersion);
    SWAT_PTF("Interface version : EBS\n");
    SWAT_PTF(" - built on %s %s\n", __DATE__, __TIME__);
}

void
swat_wmiconfig_reset()
{
    qcom_sys_reset();
}

void
swat_wmiconfig_promiscuous_test(A_UINT8 promiscuous)
{
    qcom_promiscuous_enable(promiscuous);
}

