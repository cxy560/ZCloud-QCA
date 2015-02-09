/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */
#include <qcom/qcom_common.h>
#include <qcom/socket_api.h>
#include <qcom/select_api.h>
#include "threadx/tx_api.h"
TX_THREAD ota_thread;
TX_BYTE_POOL ota_pool;
CHAR *ota_thread_stack_pointer;


extern void qcom_task_exit();

#define QCA_OTA_DEMO_LISTEN_PORT 6666

#define QCA_OTA_PROTOCOL_ID 0xFF
#define QCA_OTA_MSG_BUF_SIZE 256
#define QCA_OTA_MAX_FILENAME_LEN 64

#define MD5_CHECKSUM_LEN 32
typedef enum { 
  QCA_OTA_OK = 0, 
  QCA_OTA_ERR_UNKOWN_MSG = 1000, 
  QCA_OTA_ERR_IMAGE_NOT_FOUND = 1001, 
  QCA_OTA_ERR_IMAGE_DOWNLOAD_FAIL = 1002, 
  QCA_OTA_ERR_IMAGE_CHECKSUM_INCORRENT = 1003, 
  QCA_OTA_ERR_SERVER_RSP_TIMEOUT = 1004, 
  QCA_OTA_ERR_INVALID_FILENAME = 1005, 
  QCA_OTA_ERR_UNSUPPORT_PROTOCOL = 1006, 
  QCA_OTA_ERR_INVALID_PARTITION_INDEX = 1007, 
  QCA_OTA_ERR_IMAGE_HDR_INCORRENT = 1008,
} QCA_OTA_ERROR_CODE_t;

typedef enum { 
  QCA_OTA_REQUEST = 0, 
  QCA_OTA_RESPONSE = 1,
} QCA_OTA_MSG_OPCODE_t;

typedef struct {
    char qca_code;
    char opcode;
    unsigned char length;
    char payload[0];
} __attribute__ ((packed)) QCA_OTA_MSG_HDR_t;

typedef struct {
    int server_ip;
    char file_name[QCA_OTA_MAX_FILENAME_LEN];
    unsigned char protocol;
    char partition_index;
    char ota_msg_checksum[MD5_CHECKSUM_LEN];
} __attribute__ ((packed)) QCA_OTA_MSG_REQUEST_t;

typedef struct {
    int response_code;
    char ota_image_checksum[MD5_CHECKSUM_LEN];
    char ota_msg_checksum[MD5_CHECKSUM_LEN];
} __attribute__ ((packed)) QCA_OTA_MSG_RESPONSE_t;

/* OTA daemon buffer */
char demo_ota_msg_rx_buf[QCA_OTA_MSG_BUF_SIZE] = { 0 };
char demo_ota_msg_tx_buf[QCA_OTA_MSG_BUF_SIZE] = { 0 };
char demo_ota_msg_checksum[MD5_CHECKSUM_LEN] = { 0 };
static int demo_ota_deamon_stop = 0;
static int demo_ota_daemon_port = 0;
static int demo_firsttime_start = 1;

/* ota monitor listening socket */
int socket_serv;

#define OTA_POOL_SIZE (4*1024)
#define OTA_STACK_SIZE (3*1024)


/**********************************************************************/
void
demo_ota_packet_printf(unsigned char *data, int len)
{
    int count = 0;
    A_PRINTF("----------------------------------------------\n");
    A_PRINTF("_packet_printf:0x%p, len = %d\n", data, len);
    A_PRINTF("----------------------------------------------\n");
    for (count = 0; count < len; count++) {
        A_PRINTF("%02x ", data[count]);
        if (count && (15 == (count % 16)))
            A_PRINTF("\n");
    }
    A_PRINTF("\n");
    A_PRINTF("----------------------------------------------\n");
}

void
demo_ota_request_daemon(unsigned long which_threa)
{
    int ret = 0;
    struct sockaddr_in srv_addr;
    int socket_client = -1;
    struct sockaddr_in client_addr;
    int len = 0;
    q_fd_set fd_sockSet;
    int fd_act = 0;
    int nSend = 0, nRecv = 0;
    QCA_OTA_MSG_HDR_t *qca_ota_hdr;
    char *pkt;
    QCA_OTA_MSG_REQUEST_t *qca_ota_request;
    QCA_OTA_MSG_RESPONSE_t *qca_ota_response;
    struct timeval timeout;
    int testip;
    int client_ip_addr;
    int client_l4_port;
    A_PRINTF("OTA deamon demo task start \n");

    /* setup socket */
    socket_serv = qcom_socket(AF_INET, SOCK_STREAM, 0);
    if (socket_serv < 0) {
        return;
    }
    demo_ota_daemon_port = QCA_OTA_DEMO_LISTEN_PORT;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(demo_ota_daemon_port);
    srv_addr.sin_family = AF_INET;

    /* bind the socket */
    ret = qcom_bind(socket_serv, (struct sockaddr *) &srv_addr, sizeof (struct sockaddr_in));
    if (ret < 0) {
        A_PRINTF("qcom_bind fail, ret = %d!\n", ret);
        goto exit;
    }

    /* start listening */
    ret = qcom_listen(socket_serv, 10);
    if (ret < 0) {
        A_PRINTF("qcom_listen fail, ret = %d!\n", ret);
        goto exit;
    }
    A_PRINTF("Demo OTA request deamon is listenning on port: %d ...\n", demo_ota_daemon_port);

    /* wait for somebody to connect me */
    for (;;) {
        socket_client = qcom_accept(socket_serv, (struct sockaddr *) &client_addr, &len);
        if (socket_client < 0) {
            if (demo_ota_deamon_stop) {
                goto exit;
            }

            else {
                A_PRINTF("qcom_accept fail, ret = %d!\n", socket_client);
                continue;
            }
        }
        client_ip_addr = ntohl(client_addr.sin_addr.s_addr);
        client_l4_port = ntohs(client_addr.sin_port);
        A_PRINTF("Accept connection from IP %lx, Port %d\n", client_ip_addr, client_l4_port);
        while (1) {

            /* init fd_set */
            FD_ZERO(&fd_sockSet);
            FD_SET(socket_client, &fd_sockSet);
            timeout.tv_sec = 10;
            timeout.tv_usec = 0;

            /* wait for input */
            fd_act = 0;
            fd_act = qcom_select(socket_client + 1, &fd_sockSet, NULL, NULL, &timeout);
            if (fd_act == 0) {
                A_PRINTF("qcom_select() timeout\n");
            }

            else {
                if (FD_ISSET(socket_client, &fd_sockSet)) {
                    memset(demo_ota_msg_rx_buf, 0, QCA_OTA_MSG_BUF_SIZE);
                    nRecv = qcom_recv(socket_client, demo_ota_msg_rx_buf, QCA_OTA_MSG_BUF_SIZE, 0);
                    if (nRecv < 0) {
                        A_PRINTF("qcom_recv() error, ret = %d\n", nRecv);
                        continue;
                    }

                    /* sanity check: QCA OTA protocol ID */
                    if (pkt[0] != QCA_OTA_PROTOCOL_ID) {
                        ret = QCA_OTA_ERR_UNKOWN_MSG;
                        goto sendRsp;
                    }

                    /* handle OTA request message */
                    pkt = demo_ota_msg_rx_buf;
                    qca_ota_hdr = (QCA_OTA_MSG_HDR_t *) pkt;

                    /* dispatch according to opcode */
                    switch (qca_ota_hdr->opcode) {
                    case QCA_OTA_REQUEST:
                        A_PRINTF("receive QCA_OTA_REQUEST\n");
                        qca_ota_request = (QCA_OTA_MSG_REQUEST_t *) qca_ota_hdr->payload;
                        A_MEMCPY(&testip, &(qca_ota_request->server_ip), sizeof (int));
                        extern int qca_ota_upgrade(unsigned int ip_addr, char *file_name,
                                                   unsigned int protocol, int partition_index);
                        ret =
                            qca_ota_upgrade(testip, (char *) qca_ota_request->file_name,
                                            qca_ota_request->protocol, -1);
                        break;
                    default:
                        ret = QCA_OTA_ERR_UNKOWN_MSG;
                        break;
                    }
                  sendRsp:
                    /* prepare reponse message */
                    memset(demo_ota_msg_tx_buf, 0, QCA_OTA_MSG_BUF_SIZE);
                    qca_ota_hdr = (QCA_OTA_MSG_HDR_t *) demo_ota_msg_tx_buf;
                    qca_ota_hdr->qca_code = QCA_OTA_PROTOCOL_ID;
                    qca_ota_hdr->opcode = QCA_OTA_RESPONSE;
                    qca_ota_hdr->length = sizeof (QCA_OTA_MSG_RESPONSE_t);
                    qca_ota_response = (QCA_OTA_MSG_RESPONSE_t *) qca_ota_hdr->payload;
                    qca_ota_response->response_code = ret;

                    /* send response msg to remote controller */
                    nSend =
                        qcom_send(socket_client, demo_ota_msg_tx_buf,
                                 sizeof (QCA_OTA_MSG_HDR_t) + qca_ota_hdr->length, 0);
                    if (nSend < 0) {
                        A_PRINTF("qcom_send() error, ret = %d\n", nSend);
                        continue;
                    }
                    break;
                }
            }
        }
        qcom_close(socket_client);
    }
  exit:
    A_PRINTF("demo OTA server is turning down\n");
    FD_ZERO(&fd_sockSet); 
    tx_thread_terminate(&ota_thread);
    return;
}

int
demo_ota_daemon_start(void)
{
	int i;

	if(demo_firsttime_start)
	{
        tx_byte_pool_create(&ota_pool,
                        "ota pool",
                        TX_POOL_CREATE_DYNAMIC,
                        OTA_POOL_SIZE);
		tx_byte_allocate(&ota_pool, (VOID **)&ota_thread_stack_pointer, OTA_STACK_SIZE, TX_NO_WAIT);

		demo_firsttime_start = 0;
	}
	else
	{
		
        tx_thread_delete(&ota_thread);
	}

	extern void demo_ota_request_daemon(ULONG which_thread);
    tx_thread_create(&ota_thread, "ota thread", demo_ota_request_daemon,
          i, ota_thread_stack_pointer, OTA_STACK_SIZE, 16, 16, 4, TX_AUTO_START);

	return 0;
}

int
demo_ota_daemon_stop(void)
{
    demo_ota_deamon_stop = 1;
    qcom_close(socket_serv);
    return 0;
}
