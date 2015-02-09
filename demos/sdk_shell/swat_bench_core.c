/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom/socket_api.h"
#include "swat_bench_core.h"

A_UINT32 quitBenchVal = 0;

STREAM_CXT_t cxtTcpTxPara[CALC_STREAM_NUMBER];
STREAM_CXT_t cxtTcpRxPara[CALC_STREAM_NUMBER];
STREAM_CXT_t cxtUdpTxPara[CALC_STREAM_NUMBER];
STREAM_CXT_t cxtUdpRxPara[CALC_STREAM_NUMBER];

STREAM_INDEX_t tcpTxIndex[CALC_STREAM_NUMBER];
STREAM_INDEX_t tcpRxIndex[CALC_STREAM_NUMBER];
STREAM_INDEX_t udpTxIndex[CALC_STREAM_NUMBER];
STREAM_INDEX_t udpRxIndex[CALC_STREAM_NUMBER];

A_UINT8
swat_cxt_index_find(STREAM_INDEX_t * pStreamIndex)
{
    if (FALSE == pStreamIndex->isUsed) {
        return 0;
    }

    return CALC_STREAM_NUMBER_INVALID;
}

void
swat_cxt_index_configure(STREAM_INDEX_t * pStreamIndex)
{
    pStreamIndex->isUsed = TRUE;
}

void
swat_cxt_index_free(STREAM_INDEX_t * pStreamIndex)
{
    pStreamIndex->isUsed = FALSE;
}

void
swat_database_initial(STREAM_CXT_t * pCxtPara)
{
    /* Bench Para */
    pCxtPara->param.ipAddress = STREAM_IP_ADDRESS_DEF;
    pCxtPara->param.port = STREAM_PORT_DEF;
    pCxtPara->param.pktSize = STREAM_PKT_SIZE_DEF;
    pCxtPara->param.seconds = STREAM_SECONDS_DEF;
    pCxtPara->param.numOfPkts = STREAM_NUM_OF_PKTS_DEF;
    pCxtPara->param.mode = STREAM_MODE_DEF;
    pCxtPara->param.protocol = STREAM_PROTOCOL_DEF;
    pCxtPara->param.delay = STREAM_DELAY_DEF;

    /* Calc Para */
    pCxtPara->calc.firstTime.milliseconds = CALC_TIME_DEF;
    pCxtPara->calc.lastTime.milliseconds = CALC_TIME_DEF;
    pCxtPara->calc.bytes = CALC_BYTES_DEF;
    pCxtPara->calc.kbytes = CALC_KBYTES_DEF;
}

void
swat_database_print(STREAM_CXT_t * pCxtPara)
{

    SWAT_PTF("\nindex:%d,socketLocal:%d,clientFd:%d\n", pCxtPara->index,
             pCxtPara->socketLocal, pCxtPara->clientFd);

    /* Bench Para */
    IPV4_PTF(pCxtPara->param.ipAddress);

    SWAT_PTF(":%d,%d,size:%d,mode %d,time %d s/pkts %d \n",
             pCxtPara->param.port,
             pCxtPara->param.protocol,
             pCxtPara->param.pktSize,
             pCxtPara->param.mode, pCxtPara->param.seconds, pCxtPara->param.numOfPkts);

    /* Calc Para */

    SWAT_PTF("firsttime:%d,lasttime:%d,bytes:%d,Kbytes:%d \n",
             pCxtPara->calc.firstTime.milliseconds, pCxtPara->calc.lastTime.milliseconds,
             pCxtPara->calc.bytes, pCxtPara->calc.kbytes);

}

void
swat_database_set(STREAM_CXT_t * pCxtPara,
                  A_UINT32 ipAddress,
                  A_UINT32 mcAddress,
                  A_UINT32 port,
                  A_UINT32 protocol,
                  A_UINT32 pktSize, A_UINT32 mode, A_UINT32 seconds, A_UINT32 numOfPkts, A_UINT32 direction,
                  A_UINT32 delay)
{
    /* Stream Para */
    pCxtPara->param.ipAddress = ipAddress;
    pCxtPara->param.mcAddress = mcAddress;
    pCxtPara->param.port = port;
    pCxtPara->param.protocol = protocol;
    pCxtPara->param.pktSize = pktSize;
    pCxtPara->param.mode = mode;
    pCxtPara->param.seconds = seconds;
    pCxtPara->param.numOfPkts = numOfPkts;
    pCxtPara->param.direction = direction;
	pCxtPara->param.delay = delay;
}

void
swat_get_first_time(STREAM_CXT_t * pCxtPara)
{
    pCxtPara->calc.firstTime.milliseconds = swat_time_get_ms();
}

void
swat_get_last_time(STREAM_CXT_t * pCxtPara)
{
    pCxtPara->calc.lastTime.milliseconds = swat_time_get_ms();
}

void
swat_bytes_calc(STREAM_CXT_t * pCxtPara, A_UINT32 bytes)
{
    pCxtPara->calc.bytes += bytes;
    if (0 != pCxtPara->calc.bytes / 1024) {
        pCxtPara->calc.kbytes += pCxtPara->calc.bytes / 1024;
        pCxtPara->calc.bytes = pCxtPara->calc.bytes % 1024;
    }
}

void
swat_socket_close(STREAM_CXT_t * pCxtPara)
{
    if (-1 != pCxtPara->socketLocal) {
        swat_close(pCxtPara->socketLocal);
        pCxtPara->socketLocal = -1;
    }
}

void
swat_buffer_free(A_UINT8 ** pDataBuffer)
{
    if (NULL != *pDataBuffer) {
        swat_mem_free(*pDataBuffer);
        *pDataBuffer = NULL;
    }
}

A_UINT32
swat_check_time(STREAM_CXT_t * pCxtPara)
{
    A_UINT32 msInterval = 0;
    A_UINT32 totalInterval = 0;

    msInterval = (pCxtPara->calc.lastTime.milliseconds - pCxtPara->calc.firstTime.milliseconds);
    totalInterval = msInterval;

    if (totalInterval >= pCxtPara->param.seconds * 1000) {
        return 1;
    } else {
        return 0;
    }
}

void
swat_test_result_print(STREAM_CXT_t * pCxtPara)
{
    A_UINT32 throughput = 0;
    A_UINT32 totalBytes = 0;
    A_UINT32 totalKbytes = 0;
    A_UINT32 msInterval =
        (pCxtPara->calc.lastTime.milliseconds - pCxtPara->calc.firstTime.milliseconds);
    A_UINT32 totalInterval = msInterval;
	A_UINT32 sInterval = totalInterval/1000;//seconds

    if (totalInterval > 0) {
        if ((0 == pCxtPara->calc.bytes)
            && (0 == pCxtPara->calc.kbytes)) {
            throughput = 0;
        } else {
        	/*when calc.kbytes >0x7FFFF, the value of (pCxtPara->calc.kbytes * 1024 * 8)
        		will overflow */
        	if (pCxtPara->calc.kbytes < 0x7FFFF){
	            throughput =
	                ((pCxtPara->calc.kbytes * 1024 * 8) / (totalInterval)) +
	                ((pCxtPara->calc.bytes * 8) / (totalInterval));
    		}
			else{
				throughput =((pCxtPara->calc.kbytes * 8) / (sInterval))
					 	+ ((pCxtPara->calc.bytes * 8/1024) / (sInterval));
			}
	        totalBytes = pCxtPara->calc.bytes;
            totalKbytes = pCxtPara->calc.kbytes;
        }
    } else {
        throughput = 0;
    }

    switch (pCxtPara->param.protocol) {
    case TEST_PROT_UDP:
        {
            if (TEST_DIR_RX == pCxtPara->param.direction) {
                SWAT_PTF("\nResults for %s test:\n\n", "UDP Receive");
            } else {
                SWAT_PTF("\nResults for %s test:\n\n", "UDP Transmit");
            }

            break;
        }

    case TEST_PROT_TCP:
        {
            if (TEST_DIR_RX == pCxtPara->param.direction) {
                SWAT_PTF("\nResults for %s test:\n\n", "TCP Receive");
            } else {
                SWAT_PTF("\nResults for %s test:\n\n", "TCP Transmit");
            }

            break;
        }
    default:
        {
            SWAT_PTF("\nUnknown Protocol:\n\n");
            break;
        }
    }

    SWAT_PTF("\t%d Bytes in %d seconds %d ms  \n", totalKbytes * 1024 + totalBytes,
             totalInterval / 1000, totalInterval % 1000);
    SWAT_PTF("\t%d KBytes %d bytes in %d seconds %d ms  \n\n", totalKbytes, totalBytes,
             totalInterval / 1000, totalInterval % 1000);
    SWAT_PTF("\t throughput %d kb/sec\n", throughput);
}

A_UINT32
swat_bench_quit()
{
    return quitBenchVal;
}

void
swat_bench_quit_init()
{
    quitBenchVal = 0;
}

void
swat_bench_quit_config()
{
    quitBenchVal = 1;
}

void
swat_bench_dbg()
{
    A_UINT32 index = 0;

    A_UINT32 ret = 0;

    extern A_UINT32 allocram_remaining_bytes;
    SWAT_PTF("### SWAT MEM FREE : %d.\n", allocram_remaining_bytes);
    for (index = 0; index < CALC_STREAM_NUMBER; index++) {

        /*tcpTx */
        SWAT_PTF("\ntcpTx[%d]:\n", index);
        ret = swat_cxt_index_find(&tcpTxIndex[index]);

        if (CALC_STREAM_NUMBER_INVALID == ret) {
            swat_database_print(&cxtTcpTxPara[index]);
        }

        /*tcpRx */
        SWAT_PTF("\ntcpRx[%d]:\n", index);
        ret = swat_cxt_index_find(&tcpRxIndex[index]);

        if (CALC_STREAM_NUMBER_INVALID == ret) {
            swat_database_print(&cxtTcpRxPara[index]);
        }
        /*udpTx */
        SWAT_PTF("\nudpTx[%d]:\n", index);
        ret = swat_cxt_index_find(&udpTxIndex[index]);

        if (CALC_STREAM_NUMBER_INVALID == ret) {
            swat_database_print(&cxtUdpTxPara[index]);
        }

        /*udpRx */
        SWAT_PTF("\nudpRx[%d]:\n", index);
        ret = swat_cxt_index_find(&udpRxIndex[index]);

        if (CALC_STREAM_NUMBER_INVALID == ret) {
            swat_database_print(&cxtUdpRxPara[index]);
        }
    }

}
