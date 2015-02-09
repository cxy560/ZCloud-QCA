/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
#include "swat_parse.h"
#include "qcom/tx_alloc_api.h"
#include "swat_bench_core.h"
#include "qcom/socket_api.h"
#include "qcom/select_api.h"

extern void qcom_thread_msleep(unsigned long ms);
extern int qcom_task_start(void (*fn) (unsigned int), unsigned int arg, int stk_size, int tk_ms);
extern void qcom_task_exit();

#if defined(KF_ROM_1_1_2_INICHE)


static qcom_timer_t benchTcpTxTimer[CALC_STREAM_NUMBER];
static qcom_timer_t benchUdpTxTimer[CALC_STREAM_NUMBER];
static A_INT32 benchTcpTxTimerStop[CALC_STREAM_NUMBER];
static A_INT32 benchUdpTxTimerStop[CALC_STREAM_NUMBER];

static void
swat_bench_tcp_timer_handler(unsigned int alarm, void *data)
{
    benchTcpTxTimerStop[*(A_UINT32 *) data] = 1;
}

static void
swat_bench_udp_timer_handler(unsigned int alarm, void *data)
{
    benchUdpTxTimerStop[*(A_UINT32 *) data] = 1;
}

void
swat_bench_tcp_timer_clean(A_UINT32 index)
{
    swat_time_stop(&benchTcpTxTimer[index]);
    swat_time_delete(&benchTcpTxTimer[index]);
}

void
swat_bench_udp_timer_clean(A_UINT32 index)
{
    swat_time_stop(&benchUdpTxTimer[index]);
    swat_time_delete(&benchUdpTxTimer[index]);
}

void
swat_bench_timer_init(A_UINT32 seconds, A_UINT32 protocol, A_UINT32 index)
{
    if (TEST_PROT_TCP == protocol) {
        benchTcpTxTimerStop[index] = 0;
        benchTcpTxTimer[index].arg = index;
        swat_time_init(&benchTcpTxTimer[index], swat_bench_tcp_timer_handler,
                       (void *) &(benchTcpTxTimer[index].arg), seconds * 1000, ONESHOT);
        swat_time_start(&benchTcpTxTimer[index]);
    }
    if (TEST_PROT_UDP == protocol) {
        benchUdpTxTimerStop[index] = 0;
        benchUdpTxTimer[index].arg = index;
        swat_time_init(&benchUdpTxTimer[index], swat_bench_udp_timer_handler,
                       (void *) &(benchUdpTxTimer[index].arg), seconds * 1000, ONESHOT);
        swat_time_start(&benchUdpTxTimer[index]);
    }
}

void
swat_tcp_tx_handle(STREAM_CXT_t * pStreamCxt)
{
    A_INT32 sendBytes = 0;
    A_UINT32 sumBytes = 0;
    A_UINT32 currentPackets = 0;
    A_UINT8 *pDataBuffer = NULL;

    SWAT_PTR_NULL_CHK(pStreamCxt);

    /* Initial Bench Value */
    swat_bench_quit_init();

    /* Initial Calc & Time */
    pStreamCxt->calc.firstTime.milliseconds = CALC_TIME_DEF;
    pStreamCxt->calc.lastTime.milliseconds = CALC_TIME_DEF;
    pStreamCxt->calc.bytes = CALC_BYTES_DEF;
    pStreamCxt->calc.kbytes = CALC_KBYTES_DEF;

    /* Malloc Packet Buffer Size */
    pDataBuffer = swat_mem_malloc(pStreamCxt->param.pktSize);
    if (NULL == pDataBuffer) {
        SWAT_PTF("TCP TX data buffer malloc error\r\n");
        /* Close Socket */
        swat_socket_close(pStreamCxt);
        return;
    }
    pStreamCxt->param.pktBuff = pDataBuffer;

    /* Initial Packet */
    SWAT_PTF("Sending...\r\n");

    /* Get First Time */
    swat_get_first_time(pStreamCxt);
    if (TEST_MODE_TIME == pStreamCxt->param.mode) {
        swat_bench_timer_init(pStreamCxt->param.seconds, pStreamCxt->param.protocol,
                              pStreamCxt->index);
    }

    while (1) {
        if (swat_bench_quit()) {
            /* Get Last Time For Pressure */
            //SWAT_PTF("Warning Bench Quit!!\n");
            swat_get_last_time(pStreamCxt);
            break;
        }

        sendBytes =
            swat_send(pStreamCxt->socketLocal, (char *) pDataBuffer,
                      pStreamCxt->param.pktSize, 0);

		/*delay */
		if (pStreamCxt->param.delay){
			qcom_thread_msleep(pStreamCxt->param.delay);
		}

        if (sendBytes < 0) {
            if (TX_BUFF_FAIL == sendBytes) {
                /* buff full will send fail, it should be normal */
                SWAT_PTF("[bench id %d, port %d]buffer full\r\n", pStreamCxt->index,
                         NTOHS(pStreamCxt->param.port));
            } else {
                SWAT_PTF
                    ("[bench id %d, port %d]TCP Socket send is error %d sumBytes = %d\r\n",
                     pStreamCxt->index, NTOHS(pStreamCxt->param.port), sendBytes, sumBytes);

				swat_get_last_time(pStreamCxt);
                break;
            }
        } else {
            /* bytes & kbytes */
            sumBytes += sendBytes;
        }

		if (sendBytes >0){
			swat_bytes_calc(pStreamCxt, sendBytes);
		}

        /* Packets Mode */
        if (TEST_MODE_PACKETS == pStreamCxt->param.mode) {
            if (0 != (sumBytes / pStreamCxt->param.pktSize)) {
                currentPackets += (sumBytes / pStreamCxt->param.pktSize);
                sumBytes = sumBytes % pStreamCxt->param.pktSize;
            }

            if (currentPackets >= pStreamCxt->param.numOfPkts) {
				swat_get_last_time(pStreamCxt);
                break;
            }
        }

        /* Time Mode */
        if (TEST_MODE_TIME == pStreamCxt->param.mode) {

            if (0 != benchTcpTxTimerStop[pStreamCxt->index]) {
                swat_get_last_time(pStreamCxt);
                break;
            }
        }
    }

	swat_test_result_print(pStreamCxt);

    SWAT_PTF("IOT Throughput Test Completed.\n");

    /* Free Buffer */
    swat_buffer_free(&(pStreamCxt->param.pktBuff));

    /* Close Socket */
    swat_socket_close(pStreamCxt);
}

void
swat_tcp_rx_data(STREAM_CXT_t * pStreamCxt)
{
    A_INT32 recvBytes = 0;
    A_UINT8 *pDataBuffer = NULL;
    struct timeval tmo;
    A_INT32 fdAct = 0;
    q_fd_set sockSet;

    SWAT_PTR_NULL_CHK(pStreamCxt);
    /* Initial pBuffer */
    /* Malloc Packet Buffer Size */
    pDataBuffer = swat_mem_malloc(pStreamCxt->param.pktSize);
    if (NULL == pDataBuffer) {
        SWAT_PTF("TCP RX data buffer malloc error\r\n");
        return;
    }
    pStreamCxt->param.pktBuff = pDataBuffer;

    /* Initial Packet */
    swat_mem_set(pDataBuffer, 0, sizeof (A_UINT8) * pStreamCxt->param.pktSize);

    /* Get First Time */
    swat_get_first_time(pStreamCxt);
    swat_fd_zero(&sockSet);
    swat_fd_set(pStreamCxt->clientFd, &sockSet);
    pStreamCxt->pfd_set = (void *) &sockSet;
    tmo.tv_sec	= 300;
    tmo.tv_usec = 0;
    while (1) {

        if (swat_bench_quit()) {
            /* Get Last Time For Pressure */
            //SWAT_PTF("Bench quit!!\r\n");
            swat_get_last_time(pStreamCxt);
            break;
        }

        /* Wait for Input */
        fdAct = swat_select(pStreamCxt->clientFd + 1, &sockSet, NULL, NULL, &tmo);  //k_select()
        if (0 != fdAct) {
            if (swat_fd_isset(pStreamCxt->clientFd, &sockSet)) {
                recvBytes =
                    swat_recv(pStreamCxt->clientFd, (char *) pDataBuffer, pStreamCxt->param.pktSize,
                              0);
                if (recvBytes <= 0) {
                    swat_get_last_time(pStreamCxt);
                    break;
                } else {
                    swat_bytes_calc(pStreamCxt, recvBytes);
                }
            }
        }
		else{
			SWAT_PTF("TCP Rx idle time is over 60s and exit now\n");
			swat_get_last_time(pStreamCxt);
			break;
		}
    }
    swat_test_result_print(pStreamCxt);

    SWAT_PTF("Waiting.\n");

    /* Free Buffer */
    swat_buffer_free(&(pStreamCxt->param.pktBuff));
    swat_fd_zero(&sockSet);
    pStreamCxt->pfd_set = NULL;
}

void
swat_tcp_rx_handle(STREAM_CXT_t * pStreamCxt)
{
    A_INT32 ret = 0;
    A_UINT32 clientIp;
    A_UINT16 clientPort;
    A_INT32 clientFd = -1;
    A_UINT32 isFirst = 0;
    struct sockaddr_in clientAddr;
    A_INT32 len = sizeof (struct sockaddr_in);
    struct timeval tmo;
    A_INT32 fdAct = 0;
    q_fd_set sockSet;

    SWAT_PTR_NULL_CHK(pStreamCxt);

    /* Initial Bench Value */
    swat_bench_quit_init();

    swat_fd_zero(&sockSet);
    swat_fd_set(pStreamCxt->socketLocal, &sockSet);
    pStreamCxt->pfd_set = (void *) &sockSet;
    tmo.tv_sec = 2;
    tmo.tv_usec = 0;
    while (1) {
        if (0 == isFirst) {
            ret = swat_listen(pStreamCxt->socketLocal, 10);
            if (ret < 0) {
                /* Close Socket */
                SWAT_PTF("Failed to listen socket %d.\n", pStreamCxt->socketLocal);
                goto QUIT;
            }
            isFirst = 1;
        }
        while (1) {
            if (swat_bench_quit()) {
                goto QUIT;
            }
            fdAct = swat_select(pStreamCxt->socketLocal, &sockSet, NULL, NULL, &tmo);   //k_select()
            if (fdAct != 0) {
                break;
            }
        }
        clientFd = swat_accept(pStreamCxt->socketLocal, (struct sockaddr *) &clientAddr, &len);
        if (clientFd < 0) {
            /* Close Socket */
            SWAT_PTF("Failed to accept socket %d.\n", clientFd);
            goto QUIT;
        }

        clientIp = NTOHL(clientAddr.sin_addr.s_addr);
        clientPort = NTOHS(clientAddr.sin_port);
        pStreamCxt->clientFd = clientFd;
        SWAT_PTF("Receiving from %d.%d.%d.%d Remote port:%d \r\n",
                 (clientIp) >> 24 & 0xFF, (clientIp) >> 16 & 0xFF,
                 (clientIp) >> 8 & 0xFF, (clientIp) & 0xFF, clientPort);

        /* Initial Calc & Time */
        pStreamCxt->calc.firstTime.milliseconds = CALC_TIME_DEF;
        pStreamCxt->calc.lastTime.milliseconds = CALC_TIME_DEF;
        pStreamCxt->calc.bytes = CALC_BYTES_DEF;
        pStreamCxt->calc.kbytes = CALC_KBYTES_DEF;

        swat_tcp_rx_data(pStreamCxt);
        /* Close Client Socket */
        swat_close(pStreamCxt->clientFd);

        pStreamCxt->clientFd = -1;
    }
  QUIT:
    swat_test_result_print(pStreamCxt);
    SWAT_PTF("*************IOT Throughput Test Completed **************\n");
    SWAT_PTF("Shell> ");
    /* Init fd_set */
    swat_fd_zero(&sockSet);
    pStreamCxt->pfd_set = NULL;
    /* Close Socket */
    swat_socket_close(pStreamCxt);
}

void
swat_udp_tx_handle(STREAM_CXT_t * pStreamCxt)
{
    A_INT32 sendBytes = 0;
    A_INT32 fromSize = 0;
    A_UINT32 sumBytes = 0;
    A_UINT32 currentPackets = 0;
    A_UINT8 *pDataBuffer = NULL;
    struct sockaddr_in remoteAddr;
    struct sockaddr_in fromAddr;
    EOT_PACKET_t eotPacket;

    A_INT32 sendTerminalCount = 0;

    SWAT_PTR_NULL_CHK(pStreamCxt);

    /* Initial Bench Value */
    swat_bench_quit_init();

    /* Initial Calc & Time */
    pStreamCxt->calc.firstTime.milliseconds = CALC_TIME_DEF;
    pStreamCxt->calc.lastTime.milliseconds = CALC_TIME_DEF;
    pStreamCxt->calc.bytes = CALC_BYTES_DEF;
    pStreamCxt->calc.kbytes = CALC_KBYTES_DEF;

    /* Malloc Packet Buffer Size */
    pDataBuffer = swat_mem_malloc(pStreamCxt->param.pktSize);
    if (NULL == pDataBuffer) {
        SWAT_PTF("UDP TX data buffer malloc error\r\n");
        /* Close Socket */
        swat_socket_close(pStreamCxt);
        return;
    }
    pStreamCxt->param.pktBuff = pDataBuffer;
    /* Prepare IP address & port */
    memset(&remoteAddr, 0, sizeof (struct sockaddr_in));
    memset(&fromAddr, 0, sizeof (struct sockaddr_in));

    remoteAddr.sin_addr.s_addr = HTONL(pStreamCxt->param.ipAddress);
    remoteAddr.sin_port = HTONS(pStreamCxt->param.port);
    remoteAddr.sin_family = AF_INET;

    fromSize = sizeof (struct sockaddr_in);

    /* Initial Packet */
    SWAT_PTF("UDP IP %d.%d.%d.%d prepare OK\r\n",
             (pStreamCxt->param.ipAddress) >> 24 & 0xFF, (pStreamCxt->param.ipAddress) >> 16 & 0xFF,
             (pStreamCxt->param.ipAddress) >> 8 & 0xFF, (pStreamCxt->param.ipAddress) & 0xFF);
    SWAT_PTF("Sending...\r\n");

    /* Get First Time */
    swat_get_first_time(pStreamCxt);
    if (TEST_MODE_TIME == pStreamCxt->param.mode) {
        swat_bench_timer_init(pStreamCxt->param.seconds, pStreamCxt->param.protocol,
                              pStreamCxt->index);
    }

    while (1) {
        if (swat_bench_quit()) {
            /* Get Last Time For Pressure */
            //SWAT_PTF("Bench quit!!\r\n");
            swat_get_last_time(pStreamCxt);
            break;
        }

        sendBytes = swat_sendto(pStreamCxt->socketLocal, (char*) pDataBuffer,
                                pStreamCxt->param.pktSize, 0,
                                (struct sockaddr *) &remoteAddr, sizeof (struct sockaddr_in));

		/*delay */
		if (pStreamCxt->param.delay){
			qcom_thread_msleep(pStreamCxt->param.delay);
		}

        if (sendBytes < 0) {
            SWAT_PTF("UDP Socket send is error %d sumBytes = %d\r\n", sendBytes, sumBytes);
            /* Free Buffer */
            //swat_buffer_free(&(pStreamCxt->param.pktBuff));
            qcom_thread_msleep(100);
            //break;
        } else {
            /* bytes & kbytes */
            sumBytes += sendBytes;
        }

		if (sendBytes > 0){
			swat_bytes_calc(pStreamCxt, sendBytes);
        }

        /* Packets Mode */
        if (TEST_MODE_PACKETS == pStreamCxt->param.mode) {
            if (0 != (sumBytes / pStreamCxt->param.pktSize)) {
                currentPackets += (sumBytes / pStreamCxt->param.pktSize);
                sumBytes = sumBytes % pStreamCxt->param.pktSize;
            }

            if (currentPackets >= pStreamCxt->param.numOfPkts) {

				swat_get_last_time(pStreamCxt);
                break;
            }
        }

        /* Time Mode */
        if (TEST_MODE_TIME == pStreamCxt->param.mode) {

			/* Get Last Time */
            if (0 != benchUdpTxTimerStop[pStreamCxt->index]) {
                swat_get_last_time(pStreamCxt);
                break;
            }
        }
    }


    int send_fail = 0;
    /* Tell ath_console TX is complete end mark is AABBCCDD */
    while (send_fail <= 10) {
        eotPacket.code = 0xAABBCCDD;
        eotPacket.packetCount = currentPackets;

        sendBytes = swat_sendto(pStreamCxt->socketLocal, (char *) & eotPacket,
                                sizeof (EOT_PACKET_t), 0, (struct sockaddr *) &remoteAddr,
                                sizeof (struct sockaddr_in));
        if (sendBytes < 0) {
            SWAT_PTF("UDP send terminate packet error %d , retry %d \r\n", sendBytes,
                     sendTerminalCount);

            send_fail ++;
            qcom_thread_msleep(100);
        } else {
            qcom_thread_msleep(100);

            sendTerminalCount++;
            if(sendTerminalCount > 2)
                break;
        }

    }

	swat_test_result_print(pStreamCxt);
    SWAT_PTF("*************IOT Throughput Test Completed **************\n");

    /* Free Buffer */
    swat_buffer_free(&(pStreamCxt->param.pktBuff));

    /* Close Socket */
    swat_socket_close(pStreamCxt);
}

void
swat_udp_rx_data(STREAM_CXT_t * pStreamCxt)
{
    A_INT32 recvBytes = 0;
    A_INT32 fromSize = 0;
    A_UINT32 sumBytes = 0;
    A_UINT8 *pDataBuffer = NULL;
    struct sockaddr_in fromAddr;
    q_fd_set sockSet;
    struct timeval tmo;
    A_INT32 fdAct = 0;
    A_UINT32 isFirst = 1;
    A_UINT32 clientIp = 0;
    A_UINT16 clientPort = 0;
    A_UINT32 totalInterval = 0;
    A_UINT32 sendBytes = 0;
    STAT_PACKET_t StatPacket;

    A_INT32 sendTerminalCount = 0;

    SWAT_PTR_NULL_CHK(pStreamCxt);

    /* Initial Calc & Time */
    pStreamCxt->calc.firstTime.milliseconds = CALC_TIME_DEF;
    pStreamCxt->calc.lastTime.milliseconds = CALC_TIME_DEF;
    pStreamCxt->calc.bytes = CALC_BYTES_DEF;
    pStreamCxt->calc.kbytes = CALC_KBYTES_DEF;

    /* Initial Bench Value */
    swat_bench_quit_init();

    /* Malloc Packet Buffer Size */
    pDataBuffer = swat_mem_malloc(pStreamCxt->param.pktSize);
    if (NULL == pDataBuffer) {
        SWAT_PTF("UDP RX data buffer malloc error\r\n");
        return;
    }
    pStreamCxt->param.pktBuff = pDataBuffer;

    memset(&StatPacket, 0, sizeof (StatPacket));

    /* Prepare IP address & port */
    memset(&fromAddr, 0, sizeof (struct sockaddr_in));
    fromSize = sizeof (struct sockaddr_in);

    /* Init fd_set */
    swat_fd_zero(&sockSet);
    swat_fd_set(pStreamCxt->socketLocal, &sockSet);
    pStreamCxt->pfd_set = (void *) &sockSet;
    tmo.tv_sec = 300;
    tmo.tv_usec = 0;

    /* Get First Time */
    swat_get_first_time(pStreamCxt);

    while (1) {
        if (swat_bench_quit()) {
            /* Get Last Time For Pressure */
            //SWAT_PTF("Bench quit!!\r\n");
            swat_get_last_time(pStreamCxt);

            //swat_bytes_calc(pStreamCxt, sumBytes);
            break;
        }

        /* Wait for Input */
        fdAct = swat_select(pStreamCxt->socketLocal + 1, &sockSet, NULL, NULL, &tmo);   //k_select()
        if (0 != fdAct) {
            if (swat_fd_isset(pStreamCxt->socketLocal, &sockSet)) {
                recvBytes = swat_recvfrom(pStreamCxt->socketLocal, (char*) pDataBuffer,
                                          pStreamCxt->param.pktSize, 0,
                                          (struct sockaddr *) &fromAddr, &fromSize);
                if (recvBytes <= 0) {
                    SWAT_PTF("UDP Socket receive is error %d, sumBytes = %d\r\n", recvBytes,
                             sumBytes);
                    break;
                }

                if (recvBytes >= sizeof (EOT_PACKET_t)) {
                    if (isFirst) {
                        if (recvBytes > sizeof (EOT_PACKET_t)) {
                            clientIp = NTOHL(fromAddr.sin_addr.s_addr);
                            clientPort = NTOHS(fromAddr.sin_port);

                            SWAT_PTF("UDP receving from %d.%d.%d.%d port:%d \r\n",
                                     (clientIp) >> 24 & 0xFF, (clientIp) >> 16 & 0xFF,
                                     (clientIp) >> 8 & 0xFF, (clientIp) & 0xFF, clientPort);
                            isFirst = 0;
                            swat_get_first_time(pStreamCxt);
                        }
                        swat_bytes_calc(pStreamCxt, recvBytes);
                    } else {

                    	/*End packet is not count*/
                    	if (recvBytes > sizeof (EOT_PACKET_t)) {
							swat_bytes_calc(pStreamCxt, recvBytes);
                    	}
						else {

							/* Update Port */
							fromAddr.sin_port = HTONS(pStreamCxt->param.port);
							fromAddr.sin_family = AF_INET;	/* End Of Transfer */

                            swat_get_last_time(pStreamCxt);

                            totalInterval =
                                (pStreamCxt->calc.lastTime.milliseconds -
                                 pStreamCxt->calc.firstTime.milliseconds);
                            StatPacket.kbytes = pStreamCxt->calc.kbytes;
                            StatPacket.bytes = pStreamCxt->calc.bytes;
                            StatPacket.msec = totalInterval;
                            /* Tell ath_console TX received end mark AABBCCDD with throughput value */
                            while (sendTerminalCount <= 10) {
                                /* Wait for Output */
                                fdAct = swat_select(pStreamCxt->socketLocal + 1, NULL, &sockSet, NULL, &tmo);   //k_select()
                                if (0 != fdAct) {
                                    if (swat_fd_isset(pStreamCxt->socketLocal, &sockSet)) {
                                        sendBytes = swat_sendto(pStreamCxt->socketLocal,
                                                                (char *) (&StatPacket),
                                                                sizeof (STAT_PACKET_t), 0,
                                                                (struct sockaddr *) &fromAddr,
                                                                sizeof (struct sockaddr_in));
                                        if (sendBytes < 0) {
                                            SWAT_PTF
                                                ("UDP send throughput info packet error %d , retry %d \r\n",
                                                 sendBytes, sendTerminalCount);
                                            qcom_thread_msleep(100);
                                        } else {
                                            /* Clean */
                                            tmo.tv_sec = 2;
                                            tmo.tv_usec = 0;
                                            while (recvBytes == sizeof (EOT_PACKET_t)) {
                                                fdAct = swat_select(pStreamCxt->socketLocal + 1, &sockSet, NULL, NULL, &tmo);   //k_select()
                                                if (0 == fdAct) {
                                                    SWAT_PTF("UDP break\n");
                                                    break;
                                                }

                                                if (swat_fd_isset(pStreamCxt->socketLocal, &sockSet)) {
                                                    recvBytes = swat_recvfrom(pStreamCxt->socketLocal, (char*) pDataBuffer,
                                                                              pStreamCxt->param.pktSize, 0,
                                                                              (struct sockaddr *) &fromAddr, &fromSize);
                                                }
                                            }
                                            break;
                                        }
                                    }
                                }
                                sendTerminalCount++;
                            }
                            break;
                        }
                    }
                }
            }
        }
		else{
			SWAT_PTF("UDP Rx idle time is over 60s and exit now\n");
            swat_get_last_time(pStreamCxt);
			break;
		}
    }

    swat_test_result_print(pStreamCxt);

    SWAT_PTF("Waiting.\n");
    /* Free Buffer */
    swat_buffer_free(&(pStreamCxt->param.pktBuff));

}

void
swat_udp_rx_handle(STREAM_CXT_t * pStreamCxt)
{
    q_fd_set sockSet;
    struct timeval tmo;
    A_INT32 fdAct = 0;

    /* Initial Bench Value */
    swat_bench_quit_init();

    /* Init fd_set */
    swat_fd_zero(&sockSet);
    swat_fd_set(pStreamCxt->socketLocal, &sockSet);
    tmo.tv_sec = 2;
    tmo.tv_usec = 0;
    pStreamCxt->pfd_set = (void *) &sockSet;

    while (1) {
        while (1) {
            if (swat_bench_quit()) {
                goto QUIT;
            }
            fdAct = swat_select(pStreamCxt->socketLocal, &sockSet, NULL, NULL, &tmo);   //k_select()
            if (fdAct != 0) {
                break;
            }
        }
        swat_udp_rx_data(pStreamCxt);
    }
  QUIT:
    swat_test_result_print(pStreamCxt);
    SWAT_PTF("*************IOT Throughput Test Completed **************\n");
    SWAT_PTF("Shell> ");
    swat_fd_zero(&sockSet);
    pStreamCxt->pfd_set = NULL;
    /* Close Socket */
    swat_socket_close(pStreamCxt);
}

void
swat_bench_tcp_tx_task(STREAM_CXT_t * pStreamCxt)
{
    A_INT32 ret = 0;
    struct sockaddr_in remoteAddr;

    SWAT_PTR_NULL_CHK(pStreamCxt);

    pStreamCxt->socketLocal = swat_socket(AF_INET, SOCK_STREAM, 0);
    if (pStreamCxt->socketLocal < 0) {
        SWAT_PTF("Open socket error...\r\n");
        goto QUIT;
    }

    /* Connect Socket */
    swat_mem_set(&remoteAddr, 0, sizeof (struct sockaddr_in));
    remoteAddr.sin_addr.s_addr = HTONL(pStreamCxt->param.ipAddress);
    remoteAddr.sin_port = HTONS(pStreamCxt->param.port);
    remoteAddr.sin_family = AF_INET;
    SWAT_PTF("Connecting from socket %d.\n", pStreamCxt->socketLocal);
    ret =
        swat_connect(pStreamCxt->socketLocal, (struct sockaddr *) &remoteAddr,
                     sizeof (struct  sockaddr_in));
    if (ret < 0) {
        /* Close Socket */
        SWAT_PTF("Connect %lu.%lu.%lu.%lu:%d Failed\r\n",
                 (remoteAddr.sin_addr.s_addr) & 0xFF, (remoteAddr.sin_addr.s_addr) >> 8 & 0xFF,
                 (remoteAddr.sin_addr.s_addr) >> 16 & 0xF,
                 (remoteAddr.sin_addr.s_addr) >> 24 & 0xFF, remoteAddr.sin_port);
        swat_socket_close(pStreamCxt);
        goto QUIT;
    }

    SWAT_PTF("Connect %lu.%lu.%lu.%lu OK\r\n",
             (remoteAddr.sin_addr.s_addr) & 0xFF, (remoteAddr.sin_addr.s_addr) >> 8 & 0xFF,
             (remoteAddr.sin_addr.s_addr) >> 16 & 0xF, (remoteAddr.sin_addr.s_addr) >> 24 & 0xFF);

    /* Packet Handle */
    swat_tcp_tx_handle(pStreamCxt);
  QUIT:
    /* Free Index */
    swat_cxt_index_free(&tcpTxIndex[pStreamCxt->index]);
    swat_bench_tcp_timer_clean(pStreamCxt->index);
    swat_task_delete();
}

void
swat_bench_tcp_rx_task(STREAM_CXT_t * pStreamCxt)
{
    A_INT32 ret = 0;
    struct sockaddr_in remoteAddr;

    SWAT_PTR_NULL_CHK(pStreamCxt);

    pStreamCxt->socketLocal = swat_socket(AF_INET, SOCK_STREAM, 0);
    if (pStreamCxt->socketLocal < 0) {
        SWAT_PTF("Open socket error...\r\n");
        goto QUIT;
    }

    /* Connect Socket */
    swat_mem_set(&remoteAddr, 0, sizeof (struct sockaddr_in));
    remoteAddr.sin_addr.s_addr = HTONL(pStreamCxt->param.ipAddress);
    remoteAddr.sin_port = HTONS(pStreamCxt->param.port);
    remoteAddr.sin_family = AF_INET;

    SWAT_PTF("****************************************************\n");
    SWAT_PTF(" TCP RX Test\n");
    SWAT_PTF("****************************************************\n");
    SWAT_PTF("Local port %d\n", pStreamCxt->param.port);
    SWAT_PTF("Type benchquit to terminate test\n");
    SWAT_PTF("****************************************************\n");

    ret =
        swat_bind(pStreamCxt->socketLocal, (struct sockaddr *) &remoteAddr,
                  sizeof (struct sockaddr_in));
    if (ret < 0) {
        /* Close Socket */
        SWAT_PTF("Failed to bind socket %d.\n", pStreamCxt->socketLocal);
        swat_socket_close(pStreamCxt);
        goto QUIT;
    }

    SWAT_PTF("Waiting.\n");

    /* Packet Handle */
    swat_tcp_rx_handle(pStreamCxt);
  QUIT:
    /* Free Index */
    swat_cxt_index_free(&tcpRxIndex[pStreamCxt->index]);

    /* Thread Delete */
    swat_task_delete();
}

void
swat_bench_udp_tx_task(STREAM_CXT_t * pStreamCxt)
{
    SWAT_PTR_NULL_CHK(pStreamCxt);

    pStreamCxt->socketLocal = swat_socket(PF_INET, SOCK_DGRAM, 0);
    if (pStreamCxt->socketLocal < 0) {
        SWAT_PTF("Open socket error...\r\n");
        goto QUIT;
    }

    /* Packet Handle */
    swat_udp_tx_handle(pStreamCxt);
  QUIT:
    /* Free Index */
    swat_cxt_index_free(&udpTxIndex[pStreamCxt->index]);
    swat_bench_udp_timer_clean(pStreamCxt->index);
    /* Thread Delete */
    swat_task_delete();
}

void
swat_bench_udp_rx_task(STREAM_CXT_t * pStreamCxt)
{
    A_INT32 ret = 0;
    struct sockaddr_in remoteAddr;
    struct ip_mreq {
        A_UINT32 imr_multiaddr;   /* IP multicast address of group */
        A_UINT32 imr_interface;   /* local IP address of interface */
    } group;

    SWAT_PTR_NULL_CHK(pStreamCxt);

    pStreamCxt->socketLocal = swat_socket(PF_INET, SOCK_DGRAM, 0);
    if (pStreamCxt->socketLocal < 0) {
        SWAT_PTF("Open socket error...\r\n");
        goto QUIT;
    }

    /* Connect Socket */
    swat_mem_set(&remoteAddr, 0, sizeof (struct sockaddr_in));
    remoteAddr.sin_addr.s_addr = HTONL(pStreamCxt->param.ipAddress);
    remoteAddr.sin_port = HTONS(pStreamCxt->param.port);
    remoteAddr.sin_family = AF_INET;

    SWAT_PTF("****************************************************\n");
    SWAT_PTF(" UDP RX Test\n");
    SWAT_PTF("****************************************************\n");

    SWAT_PTF("Local port %d\n", pStreamCxt->param.port);
    SWAT_PTF("Type benchquit to termintate test\n");
    SWAT_PTF("****************************************************\n");

    SWAT_PTF("Waiting.\n");

    ret =
        swat_bind(pStreamCxt->socketLocal, (struct sockaddr *) &remoteAddr,
                  sizeof (struct sockaddr_in));
    if (ret < 0) {
        /* Close Socket */
        SWAT_PTF("Failed to bind socket %d.\n", pStreamCxt->socketLocal);
        swat_socket_close(pStreamCxt);
        goto QUIT;
    }

    group.imr_multiaddr = HTONL(pStreamCxt->param.mcAddress);
    group.imr_interface = HTONL(pStreamCxt->param.ipAddress);

    if (group.imr_multiaddr != 0) {
        ret =
            swat_setsockopt(pStreamCxt->socketLocal, SOL_SOCKET, IP_ADD_MEMBERSHIP, (void *) &group,
                            sizeof (group));
        if (ret < 0) {
            /* Close Socket */
            SWAT_PTF("Failed to set socket option %d.\n", pStreamCxt->socketLocal);
            swat_socket_close(pStreamCxt);
            goto QUIT;
        }
    }

    /* Packet Handle */
    swat_udp_rx_handle(pStreamCxt);
  QUIT:
    /* Free Index */
    swat_cxt_index_free(&udpRxIndex[pStreamCxt->index]);

    /* Thread Delete */
    swat_task_delete();
}

void
swat_bench_tcp_tx(A_UINT32 index)
{
    swat_bench_tcp_tx_task(&cxtTcpTxPara[index]);
}

void
swat_bench_udp_tx(A_UINT32 index)
{
    swat_bench_udp_tx_task(&cxtUdpTxPara[index]);
}

void
swat_bench_tcp_rx(A_UINT32 index)
{
    swat_bench_tcp_rx_task(&cxtTcpRxPara[index]);
}

void
swat_bench_udp_rx(A_UINT32 index)
{
    swat_bench_udp_rx_task(&cxtUdpRxPara[index]);
}

void
swat_bench_tx_test(A_UINT32 ipAddress,A_UINT32 port, A_UINT32 protocol, A_UINT32 pktSize,
							A_UINT32 mode, A_UINT32 seconds, A_UINT32 numOfPkts,A_UINT32 delay)
{

    A_UINT8 index = 0;
    A_UINT8 ret = 0;

    /* TCP */
    if (TEST_PROT_TCP == protocol) {
        /*Calc Handle */
        for (index = 0; index < CALC_STREAM_NUMBER; index++) {
            ret = swat_cxt_index_find(&tcpTxIndex[index]);

            if (CALC_STREAM_NUMBER_INVALID != ret) {
                swat_cxt_index_configure(&tcpTxIndex[index]);
                break;
            }
        }
        if (CALC_STREAM_NUMBER_INVALID == ret) {
            SWAT_PTF("Warning tcpTxIndex is full\n");
            return;
        }
        /* Database Initial */
        swat_database_initial(&cxtTcpTxPara[index]);
        /* Update Database */
        swat_database_set(&cxtTcpTxPara[index], ipAddress, INADDR_ANY, port, protocol,
                          pktSize, mode, seconds, numOfPkts, TEST_DIR_TX, delay);

        /* Client */
        cxtTcpTxPara[index].index = index;
        ret = swat_task_create(swat_bench_tcp_tx, index, 2048, 50);
        if (0 == ret) {
            SWAT_PTF("TCP TX session %d Success\n", index);
        } else {
            swat_cxt_index_free(&tcpTxIndex[index]);
        }
        /* Thread Delete */
    }
    /* UDP */
    if (TEST_PROT_UDP == protocol) {
        /*Calc Handle */
        for (index = 0; index < CALC_STREAM_NUMBER; index++) {
            ret = swat_cxt_index_find(&udpTxIndex[index]);

            if (CALC_STREAM_NUMBER_INVALID != ret) {
                swat_cxt_index_configure(&udpTxIndex[index]);
                break;
            }
        }
        if (CALC_STREAM_NUMBER_INVALID == ret) {
            SWAT_PTF("Warning udpTxIndex is full\n");
            return;
        }

        /* DataBase Initial */
        swat_database_initial(&cxtUdpTxPara[index]);

        /* Update Database */
        swat_database_set(&cxtUdpTxPara[index], ipAddress, INADDR_ANY, port, protocol,
                          pktSize, mode, seconds, numOfPkts, TEST_DIR_TX,delay);

        /* Client */
        cxtUdpTxPara[index].index = index;
        /* Client */
        ret = swat_task_create(swat_bench_udp_tx, index, 2048, 50);
        if (0 == ret) {
            SWAT_PTF("UDP TX session %d Success\n", index);
        } else {
            swat_cxt_index_free(&udpTxIndex[index]);
        }
    }
}

void
swat_bench_rx_test(A_UINT32 protocol, A_UINT32 port, A_UINT32 localIpAddress, A_UINT32 mcastIpAddress)
{
    A_UINT8 index = 0;
    A_UINT8 ret = 0;

    /* TCP */
    if (TEST_PROT_TCP == protocol) {
        /*Calc Handle */
        for (index = 0; index < CALC_STREAM_NUMBER; index++) {
            ret = swat_cxt_index_find(&tcpRxIndex[index]);

            if (CALC_STREAM_NUMBER_INVALID != ret) {
                swat_cxt_index_configure(&tcpRxIndex[index]);
                break;
            }
        }
        if (CALC_STREAM_NUMBER_INVALID == ret) {
            SWAT_PTF("Warning tcpRxIndex is full\n");
            return;
        }

        /* DataBase Initial */
        swat_database_initial(&cxtTcpRxPara[index]);

        /* Update DataBase */
        swat_database_set(&cxtTcpRxPara[index], INADDR_ANY, INADDR_ANY, port, protocol,
                          1500, 0, 0, 0, TEST_DIR_RX, 0);

        /* Server */
        cxtTcpRxPara[index].index = index;
        ret = swat_task_create(swat_bench_tcp_rx, index, 2048, 50);
        if (0 == ret) {
            SWAT_PTF("TCP RX session %d Success\n", index);
        } else {
            swat_cxt_index_free(&tcpRxIndex[index]);
        }
    }
    /* UDP */
    if (TEST_PROT_UDP == protocol) {
        /*Calc Handle */
        for (index = 0; index < CALC_STREAM_NUMBER; index++) {
            ret = swat_cxt_index_find(&udpRxIndex[index]);

            if (CALC_STREAM_NUMBER_INVALID != ret) {
                swat_cxt_index_configure(&udpRxIndex[index]);
                break;
            }
        }

        if (CALC_STREAM_NUMBER_INVALID == ret) {
            SWAT_PTF("Warning udpRxIndex is full\n");
            return;
        }

        /* DataBase Initial */
        swat_database_initial(&cxtUdpRxPara[index]);

        /* Update DataBase */
        swat_database_set(&cxtUdpRxPara[index], INADDR_ANY, mcastIpAddress, port,
                          protocol, 1500, 0, 0, 0, TEST_DIR_RX,0);

        /* Server */
        cxtUdpRxPara[index].index = index;
        /* Server */
        ret = swat_task_create(swat_bench_udp_rx, index, 2048, 50);
        if (0 == ret) {
            SWAT_PTF("UDP RX session %d Success\n", index);
        } else {
            swat_cxt_index_free(&udpRxIndex[index]);
        }
    }
}

#endif
