/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#ifndef _SWAT_PARSE_H_
#define _SWAT_PARSE_H_

//#include "swat_interface.h"
/*----------------------- Module Macro Define ------------------------*/
#define SWAT 1
#define SWAT_WMICONFIG 1
#define SWAT_WMICONFIG_SOFTAP 1
#define SWAT_WMICONFIG_WPS 1
#define SWAT_WMICONFIG_WEP 1
#define SWAT_WMICONFIG_WPA 1
#define SWAT_WMICONFIG_P2P 0

/*Network Part */
#define SWAT_WMICONFIG_IP 1
/*Misc or Other Part */
#define SWAT_WMICONFIG_MISC 1


#define SWAT_BENCH 1
#define SWAT_PING 1

#define SWAT_OK 1
#define SWAT_ERROR 2
#define SWAT_NOFOUND 0
/* Timer */
#define swat_time_get_ms    time_ms
#define swat_time_init      qcom_timer_init
#define swat_time_start     qcom_timer_start
#define swat_time_stop      qcom_timer_stop
#define swat_time_delete    qcom_timer_delete

/* Memory */
#define swat_mem_malloc     qcom_mem_alloc
#define swat_mem_cpy        A_MEMCPY
#define swat_mem_set        A_MEMSET
#define swat_mem_free       qcom_mem_free
/* String */
#define swat_atoi           atoi
#define swat_sscanf         A_SSCANF
#define swat_strcmp         A_STRCMP
#define swat_strcpy         A_STRCPY
/* Task */
#define swat_task_create    qcom_task_start
#define swat_task_delete    qcom_task_exit
/* TCP/IP */
#define swat_ping_request qcom_ping_request

/* Socket */
#define swat_accept         qcom_accept
#define swat_bind           qcom_bind
#define swat_close      	qcom_close
#define swat_connect        qcom_connect
#define swat_setsockopt     qcom_setsockopt
#define swat_getsockopt     qcom_getsockopt
#define swat_listen         qcom_listen
#define swat_recv           qcom_recv
#define swat_recvfrom       qcom_recvfrom
#define swat_send           qcom_send
#define swat_sendto         qcom_sendto
#define swat_socket         qcom_socket
#define swat_select         qcom_select
#define swat_setsockopt     qcom_setsockopt
#define swat_getsockopt     qcom_getsockopt
#define swat_fd_zero        FD_ZERO
#define swat_fd_set         FD_SET
#define swat_fd_isset       FD_ISSET


// to fix later, xiny
//#define A_ATOUL atoi
#define A_ATOUL

#if defined(SWAT_BENCH)
extern void swat_bench_quit_config();
extern void swat_bench_rx_test(A_UINT32 protocol, A_UINT32 port, A_UINT32 localIpAddress, A_UINT32 mcastIpAddress);
extern void swat_bench_tx_test(A_UINT32 ipAddress,
                                    A_UINT32 port,
                                    A_UINT32 protocol,
                                    A_UINT32 pktSize,
                                    A_UINT32 mode,
                                    A_UINT32 seconds,
                                    A_UINT32 numOfPkts,
                                    A_UINT32 delay);


extern A_INT32 swat_benchtx_handle(A_INT32 argc, A_CHAR *argv[]);
extern A_INT32 swat_benchrx_handle(A_INT32 argc, A_CHAR *argv[]);
extern A_INT32 swat_benchquit_handle(A_INT32 argc, A_CHAR *argv[]);
extern A_INT32 swat_benchdbg_handle(A_INT32 argc, A_CHAR *argv[]);
extern void swat_bench_dbg();

#endif

#if defined(SWAT_WMICONFIG)

extern A_INT32 swat_wmiconfig_handle(A_INT32 argc, A_CHAR *argv[]);
extern A_INT32 swat_wmiconfig_connect_handle(A_INT32 argc, A_CHAR *argv[]);

#if defined(SWAT_WMICONFIG_WEP)
extern A_INT32 swat_wmiconfig_wep_handle(A_INT32 argc, A_CHAR *argv[]);
#endif
#if defined(SWAT_WMICONFIG_WPA)
extern A_INT32 swat_wmiconfig_wpa_handle(A_INT32 argc, A_CHAR *argv[]);
#endif
#if defined(SWAT_WMICONFIG_WPS)
extern A_INT32 swat_wmiconfig_wps_handle(A_INT32 argc, A_CHAR *argv[]);
#endif
#if defined(SWAT_WMICONFIG_P2P)
extern A_INT32 swat_wmiconfig_p2p_handle(A_INT32 argc, A_CHAR *argv[]);
#endif
#if defined(SWAT_WMICONFIG_MISC)
extern A_INT32 swat_wmiconfig_misc_handle(A_INT32 argc, A_CHAR *argv[]);
#endif
#if defined(SWAT_WMICONFIG_SOFTAP)
extern A_INT32 swat_wmiconfig_softAp_handle(A_INT32 argc, A_CHAR *argv[]);
#endif
#if defined(SWAT_WMICONFIG_IP)
extern A_INT32 swat_wmiconfig_ip_handle(A_INT32 argc, A_CHAR *argv[]);
#endif
extern A_INT32 swat_iwconfig_scan_handle(A_INT32 argc, A_CHAR *argv[]);
#endif

#if defined(SWAT_PING)
extern A_INT32 swat_ping_handle(A_INT32 argc, A_CHAR *argv[]);
#endif

typedef struct _console_cmd {
    struct _console_cmd *next;
    A_UCHAR    *name;                  /* command name */
    A_UCHAR    *description;           /* optional description */
    A_INT32       (*execute)(A_INT32 argc, A_CHAR *argv[]);  /* execute callback */
    void      *context;               /* context to pass to callback */
} console_cmd_t;

void
swat_time();
#define SWAT_SW_VERSION "v0.109-throughput"

#define HTONS htons
#define HTONL htonl
#define NTOHL ntohl
#define NTOHS ntohs

#define ORG_PTR(fmt,args...) do{if(1) {cmnos_printf(fmt,##args);}}while(0)
#define SWAT_PTF(fmt,args...) do{if(1) {swat_time();qcom_printf(fmt,##args);}}while(0)
#define IPV4_PTF(addr) SWAT_PTF("%d.%d.%d.%d", (addr) >> 24 & 0xFF, (addr) >> 16 & 0xFF, (addr) >> 8 & 0xFF, (addr) & 0xFF)
#define IPV4_ORG_PTF(addr) ORG_PTR("%d.%d.%d.%d", (addr) >> 24 & 0xFF, (addr) >> 16 & 0xFF, (addr) >> 8 & 0xFF, (addr) & 0xFF)

/*Stack type definination*/
#define KF_ROM_1_1_2_INICHE 1
#define CALC_STREAM_NUMBER 4
#define CALC_STREAM_NUMBER_INVALID 0xFF


#endif

