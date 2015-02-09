/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include <qcom/qcom_common.h>
#include <qcom/socket_api.h>
#include <qcom/select_api.h>
#include <qcom/qcom_utils.h>
#include <qcom/qcom_nvram.h>


#define QCA_PARTITION_NUM 4

#define QCA_OTA_MAX_TFTP_RETRY_TIMES 2

#define QCA_OTA_PROTOCOL_ID 0xFF
#define QCA_OTA_MSG_BUF_SIZE 256
#define QCA_OTA_MAX_FILENAME_LEN 64

#define MD5_CHECKSUM_LEN 32

#define NVM_BLOCK_SZ 0x1000

typedef enum {
  QCA_OTA_PROTOCOL_TFTP = 0,
  QCA_OTA_PROTOCOL_MAX = QCA_OTA_PROTOCOL_TFTP,
} QCA_OTA_PROTOCOL_t;

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
} QCA_OTA_MSG_TYPE_t;

typedef struct {
    int magic;
    int length;
    char checksum[MD5_CHECKSUM_LEN];
} __attribute__ ((packed)) QCA_OTA_IMAGE_HDR_t;

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

/* TFTP misc */
#define TFTP_TIMEOUT		5000UL
#define TFTP_RX_SIZE        1024
#define TFTP_TX_SIZE        128
#define TFTP_SERVER_PORT    69

/* TFTP operations. */
#define TFTP_OP_RRQ		1
#define TFTP_OP_WRQ	 	2
#define TFTP_OP_DATA	3
#define TFTP_OP_ACK		4
#define TFTP_OP_ERROR	5
#define TFTP_OP_OACK	6

/* TFTP states */
#define TFTP_ST_RRQ			1
#define TFTP_ST_DATA		2
#define TFTP_ST_TOO_LARGE	3
#define TFTP_ST_BAD_MAGIC	4
#define TFTP_ST_OACK		5
#define TFTP_ST_ERROR 		6
#define TFTP_BLOCK_SIZE		512 /* default TFTP block size  */

void _ota_tftp_pkt_rrq(const char *file_name, unsigned char *buf, int *len);
void _ota_tftp_pkt_ack(int block_seq, unsigned char *buf, int *len);
void _ota_tftp_pkt_parse(unsigned char *pkt_ptr, int pkt_len, int *pkt_seq, int *pkt_state);
void _ota_packet_printf(unsigned char *data, int len);
void _ota_nvram_read_data(int partition, unsigned int offset, unsigned char *buf, int size);
void _ota_nvram_write_data(int partition, unsigned int offset, unsigned char *buf, int size);
void _ota_nvram_erase_partition(int partition);
void _ota_nvram_erase_partition_magicword(int partition);
void _ota_nvram_write_partition_magicword(int partition);
int _ota_nvram_get_partition_base(int partition);
void _ota_nvram_set_partition_base(int partition, A_UINT32 base_addr);
void _ota_nvram_get_active_image_partition(int *partition);
int _ota_parse_image_hdr(unsigned char *buffer);
void _ota_calculate_image_checksum(int partition, char *checksum);
void _ota_nvram_swap_partition_tbl_entry(int partition1, int partition2);
int _ota_tftp(unsigned int ip_addr, char *file_name, int partition_index);

/* TFTP buffer */
unsigned char tftp_tx_buf[TFTP_TX_SIZE] = { 0 };
unsigned char tftp_rx_buf[TFTP_RX_SIZE] = { 0 };
unsigned char tmp_buf[256] = { 0 };
int current_partition = 0;
int upgrade_partition = 0;
int qca_partition_tbl_base = 0;
int qca_partition_entries[QCA_PARTITION_NUM] = { 0 };
int ota_image_len = 0;
char ota_image_checksum[MD5_CHECKSUM_LEN] = { 0 };
char current_image_checksum[MD5_CHECKSUM_LEN] = { 0 };
char last_image_checksum[MD5_CHECKSUM_LEN] = { 0 };
int retry_count = 0;
int _ota_write_flash = 1;
int ota_need_swap_partition = 0;

struct MD5Context {
    A_UINT32      buf[4];
    A_UINT32      bits[2];
    unsigned char in[64];
};
struct MD5Context ota_md5_ctx;

void
_ota_tftp_pkt_rrq(const char *file_name, unsigned char *buf, int *len)
{
    unsigned char *pkt;
    unsigned short *s;
    pkt = buf;
    s = (unsigned short *) pkt;
    *s++ = htons(TFTP_OP_RRQ);
    pkt = (unsigned char *) s;
    A_STRCPY((char *) pkt, file_name);
    pkt += A_STRLEN(file_name) + 1;
    A_STRCPY((char *) pkt, "octet");
    pkt += 5;
    *len = pkt - buf;
}

void
_ota_tftp_pkt_ack(int block_seq, unsigned char *buf, int *len)
{
    unsigned char *pkt;
    unsigned short *s;
    pkt = buf;
    s = (unsigned short *) pkt;
    *s++ = htons(TFTP_OP_ACK);
    *s++ = htons(block_seq);
    pkt = (unsigned char *) s;
    *len = pkt - buf;
}

void
_ota_tftp_pkt_parse(unsigned char *pkt_ptr, int pkt_len, int *pkt_seq, int *pkt_state)
{
    int len = pkt_len;
    unsigned char *pkt = pkt_ptr;
    unsigned short opcode, *s;
    if (len < 2) {
        return;
    }
    len -= 2;
    s = (unsigned short *) pkt;
    opcode = *s++;
    pkt = (unsigned char *) s;
    switch (ntohs(opcode)) {
    case TFTP_OP_RRQ:
    case TFTP_OP_WRQ:
    case TFTP_OP_ACK:
        break;
    case TFTP_OP_OACK:
        *pkt_state = TFTP_ST_OACK;
        break;
    case TFTP_OP_DATA:
        if (len < 2)
            return;
        *pkt_seq = ntohs(*(unsigned short *) pkt);
        if (*pkt_state == TFTP_ST_RRQ || *pkt_state == TFTP_ST_OACK) {
            *pkt_state = TFTP_ST_DATA;
        }
        break;
    case TFTP_OP_ERROR:
        *pkt_state = TFTP_ST_ERROR;
        break;
    default:
        break;
    }
}

void
_ota_packet_printf(unsigned char *data, int len)
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
    if (data[1] == TFTP_OP_RRQ) {
        A_PRINTF("TFTP_OP_RRQ: filename = %s\n", (&data[2]));
    }

    else if (data[1] == TFTP_OP_DATA) {
        A_PRINTF("TFTP_OP_DATA: blockNum = %d\n", ((data[2] << 8) | data[3]));
    }
    A_PRINTF("----------------------------------------------\n");
}

/* write data to one partition */
void
_ota_nvram_read_data(int partition, unsigned int offset, unsigned char *buf, int size)
{
    int base_addr = 0;
    base_addr = _ota_nvram_get_partition_base(partition);
    qcom_nvram_read(base_addr + offset, buf, size);
    return;
}

/* write data to one partition */
void
_ota_nvram_write_data(int partition, unsigned int offset, unsigned char *buf, int size)
{
    int base_addr = 0;
    base_addr = _ota_nvram_get_partition_base(partition);
    qcom_nvram_write(base_addr + offset, buf, size);
    return;
}

void
_ota_nvram_erase_partition(int partition)
{
    int base_addr = 0;
    int size = 0;
    base_addr = _ota_nvram_get_partition_base(partition);
    if (partition == 0) {

        /* partition #0 is special, with some bytes used for other purpose,
           16 bytes at the header, and 12 bytes at the end. These bytes
           shoule not been erased */
        size = 0x40000 - 16 - 12;

        /* align with FLASH sector size */
        size = size & 0xFFFFF000;
    }
    else {
        size = 0x40000;
    }
	qcom_nvram_erase(base_addr, size);
    return;
}

/* erase MAGIC word for a partition */
void
_ota_nvram_erase_partition_magicword(int partition)
{
    int base_addr = 0;
    unsigned char magic[4] = { 0xff, 0xff, 0xff, 0xff };
    base_addr = _ota_nvram_get_partition_base(partition);
    qcom_nvram_write(base_addr, magic, 4);
    return;
}

/* write MAGIC word for a partition */
void
_ota_nvram_write_partition_magicword(int partition)
{
    int base_addr = 0;
    unsigned char magic[4] = { 'N', 'V', 'R', 'M' };
    base_addr = _ota_nvram_get_partition_base(partition);
    qcom_nvram_write(base_addr, magic, 4);
    return;
}

void
_ota_nvram_load_partition_table(void)
{
    qca_partition_entries[0] = 0x10;

    /* load partition table */
    qcom_nvram_read(0, (A_UINT8 *) & qca_partition_tbl_base, 4);
    qcom_nvram_read(qca_partition_tbl_base, (A_UINT8 *) & qca_partition_entries[1], 12);
    return;
}

/* set base address of one partition */
int
_ota_nvram_get_partition_base(int partition)
{
    return qca_partition_entries[partition];
}

/* get base address of one partition */
void
_ota_nvram_set_partition_base(int partition, A_UINT32 base_addr)
{
    qca_partition_entries[partition] = base_addr;
    _ota_nvram_load_partition_table();
    return;
}

/* get partition num of current active image*/
void
_ota_nvram_get_active_image_partition(int *partition)
{
    int active_partition = 0;
    active_partition = qcom_nvram_select(-1);
    *partition = active_partition;
    return;
}

/* get partition num of current active image*/
int
_ota_parse_image_hdr(unsigned char *buffer)
{
    int ret = 0;
    QCA_OTA_IMAGE_HDR_t *ota_image_hdr;
    int magic = 0;
    ota_image_hdr = (QCA_OTA_IMAGE_HDR_t *) buffer;
    magic = ntohl(ota_image_hdr->magic);
    if (magic != 0x1234) {
        return QCA_OTA_ERR_IMAGE_HDR_INCORRENT;
    }
    ota_image_len = ntohl(ota_image_hdr->length);
    memcpy(ota_image_checksum, ota_image_hdr->checksum, MD5_CHECKSUM_LEN);
    return ret;
}

void
_ota_nvram_swap_partition_tbl_entry(int partition1, int partition2)
{
    int tmp_base_1;
    int tmp_base_2;
    tmp_base_1 = _ota_nvram_get_partition_base(partition1);
    tmp_base_2 = _ota_nvram_get_partition_base(partition1);
    _ota_nvram_set_partition_base(partition1, tmp_base_2);
    _ota_nvram_set_partition_base(partition2, tmp_base_1);
} void
_ota_calculate_image_checksum(int partition, char *checksum)
{
    int len = ota_image_len;
    int offset = 4;
    int i = 0;
    qcom_sec_md5_init();
    while (len > TFTP_RX_SIZE) {
        memset(tftp_rx_buf, 0, TFTP_RX_SIZE);
        _ota_nvram_read_data(partition, offset, tftp_rx_buf, TFTP_RX_SIZE);
        qcom_sec_md5_update(tftp_rx_buf, TFTP_RX_SIZE);
        offset += TFTP_RX_SIZE;
        len -= TFTP_RX_SIZE;
        i++;
    }
    memset(tftp_rx_buf, 0, TFTP_RX_SIZE);
    _ota_nvram_read_data(partition, offset, tftp_rx_buf, len);
    qcom_sec_md5_update(tftp_rx_buf, len);
    memset(checksum, 0, 16);
    qcom_sec_md5_final(checksum);
    return;
}

/*  QCA OTA upgrade based on TFTP protocol */
int
_ota_tftp(unsigned int ip_addr, char *file_name, int partition_index)
{
    int ret = 0;
    int fd, pkt_len, pkt_seq = 0, pkt_seq_last = 0;
    struct sockaddr_in serv_addr, from_addr;
    int from_size = 0;
    struct timeval tmo;
    q_fd_set fd_sockSet;
    int fd_act, nRecv, retry_times = 0;
    int tftp_state = 0, total_rx_bytes = 0, blk_count = 0;
    unsigned char *data_blk_buf;
    int data_blk_size = 0;
    int current_partition = 0;
    int nSend = 0;

    /* misc reset */
    retry_count = 0;

    /* In current design, if FLASH is 512KB, then only 2 partitions are supported.
       Partition 0 (0x0000-0010 - 0x000-3FFFF): Gloden image
       Partition 1 (0x0004-0000 - 0x000-7FFFF): User image (upgradable)
       Partition 2 (0xFFFF-FFFF - 0FFFF-FFFF): Invalid
       Partition 3 (0xFFFF-FFFF - 0FFFF-FFFF): Invalid

       ------------------------------------------------------------------------
       |  Partition #1   | Current Active Part | Upgrade Partition | Need swap Partition Entry |
       -----------------------------------------------------------------------
       | Invalid           |  Partition #0           |  Partition#1         |  No                                |
       -----------------------------------------------------------------------
       |  Valid             |  Partition #1            | Partition#1          | No                                |
       -----------------------------------------------------------------------

       If FLASH is 1024 KB, the 4 partitions are supported

       Partition 0 (0x0000-0010 - 0x0003FFFF): Gloden image
       Partition 1 (0x0004-0000 - 0x0007FFFF): User image1(upgradable)
       Partition 2 (0x0008-0000 - 0x000BFFFF): User image2(upgradable)
       Partition 2 (0x000C-0000 - 0x0010FFFF): Reserved

       ------------------------------------------------------------------------------------
       |  Partition #1   |  Partition #2  | Current Active Part | Upgrade Partition | Need swap Partition Entry |
       ------------------------------------------------------------------------------------
       | Valid             |  Vaid            |   Partition #1           |  Partition#2         |  Yes                               |
       ------------------------------------------------------------------------------------
       |  Valid            |  Invalid         |  Partition #1            | Partition#2          | Yes                               |
       ------------------------------------------------------------------------------------
       |  Invalid         |  Invalid         |  Partition #0             |  Partition#1         | No                                |
       ------------------------------------------------------------------------------------
     */
    if (qca_partition_entries[2] == 0xFFFFFFFF) {

        /* Partition#2 invlaid, so it use 512KB FLASH */
        A_PRINTF("\nUse 512K FLASH\n");
        _ota_nvram_get_active_image_partition(&current_partition);
        if (partition_index != -1 && partition_index < 2) {
            upgrade_partition = partition_index;
        }

        else {
            upgrade_partition = 1;
        }
    }

    else {

        /* Partition#2 invlaid, so it use 1024KB FLASH */
        A_PRINTF("\nUse 1024KB FLASH\n");
        _ota_nvram_get_active_image_partition(&current_partition);
        if (partition_index != -1 && partition_index < 4) {
            upgrade_partition = partition_index;
        }

        else if (current_partition == 0) {
            upgrade_partition = 1;
        }

        else {
            upgrade_partition = 2;
            ota_need_swap_partition = 1;
        }
    }
    A_PRINTF("current_partition = %d \n", current_partition);
    A_PRINTF("upgrade_partition = %d \n", upgrade_partition);

    /* create UDP socket */
    fd = qcom_socket(PF_INET, SOCK_DGRAM, 0);

    /* init tftp server address */
    serv_addr.sin_addr.s_addr = htonl(ip_addr);
    serv_addr.sin_port = htons(TFTP_SERVER_PORT);
    serv_addr.sin_family = AF_INET;
  retry:total_rx_bytes = 0;

    /* send TFTP_RRQ */
    tftp_state = TFTP_ST_RRQ;
    memset(tftp_tx_buf, 0, TFTP_TX_SIZE);
    _ota_tftp_pkt_rrq(file_name, tftp_tx_buf, &pkt_len);
    nSend =
        qcom_sendto(fd, (char *) tftp_tx_buf, pkt_len, 0, (struct sockaddr *) &serv_addr,
                   sizeof (serv_addr));
    while (1) {
        FD_ZERO(&fd_sockSet);
        FD_SET(fd, &fd_sockSet);
        tmo.tv_sec = TFTP_TIMEOUT / 1000;
        tmo.tv_usec = TFTP_TIMEOUT % 1000;

        /* wait for response from TFTP server */
        fd_act = 0;
        fd_act = qcom_select(fd + 1, &fd_sockSet, NULL, NULL, &tmo);
        if (0 == fd_act) {

            /* timeout, retry */
            A_PRINTF("OTA server response timeout, retry\n");
            retry_times++;
            if (retry_times > 3) {
                ret = QCA_OTA_ERR_SERVER_RSP_TIMEOUT;
                break;
            }

            else {
                qcom_sendto(fd, (char *) tftp_tx_buf, pkt_len, 0, (struct sockaddr *) &serv_addr,
                           sizeof (serv_addr));
            }
        }

        else {
            if (FD_ISSET(fd, &fd_sockSet)) {
                from_size = sizeof (from_addr);
                memset(tftp_rx_buf, 0, TFTP_RX_SIZE);
                nRecv = qcom_recvfrom(fd, (char *) tftp_rx_buf, TFTP_RX_SIZE, 0,
                                     (struct sockaddr *) &from_addr, &from_size);
                if (nRecv > 0) {
                    retry_times = 0;

                    /* parse for TFTP packet */
                    _ota_tftp_pkt_parse(tftp_rx_buf, nRecv, &pkt_seq, &tftp_state);
                    if ((TFTP_ST_OACK == tftp_state) || (TFTP_ST_DATA == tftp_state)) {

                        /* acknolowdge TFTP_ DATA and TFTP_OACK */
                        memset(tftp_tx_buf, 0, TFTP_TX_SIZE);
                        _ota_tftp_pkt_ack(pkt_seq, tftp_tx_buf, &pkt_len);
                        serv_addr.sin_port = from_addr.sin_port;
                        qcom_sendto(fd, (char *) tftp_tx_buf, pkt_len, 0,
                                   (struct sockaddr *) &serv_addr, sizeof (serv_addr));
                    }

                    else if (TFTP_ST_ERROR == tftp_state) {

                        /* response: 0x1002 (file download fail) */
                        ret = QCA_OTA_ERR_IMAGE_DOWNLOAD_FAIL;
                        break;
                    }

                    /* process TFTP_DATA */
                    if ((TFTP_ST_DATA == tftp_state) && (pkt_seq != pkt_seq_last)) {
                        pkt_seq_last = pkt_seq;

                        /* skip TFTP header 4 bytes */
                        data_blk_buf = (tftp_rx_buf + 4);
                        data_blk_size = nRecv - 4;

                        /* special work for first data block */
                        if (pkt_seq == 1) {

                            /* parse image header */
                            if ((ret = _ota_parse_image_hdr(data_blk_buf)) != 0) {

                                /* invalid image header, stop and exit */
                                break;
                            }
                            data_blk_size -= sizeof (QCA_OTA_IMAGE_HDR_t);
                            data_blk_buf += sizeof (QCA_OTA_IMAGE_HDR_t);

                            /*  erase upgrade partition */
                            if (_ota_write_flash) {
                                _ota_nvram_erase_partition(upgrade_partition);
                            }
                        }

                        /* save data block to upgrade partition */
                        if (data_blk_size >= 0) {
                            if (_ota_write_flash) {
                                _ota_nvram_write_data(upgrade_partition, total_rx_bytes + 4,
                                                      data_blk_buf, data_blk_size);
                            }
                            blk_count++;
                            total_rx_bytes += data_blk_size;
                        }

                        /* debug print */
                        if ((blk_count % 20) == 0) {
                            A_PRINTF("* ");
                            if ((blk_count % 400) == 0) {
                                A_PRINTF("\n");
                            }
                        }

                        /* check if image reception complete */
                        if (total_rx_bytes == ota_image_len) {
                            A_PRINTF("\nRecv completed\n");

                            /* calculate image checksum */
                            _ota_calculate_image_checksum(upgrade_partition,
                                                          current_image_checksum);

                            /* checksum check pass */
                            if ((memcmp
                                 (current_image_checksum, ota_image_checksum,
                                  MD5_CHECKSUM_LEN) == 0)) {
                                A_PRINTF("\nchecksum ok\n");

                                /* write MAGIC word for upgrade partition */
                                _ota_nvram_write_partition_magicword(upgrade_partition);

                                /* do partition table entry swap if needed */
                                if (ota_need_swap_partition) {
                                    _ota_nvram_swap_partition_tbl_entry(1, 2);
                                }

                                /* response code: 0 (success) */
                                ret = QCA_OTA_OK;
                                break;
                            }

                            else
                                /* checksum check fail */
                            {
                                A_PRINTF("\nchecksum error\n");

                                /* if retry < maxtime */
                                if (retry_count < QCA_OTA_MAX_TFTP_RETRY_TIMES) {

                                    /* save curernt checksum for later comparision */
                                    memcpy(last_image_checksum, current_image_checksum,
                                           MD5_CHECKSUM_LEN);

                                    /* reset TFTP client and retry */
                                    retry_count++;
                                    goto retry;
                                }

                                else {  /* retry exceeds maximum times */

                                    /* compare current checksum with last checksum */
                                    if (memcmp
                                        (current_image_checksum, last_image_checksum,
                                         MD5_CHECKSUM_LEN) == 0) {
                                        ret = QCA_OTA_ERR_IMAGE_CHECKSUM_INCORRENT;
                                    }

                                    else {
                                        ret = QCA_OTA_ERR_IMAGE_DOWNLOAD_FAIL;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            else {
                A_PRINTF("FD_ISSET fail\n");
            }
        }
    }

    /* close socket */
    qcom_close(fd);
    if (ret == QCA_OTA_OK)
        A_PRINTF("\nOTA upgrade finished!\n");

    else
        A_PRINTF("\nOTA upgrade fail: error code = %d!\n", ret);
    return ret;
}

/* QCA_OTA_upgrade external API */
int
qca_ota_upgrade(unsigned int ip_addr, char *file_name, unsigned int protocol, int partition_index)
{
    int ret;

    /* input parameter sanity check */
    if (!file_name) {
        return QCA_OTA_ERR_INVALID_FILENAME;
    }
    if (protocol > QCA_OTA_PROTOCOL_MAX) {
        return QCA_OTA_ERR_UNSUPPORT_PROTOCOL;
    }

    /* load partition table */
    _ota_nvram_load_partition_table();

    /* do OTA upgrade according with different protocol */
    switch (protocol) {
    case QCA_OTA_PROTOCOL_TFTP:
        ret = _ota_tftp(ip_addr, file_name, partition_index);
        break;
    default:
        ret = QCA_OTA_ERR_UNSUPPORT_PROTOCOL;
        break;
    }
    return ret;
}
