/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
#include "swat_wmiconfig_common.h"

// wmiconfig
A_INT32 cmd_wmi_config(A_INT32 argc, A_CHAR *argv[]);

// iwconfig scan
A_INT32 cmd_iwconfig_scan(A_INT32 argc, A_CHAR *argv[]);

// bench test
A_INT32 cmd_bench_tx_test(A_INT32 argc, A_CHAR *argv[]);
A_INT32 cmd_bench_rx_test(A_INT32 argc, A_CHAR *argv[]);
A_INT32 cmd_bench_quit(A_INT32 argc, A_CHAR *argv[]);

// ping
A_INT32 cmd_ping(A_INT32 argc, A_CHAR *argv[]);

// ota
#if defined(OTA_DEMO_EN)
A_INT32 cmd_ota_fw_upgrade(A_INT32 argc, A_CHAR *argv[]);
A_INT32 cmd_ota_daemon_start(A_INT32 argc, A_CHAR *argv[]);
A_INT32 cmd_ota_daemon_stop(A_INT32 argc, A_CHAR *argv[]);
#endif
int cmd_i2s_test(int argc, char *argv[]);
int cmd_i2c_test(int argc, char *argv[]);


console_cmd_t cust_cmds[] = {
#ifdef SWAT_PING
    {.name = (A_UCHAR *) "ping",
     .description = (A_UCHAR *) "ping",
     .execute = cmd_ping},
#endif

    /* Used to Auto Test */
#if defined(SWAT_WMICONFIG)
    {.name = (A_UCHAR *) "wmiconfig",
     .description = (A_UCHAR *) "wmiconfig cmd args",
     .execute = cmd_wmi_config},
    {.name = (A_UCHAR *) "iwconfig",
     .description = (A_UCHAR *) "scan for APs and specified SSID",
     .execute = cmd_iwconfig_scan},
#endif


#if 1
#if defined(SWAT_BENCH)
    {.name = (A_UCHAR *) "benchtx",
     .description = (A_UCHAR *) "run the traffic transmit test",
     .execute = cmd_bench_tx_test},
    {.name = (A_UCHAR *) "benchrx",
     .description = (A_UCHAR *) "run the traffic receive test",
     .execute = cmd_bench_rx_test},
    {.name = (A_UCHAR *) "benchquit",
     .description = (A_UCHAR *) "quit the bench test",
     .execute = cmd_bench_quit},
    /* {.name = (A_UCHAR *) "benchdbg", */
     /* .description = (A_UCHAR *) "dbg the bench test", */
     /* .execute = swat_benchdbg_handle}, */
#endif

#ifdef OTA_DEMO_EN
    {.name = (A_UCHAR *) "ota_fw_upgrade",
     .description = (A_UCHAR *) "ota_fw_upgrade <OTA-server-ip> <image-name> <protocol>",
     .execute = cmd_ota_fw_upgrade},
    {.name = (A_UCHAR *) "ota_upgrade_monitor_start",
     .description = (A_UCHAR *) "Start demo OTA deamon",
     .execute = cmd_ota_daemon_start},
    {.name = (A_UCHAR *) "ota_upgrade_monitor_stop",
     .description = (A_UCHAR *) "Stop demo OTA deamon",
     .execute = cmd_ota_daemon_stop},
#endif
#endif

#if 0
  {.name = (A_UCHAR *) "i2c",
     .description = (A_UCHAR *) "i2c read/write",
     .execute = cmd_i2c_test},
#endif

#ifdef I2S_DEMO_EN
  {.name = (A_UCHAR *) "i2s",
     .description = (A_UCHAR *) "i2s test",
     .execute = cmd_i2s_test},
#endif

};


A_INT32 cust_cmds_num = sizeof (cust_cmds) / sizeof (console_cmd_t);


#ifdef SWAT_PING
A_INT32 cmd_ping(A_INT32 argc, A_CHAR *argv[])
{
    return swat_ping_handle(argc, argv);
}
#endif

/* For Auto Test */
#if defined(SWAT_WMICONFIG)
A_INT32 cmd_wmi_config(A_INT32 argc, A_CHAR *argv[])
{
    return swat_wmiconfig_handle(argc, argv);
}

A_INT32 cmd_iwconfig_scan(A_INT32 argc, A_CHAR *argv[])
{
    return swat_iwconfig_scan_handle(argc, argv);
}
#endif

#if defined(SWAT_BENCH)
A_INT32 cmd_bench_tx_test(A_INT32 argc, A_CHAR *argv[])
{
    return swat_benchtx_handle(argc, argv);
}

A_INT32 cmd_bench_rx_test(A_INT32 argc, A_CHAR *argv[])
{
    return swat_benchrx_handle(argc, argv);
}

A_INT32 cmd_bench_quit(A_INT32 argc, A_CHAR *argv[])
{
     return swat_benchquit_handle(argc, argv);
}
#endif


#if defined(OTA_DEMO_EN)
A_INT32
cmd_ota_fw_upgrade(A_INT32 argc, A_CHAR *argv[])
{
    A_UINT32 ota_server_ip = 0;
    A_CHAR *file_name = NULL;
    A_UINT32 protocol = 0;

    if (argc < 4) {
        return -1;
    }

    ota_server_ip = _inet_addr(argv[1]);

    file_name = argv[2];

    protocol = atoi(argv[3]);

    extern A_INT32 qca_ota_upgrade(A_UINT32 ip_addr, A_CHAR *file_name, A_UINT32 protocol,
                               A_INT32 partition_index);
    return qca_ota_upgrade(ota_server_ip, file_name, protocol, -1);
}

A_INT32
cmd_ota_daemon_start(A_INT32 argc, A_CHAR *argv[])
{
    extern A_INT32 demo_ota_daemon_start(void);
    return demo_ota_daemon_start();
}

A_INT32
cmd_ota_daemon_stop(A_INT32 argc, A_CHAR *argv[])
{
    extern A_INT32 demo_ota_daemon_stop(void);
    demo_ota_daemon_stop();
}
#endif

