/**
******************************************************************************
* @file     zc_qca_adpter.c
* @authors  cxy
* @version  V1.0.0
* @date     10-Sep-2014
* @brief    Event
******************************************************************************
*/


#include <qcom_common.h>
#include <zc_protocol_controller.h>
#include <zc_timer.h>
#include <zc_module_interface.h>
#include <zc_socket.h>
#include <zc_qca_adpter.h>
#include <qcom_wlan.h>
#include <qcom_system.h>
#include <qca_sniffer.h>
#include <qcom_internal.h>


extern PTC_ProtocolCon  g_struProtocolController;
extern A_UINT32 _inet_addr(A_CHAR *str);

PTC_ModuleAdapter g_struQcAdapter;

MSG_Buffer g_struRecvBuffer;
MSG_Buffer g_struRetxBuffer;
MSG_Buffer g_struClientBuffer;


MSG_Queue  g_struRecvQueue;
MSG_Buffer g_struSendBuffer[MSG_BUFFER_SEND_MAX_NUM];
MSG_Queue  g_struSendQueue;

u8 g_u8MsgBuildBuffer[MSG_BULID_BUFFER_MAXLEN];
u8 g_u8UartBuildBuffer[MSG_BULID_BUFFER_MAXLEN];

u8 g_u8ClientSendLen = 0;


u16 g_u16TcpMss;
u16 g_u16LocalPort;


QC_StaInfo g_struQcStaInfo = 
{
    0xFFFFFFFF,
    DEFAULT_IOT_CLOUD_KEY,
    DEFAULT_IOT_PRIVATE_KEY,
    DEFAULT_DEVICIID,
    "www.ablecloud.cn",
    DEFAULT_EQ_VERSION,
    DEFAULT_TOKEN_KEY,
    "test",
    "test"
};
u8 g_u8recvbuffer[QC_MAX_SOCKET_LEN];
ZC_UartBuffer g_struUartBuffer;
QC_TimerInfo g_struQcTimer[ZC_TIMER_MAX_NUM];
int g_Bcfd;
u8  g_u8BcSendBuffer[100];

extern u32 g_u32GloablIp;

u32      g_u32UartFd = PCT_INVAILD_SOCKET;

extern void _ota_nvram_load_partition_table(void);

extern int qca_partition_tbl_base;
extern int qca_partition_entries[4];
extern void _ota_nvram_write_data(int partition, unsigned int offset, unsigned char *buf, int size);
extern void _ota_nvram_read_data(int partition, unsigned int offset, unsigned char *buf, int size);
extern void _ota_nvram_write_partition_magicword(int partition);

s32 g_s32StorePartition = 0;//1024 use 4 partition, 512 use 2 partition
u32 g_u32CloudIp = 0;

/*************************************************
* Function: QC_LoadPartition
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_LoadPartition()
{
    s32 s32ActivePartition = -1;
    _ota_nvram_load_partition_table();
    s32ActivePartition = qcom_nvram_select(-1);
    
    if (qca_partition_entries[2] == 0xFFFFFFFF) 
    {
        g_s32StorePartition = 1;
    }
    else
    {
        g_s32StorePartition = 3;        
    }
}

/*************************************************
* Function: QC_ReadDataFormFlash
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_ReadDataFormFlash(void) 
{
    u32 u32Offset = 0x40000 - sizeof(g_struQcStaInfo);
    _ota_nvram_read_data(g_s32StorePartition, u32Offset, (u8 *)(&g_struQcStaInfo), sizeof(g_struQcStaInfo));
}

/*************************************************
* Function: QC_WriteDataToFlash
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_WriteDataToFlash(void)
{
    u32 u32Offset = 0x40000 - sizeof(g_struQcStaInfo);
    u32 u32EraseOffset;
    u32EraseOffset = u32Offset & 0xFFFFF000;
    
    qcom_nvram_erase(qca_partition_entries[g_s32StorePartition] + u32EraseOffset, 4096);
    
    _ota_nvram_write_data(g_s32StorePartition, u32Offset, (u8 *)(&g_struQcStaInfo), sizeof(g_struQcStaInfo));
}

/*************************************************
* Function: QC_timer_callback
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_timer_callback(unsigned int alarm, void *data) 
{
    u8 u8TimeId;
    qcom_timer_t *pstruTimer;
    pstruTimer = (qcom_timer_t *)data;
    u8TimeId = pstruTimer->arg;
    TIMER_TimeoutAction(u8TimeId);
    TIMER_StopTimer(u8TimeId);
}
/*************************************************
* Function: QC_StopTimer
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_StopTimer(u8 u8TimerIndex)
{
    qcom_timer_stop(&(g_struQcTimer[u8TimerIndex].struHandle));
    qcom_timer_delete(&(g_struQcTimer[u8TimerIndex].struHandle));
}

/*************************************************
* Function: QC_SetTimer
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
u32 QC_SetTimer(u8 u8Type, u32 u32Interval, u8 *pu8TimeIndex)
{
    u8 u8TimerIndex;
    u32 u32Retval;
    
    u32Retval = TIMER_FindIdleTimer(&u8TimerIndex);
    if (ZC_RET_OK == u32Retval)
    {
        TIMER_AllocateTimer(u8Type, u8TimerIndex, (u8*)&g_struQcTimer[u8TimerIndex]);
        g_struQcTimer[u8TimerIndex].struHandle.arg = u8TimerIndex;
        qcom_timer_init(&(g_struQcTimer[u8TimerIndex].struHandle),QC_timer_callback,&g_struQcTimer[u8TimerIndex].struHandle,u32Interval,ONESHOT);
        //g_struQcTimer[u8TimerIndex].u32FirstFlag = 1;
        qcom_timer_start(&(g_struQcTimer[u8TimerIndex].struHandle));        
        *pu8TimeIndex = u8TimerIndex;
    }
    
    return u32Retval;
}


/*************************************************
* Function: QC_SendDataToCloud
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_SendDataToCloud(PTC_Connection *pstruConnection)
{
    MSG_Buffer *pstruBuf = NULL;

    u16 u16DataLen; 
    pstruBuf = (MSG_Buffer *)MSG_PopMsg(&g_struSendQueue); 
    
    if (NULL == pstruBuf)
    {
        return;
    }
    
    u16DataLen = pstruBuf->u32Len; 
    zc_send(pstruConnection->u32Socket, (char*)pstruBuf->u8MsgBuffer, u16DataLen, 0);
 
    ZC_Printf("send data len = %d\n", u16DataLen);
    pstruBuf->u8Status = MSG_BUFFER_IDLE;
    pstruBuf->u32Len = 0;
    return;
}
/*************************************************
* Function: QC_RecvDataFromCloud
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_RecvDataFromCloud(u8 *pu8Data, u32 u32DataLen)
{
    u32 u32RetVal;
    u16 u16PlainLen;
    u32RetVal = MSG_RecvData(&g_struRecvBuffer, pu8Data, u32DataLen);

    if (ZC_RET_OK == u32RetVal)
    {
        if (MSG_BUFFER_FULL == g_struRecvBuffer.u8Status)
        {
            u32RetVal = SEC_Decrypt((ZC_SecHead*)g_struRecvBuffer.u8MsgBuffer, 
                g_struRecvBuffer.u8MsgBuffer + sizeof(ZC_SecHead), g_u8MsgBuildBuffer, &u16PlainLen);

            /*copy data*/
            memcpy(g_struRecvBuffer.u8MsgBuffer, g_u8MsgBuildBuffer, u16PlainLen);

            g_struRecvBuffer.u32Len = u16PlainLen;
            if (ZC_RET_OK == u32RetVal)
            {
                u32RetVal = MSG_PushMsg(&g_struRecvQueue, (u8*)&g_struRecvBuffer);
            }
        }
    }
    
    return;
}
/*************************************************
* Function: QC_FirmwareUpdateFinish
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
u32 QC_FirmwareUpdateFinish(u32 u32TotalLen)
{
    _ota_nvram_write_partition_magicword(g_s32StorePartition);
}


/*************************************************
* Function: QC_FirmwareUpdate
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
u32 QC_FirmwareUpdate(u8 *pu8FileData, u32 u32Offset, u32 u32DataLen)
{
    u32 u32EraseOffset;
    u32 u32HeadLen;
    u32 u32WritLen = 0;
    u32 u32DataStartOffset = 0;
    u32 u32FlashStartOffset = 0;

    u32HeadLen = 40;//sizeof(QCA_OTA_IMAGE_HDR_t);

    /*first erase partition, but not to erase last sector, last sector is sta store infor*/
    u32EraseOffset = (0x40000 - sizeof(g_struQcStaInfo)) & 0xFFFFF000;
    if (0 == u32Offset)
    {
        qcom_nvram_erase(qca_partition_entries[g_s32StorePartition], u32EraseOffset);
    }

    if ((u32Offset + u32DataLen) <= u32HeadLen)
    {
        return ZC_RET_OK;
    }
    else if (((u32Offset + u32DataLen) > u32HeadLen) && (u32Offset < u32HeadLen))
    {
        u32WritLen =  u32DataLen - (u32HeadLen - u32Offset);  
        u32DataStartOffset = u32HeadLen - u32Offset;
        u32FlashStartOffset = 0;
    }
    else
    {
        u32WritLen = u32DataLen;  
        u32DataStartOffset = 0;
        u32FlashStartOffset = (u32Offset - u32HeadLen);
    }

    //offset need add a magicflag size
    _ota_nvram_write_data(g_s32StorePartition, u32FlashStartOffset + 4, pu8FileData + u32DataStartOffset, u32WritLen);

    return ZC_RET_OK;
    
}
/*************************************************
* Function: QC_SendDataToMoudle
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
u32 QC_SendDataToMoudle(u8 *pu8Data, u16 u16DataLen)
{
    u8 u8MagicFlag[4] = {0x02,0x03,0x04,0x05};
    A_UINT32 u32OutLen;

    u32OutLen = 4;
    qcom_uart_write(g_u32UartFd,(char*)u8MagicFlag,&u32OutLen); 

    u32OutLen = u16DataLen;   
    qcom_uart_write(g_u32UartFd,(char*)pu8Data,&u32OutLen); 
    return ZC_RET_OK;
}

/*************************************************
* Function: QC_GetStoreInfor
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
u32 QC_GetStoreInfor(u8 u8Type, u8 **pu8Data)
{
    switch(u8Type)
    {
        case ZC_GET_TYPE_CLOUDKEY:
            *pu8Data = g_struQcStaInfo.u8CloudKey;
            break;
        case ZC_GET_TYPE_DEVICEID:
            *pu8Data = g_struQcStaInfo.u8DeviciId;
            break;
        case ZC_GET_TYPE_PRIVATEKEY:
            *pu8Data = g_struQcStaInfo.u8PrivateKey;
            break;
        case ZC_GET_TYPE_VESION:
            *pu8Data = g_struQcStaInfo.u8EqVersion;
            break;
        case ZC_GET_TYPE_TOKENKEY:
            *pu8Data = g_struQcStaInfo.u8TokenKey;
            break;
    }
    return ZC_RET_OK;
}
/*************************************************
* Function: QC_SnifferSuccess
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_SnifferSuccess(char *ssid, char *password, unsigned char response)
{
    ZC_Printf("sniffer_success:\n");
    ZC_Printf("ssid: %s\n", ssid);
    ZC_Printf("password: %s\n", password);

    if (((strlen(ssid) + 1) > 32) || ((strlen(password) + 1) > 64))
    {
        return;
    }
    g_struQcStaInfo.u32Magic = ZC_MAGIC_FLAG;
    memcpy(g_struQcStaInfo.u8Ssid, ssid, strlen(ssid) + 1);
    memcpy(g_struQcStaInfo.u8Password, password, strlen(password) + 1);
    
    QC_WriteDataToFlash();

    qcom_sys_reset();
}


/*************************************************
* Function: QC_Rest
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_Rest(void)
{
    g_struQcStaInfo.u32Magic = 0xFFFFFFFF;
    
    QC_WriteDataToFlash();

    qcom_sys_reset();
}
/*************************************************
* Function: QC_SendDataToNet
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_SendDataToNet(u32 u32Fd, u8 *pu8Data, u16 u16DataLen, ZC_SendParam *pstruParam)
{
    zc_send(u32Fd, (char*)pu8Data, u16DataLen, 0);
}

/*************************************************
* Function: QC_StoreRegisterInfor
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
u32 QC_StoreRegisterInfor(u8 u8Type, u8 *pu8Data, u16 u16DataLen)
{
    switch(u8Type)
    {
        case 0:
        {
            ZC_RegisterReq *pstruRegister;
            
            pstruRegister = (ZC_RegisterReq *)(pu8Data);
            
            memcpy(g_struQcStaInfo.u8PrivateKey, pstruRegister->u8ModuleKey, ZC_MODULE_KEY_LEN);
            memcpy(g_struQcStaInfo.u8DeviciId, pstruRegister->u8DeviceId, ZC_HS_DEVICE_ID_LEN);
            memcpy(g_struQcStaInfo.u8DeviciId + ZC_HS_DEVICE_ID_LEN, pstruRegister->u8Domain, ZC_DOMAIN_LEN);
            memcpy(g_struQcStaInfo.u8EqVersion, pstruRegister->u8EqVersion, ZC_EQVERSION_LEN);
        
            break;
        }
        case 1:
        {
            memcpy(g_struQcStaInfo.u8TokenKey, pu8Data, u16DataLen);
            QC_WriteDataToFlash();
            break;
        }
        default:
            break;
    }
    
    return ZC_RET_OK;
}
/*************************************************
* Function: QC_SendClientQueryReq
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_SendClientQueryReq(u8 *pu8Msg, u16 u16RecvLen)
{
    ZC_MessageHead *pstruMsg;
    struct sockaddr_in addr;
    ZC_ClientQueryRsp struRsp;
    u16 u16Len;
    u8 *pu8DeviceId;
    u8 *pu8Domain;    
    u32 u32Index;
    ZC_ClientQueryReq *pstruQuery;

    if (g_struProtocolController.u8MainState < PCT_STATE_ACCESS_NET)
    {
        return;
    }
    
    if (u16RecvLen != sizeof(ZC_MessageHead) + sizeof(ZC_ClientQueryReq))
    {
        return;
    }
    
    pstruMsg = (ZC_MessageHead *)pu8Msg;
    pstruQuery = (ZC_ClientQueryReq *)(pstruMsg + 1);

    if (ZC_CODE_CLIENT_QUERY_REQ != pstruMsg->MsgCode)
    {
        return;
    }
    g_struProtocolController.pstruMoudleFun->pfunGetStoreInfo(ZC_GET_TYPE_DEVICEID, &pu8DeviceId);
    pu8Domain = pu8DeviceId + ZC_HS_DEVICE_ID_LEN;

    /*Only first 6 bytes is vaild*/
    for (u32Index = 0; u32Index < 6; u32Index++)
    {
        if (pstruQuery->u8Domain[u32Index] != pu8Domain[u32Index])
        {
            return;
        }
        
    }


    memset((char*)&addr,0,sizeof(addr));
    addr.sin_family = AF_INET; 
    addr.sin_port = htons(ZC_MOUDLE_BROADCAST_PORT); 
    addr.sin_addr.s_addr=_inet_addr("255.255.255.255"); 
    
    struRsp.addr[3] = g_u32GloablIp & 0xff;
    struRsp.addr[2] = (g_u32GloablIp >> 8) & 0xff;
    struRsp.addr[1] = (g_u32GloablIp >> 16) & 0xff;
    struRsp.addr[0] = (g_u32GloablIp >> 24)  & 0xff;
    
    memcpy(struRsp.DeviceId, pu8DeviceId, ZC_HS_DEVICE_ID_LEN);
    EVENT_BuildMsg(ZC_CODE_CLIENT_QUERY_RSP, 0, g_u8MsgBuildBuffer, &u16Len, (u8*)&struRsp, sizeof(ZC_ClientQueryRsp));
    zc_sendto(g_Bcfd,(char*)g_u8MsgBuildBuffer,u16Len,0,(struct sockaddr *)&addr,sizeof(addr)); 
}

/*************************************************
* Function: QC_CloudRecvfunc
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_CloudRecvfunc() 
{
    s32 s32RecvLen=0; 
    q_fd_set fdread;
    u32 u32Index;
    u32 u32Len=0; 
    u32 u32ActiveFlag = 0;
    struct sockaddr_in cliaddr;
    int connfd;
    u32 u32MaxFd = 0;
    struct timeval timeout; 
    struct sockaddr_in addr;
    int tmp=1;
    int fd_act = 0;
    A_UINT32 uartrecv = MSG_BULID_BUFFER_MAXLEN;
    A_UINT32 bufferlen = MSG_BULID_BUFFER_MAXLEN;

    ZC_StartClientListen();

    u32ActiveFlag = 0;
    
    timeout.tv_sec= 0; 
    timeout.tv_usec= 1000; 
    
    FD_ZERO(&fdread);

    FD_SET(g_Bcfd, &fdread);
    u32MaxFd = u32MaxFd > g_Bcfd ? u32MaxFd : g_Bcfd;

    if (PCT_INVAILD_SOCKET != g_u32UartFd)
    {
        FD_SET(g_u32UartFd, &fdread);
        u32MaxFd = u32MaxFd > g_u32UartFd ? u32MaxFd : g_u32UartFd;
        u32ActiveFlag = 1;
    }
    
    if (PCT_INVAILD_SOCKET != g_struProtocolController.struClientConnection.u32Socket)
    {
        FD_SET(g_struProtocolController.struClientConnection.u32Socket, &fdread);
        u32MaxFd = u32MaxFd > g_struProtocolController.struClientConnection.u32Socket ? u32MaxFd : g_struProtocolController.struClientConnection.u32Socket;
        u32ActiveFlag = 1;
    }
    

    if ((g_struProtocolController.u8MainState >= PCT_STATE_WAIT_ACCESSRSP) 
    && (g_struProtocolController.u8MainState < PCT_STATE_DISCONNECT_CLOUD))
    {
        FD_SET(g_struProtocolController.struCloudConnection.u32Socket, &fdread);
        u32MaxFd = u32MaxFd > g_struProtocolController.struCloudConnection.u32Socket ? u32MaxFd : g_struProtocolController.struCloudConnection.u32Socket;
        u32ActiveFlag = 1;
    }

    for (u32Index = 0; u32Index < ZC_MAX_CLIENT_NUM; u32Index++)
    {
        if (0 == g_struClientInfo.u32ClientVaildFlag[u32Index])
        {
            FD_SET(g_struClientInfo.u32ClientFd[u32Index], &fdread);
            u32MaxFd = u32MaxFd > g_struClientInfo.u32ClientFd[u32Index] ? u32MaxFd : g_struClientInfo.u32ClientFd[u32Index];
            u32ActiveFlag = 1;            
        }
    }
    if (0 == u32ActiveFlag)
    {
        return;
    }
    
    fd_act = 0;
    fd_act = zc_select(u32MaxFd + 1, &fdread, NULL, NULL, &timeout);

    if (0 == fd_act)
    {
        return;
    }

    
    if (PCT_INVAILD_SOCKET != g_u32UartFd)
    {
        if (FD_ISSET(g_u32UartFd, &fdread))
        {
            bufferlen = MSG_BULID_BUFFER_MAXLEN;
            uartrecv = qcom_uart_read(g_u32UartFd, (char*)g_u8UartBuildBuffer, &bufferlen);
            if (bufferlen > 0)
            {
                qcom_thread_msleep(100);
                ZC_Moudlefunc(g_u8UartBuildBuffer, bufferlen);
            }
        }
    }
    
    if ((g_struProtocolController.u8MainState >= PCT_STATE_WAIT_ACCESSRSP) 
    && (g_struProtocolController.u8MainState < PCT_STATE_DISCONNECT_CLOUD))
    {
        if (FD_ISSET(g_struProtocolController.struCloudConnection.u32Socket, &fdread))
        {
            s32RecvLen = zc_recv(g_struProtocolController.struCloudConnection.u32Socket, (char*)g_u8recvbuffer, QC_MAX_SOCKET_LEN, 0); 
            
            if(s32RecvLen > 0) 
            {
                ZC_Printf("recv data len = %d\n", s32RecvLen);
                QC_RecvDataFromCloud(g_u8recvbuffer, s32RecvLen);
            }
            else
            {
                ZC_Printf("recv error, len = %d\n",s32RecvLen);
                PCT_DisConnectCloud(&g_struProtocolController);
                
                g_struUartBuffer.u32Status = MSG_BUFFER_IDLE;
                g_struUartBuffer.u32RecvLen = 0;
            }
        }
        
    }

    for (u32Index = 0; u32Index < ZC_MAX_CLIENT_NUM; u32Index++)
    {
        if (0 == g_struClientInfo.u32ClientVaildFlag[u32Index])
        {
            if (FD_ISSET(g_struClientInfo.u32ClientFd[u32Index], &fdread))
            {
                s32RecvLen = zc_recv(g_struClientInfo.u32ClientFd[u32Index], (char*)g_u8recvbuffer, QC_MAX_SOCKET_LEN, 0); 
                if (s32RecvLen > 0)
                {
                    ZC_RecvDataFromClient(g_struClientInfo.u32ClientFd[u32Index], g_u8recvbuffer, s32RecvLen);
                }
                else
                {   
                    ZC_ClientDisconnect(g_struClientInfo.u32ClientFd[u32Index]);
                    zc_close(g_struClientInfo.u32ClientFd[u32Index]);
                }
                
            }
        }
        
    }

    if (PCT_INVAILD_SOCKET != g_struProtocolController.struClientConnection.u32Socket)
    {
        if (FD_ISSET(g_struProtocolController.struClientConnection.u32Socket, &fdread))
        {
        
            connfd = zc_accept(g_struProtocolController.struClientConnection.u32Socket,(struct sockaddr *)&cliaddr,(int*)&u32Len);

            if (ZC_RET_ERROR == ZC_ClientConnect((u32)connfd))
            {
                zc_close(connfd);
            }
            else
            {
                ZC_Printf("accept client = %d", connfd);
            }
        } 
    }

    if (FD_ISSET(g_Bcfd, &fdread))
    {
        tmp = sizeof(addr); 
        s32RecvLen = zc_recvfrom(g_Bcfd, (char*)g_u8BcSendBuffer, 100, 0, (struct sockaddr *)&addr, (socklen_t*)&tmp); 
        if(s32RecvLen > 0) 
        {
            QC_SendClientQueryReq(g_u8BcSendBuffer, (u16)s32RecvLen);
        } 
    }
}
/*************************************************
* Function: QC_GenRand
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
u32 QC_GenRand()
{
    return time_ms();
}



/*************************************************
* Function: QC_Rand
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_Rand(u8 *pu8Rand)
{
    u32 u32Rand;
    u32 u32Index; 
    for (u32Index = 0; u32Index < 10; u32Index++)
    {
        u32Rand = QC_GenRand();
        pu8Rand[u32Index * 4] = ((u8)u32Rand % 26) + 65;
        pu8Rand[u32Index * 4 + 1] = ((u8)(u32Rand >> 8) % 26) + 65;
        pu8Rand[u32Index * 4 + 2] = ((u8)(u32Rand >> 16) % 26) + 65;
        pu8Rand[u32Index * 4 + 3] = ((u8)(u32Rand >> 24) % 26) + 65;        
    }
}

/*************************************************
* Function: QC_ConnectToCloud
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
u32 QC_ConnectToCloud(PTC_Connection *pstruConnection)
{
    int fd; 
    struct sockaddr_in addr;
    
    memset((char*)&addr,0,sizeof(addr));
    ZC_Printf("connect to cloud\n");
    
    
    memset(&addr, 0, sizeof (struct sockaddr_in));   
    addr.sin_family = AF_INET;
    addr.sin_port = htons(ZC_CLOUD_PORT);
    addr.sin_addr.s_addr = htonl(g_u32CloudIp);
    fd = zc_socket(AF_INET, SOCK_STREAM, 0);

    if(fd<0)
        return ZC_RET_ERROR;
    
    if (zc_connect(fd, (struct sockaddr *)&addr, sizeof(addr))< 0)
    {
        zc_close(fd);
        return ZC_RET_ERROR;
    }

    ZC_Printf("connect ok!\n");
    g_struProtocolController.struCloudConnection.u32Socket = fd;

    
    QC_Rand(g_struProtocolController.RandMsg);

    return ZC_RET_OK;
}
/*************************************************
* Function: QC_ConnectToCloud
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
u32 QC_ListenClient(PTC_Connection *pstruConnection)
{
    int fd; 
    struct sockaddr_in servaddr;

    fd = zc_socket(AF_INET, SOCK_STREAM, 0);
    if(fd<0)
        return ZC_RET_ERROR;

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port = htons(pstruConnection->u16Port);
    if(zc_bind(fd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
    {
        zc_close(fd);
        return ZC_RET_ERROR;
    }
    
    if (zc_listen(fd, 10)< 0)
    {
        zc_close(fd);
        return ZC_RET_ERROR;
    }

    ZC_Printf("Tcp Listen Port = %d\n", pstruConnection->u16Port);
    g_struProtocolController.struClientConnection.u32Socket = fd;

    return ZC_RET_OK;
}

/*************************************************
* Function: QC_BcInit
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_BcInit()
{
    int tmp=1;
    struct sockaddr_in addr; 

    addr.sin_family = AF_INET; 
    addr.sin_port = htons(ZC_MOUDLE_PORT); 
    addr.sin_addr.s_addr=htonl(INADDR_ANY);

    g_Bcfd = zc_socket(AF_INET, SOCK_DGRAM, 0); 

    tmp=1; 
    zc_setsockopt(g_Bcfd, SOL_SOCKET,SO_BROADCAST,&tmp,sizeof(tmp)); 
    zc_bind(g_Bcfd, (struct sockaddr*)&addr, sizeof(addr)); 

    return;
}
/*************************************************
* Function: QC_SendBc
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_SendBc()
{
    struct sockaddr_in addr;
    u16 u16Len;
    static int sleepcount = 0;

    if (PCT_STATE_CONNECT_CLOUD != g_struProtocolController.u8MainState)
    {
        sleepcount = 0;
        return;
    }
    sleepcount++;
    if (sleepcount > 800)
    {
        memset((char*)&addr,0,sizeof(addr));
        addr.sin_family = AF_INET; 
        addr.sin_port = htons(ZC_MOUDLE_BROADCAST_PORT); 
        addr.sin_addr.s_addr=htonl(_inet_addr("255.255.255.255")); 
        
        EVENT_BuildBcMsg(g_u8MsgBuildBuffer, &u16Len);

        if (g_struProtocolController.u16SendBcNum < (PCT_SEND_BC_MAX_NUM))
        {
           zc_sendto(g_Bcfd,(char*)g_u8MsgBuildBuffer,u16Len,0,(struct sockaddr *)&addr,sizeof(addr)); 
           g_struProtocolController.u16SendBcNum++;
        }
        sleepcount = 0;
    }
    
}
/*************************************************
* Function: QC_GetDhcp
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_GetDhcp()
{
    A_UINT32 ip = 0;
    A_UINT32 submask = 0xFFFFFF00;
    A_UINT32 gw = 0;


    qcom_ip_address_set(ip, submask, gw);


    while(1)
    {
        QC_CloudRecvfunc();
        /* dhcp */
        qcom_dhcpc_enable(1);

        qcom_thread_msleep(5000);

        qcom_ip_address_get(&ip, &submask, &gw);
        if ((ip == 0) ||
            (((ip >> 24 & 0xff) == 169)
             && ((ip >> 16 & 0xFF) == 254))) 
        {
            continue;
        }
        else 
        {
            g_u32GloablIp = ip; 
            break;
        }
    }

}
/*************************************************
* Function: QC_SetNetwork
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_SetNetwork()
{
    A_UINT8 wifi_state;
    int retval;

    wifi_state = 0;
    while(wifi_state != 4)
    {
        QC_CloudRecvfunc();
        
        qcom_op_set_mode(QCOM_WLAN_DEV_MODE_STATION);

        qcom_sec_set_passphrase((char*)g_struQcStaInfo.u8Password);//("2882135be2bb");//
        
        qcom_sta_connect_with_scan((char*)g_struQcStaInfo.u8Ssid);//("InFocus M810t");
        qcom_thread_msleep(5000);

        qcom_get_state(&wifi_state);
        if (wifi_state == 4) 
        {
            QC_GetDhcp();   
            qcom_dnsc_enable(1);

            while(0 == g_u32CloudIp)
            {
                QC_CloudRecvfunc();
                retval = zc_gethostbyname((A_CHAR *)"test.ablecloud.cn"/*g_struQcStaInfo.u8CloudAddr*/, &g_u32CloudIp);
                qcom_thread_msleep(5000);
                if (A_OK != retval)
                {
                    g_u32CloudIp = 0;
                    continue;
                }    
            }
            ZC_Printf("cloud ip = %d\n", g_u32CloudIp);
            ZC_Printf("Connect with wifi\n");
            
            QC_WakeUp();
            break;
        }
        qcom_disconnect();
    }
}

/*************************************************
* Function: QC_Cloudfunc
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_Cloudfunc(ULONG which_thread) 
{
    int fd;
    u32 u32Timer = 0;
    A_UINT8 u8WifiStatus;

    if (g_struQcStaInfo.u32Magic != ZC_MAGIC_FLAG)
    {
        sniffer_start(SCAN_CHANNEL_MODE, QC_SnifferSuccess);
    }
    else
    {
        ZC_Printf("in cloud\n");

        QC_SetNetwork();

        QC_BcInit();

        while(1) 
        {
            fd = g_struProtocolController.struCloudConnection.u32Socket;
            QC_CloudRecvfunc();
            PCT_Run();
            
            if (PCT_STATE_DISCONNECT_CLOUD == g_struProtocolController.u8MainState)
            {
                zc_close(fd);
                u32Timer = (PCT_TIMER_INTERVAL_RECONNECT) * (QC_GenRand() % 10 + 1);
                PCT_ReconnectCloud(&g_struProtocolController, u32Timer);
                g_struUartBuffer.u32Status = MSG_BUFFER_IDLE;
                g_struUartBuffer.u32RecvLen = 0;
                qcom_thread_msleep(u32Timer);
            }
            else
            {
                QC_SendDataToCloud(&g_struProtocolController.struCloudConnection);
            }
            QC_SendBc();
            qcom_get_state(&u8WifiStatus);
            if (u8WifiStatus != QCOM_WLAN_LINK_STATE_CONNECTED_STATE)
            {
               QC_Sleep();
               QC_SetNetwork();
            }

        } 
    }

    
}
/*************************************************
* Function: QC_Init
* Description:  Uart0:  communication
*                 Uart1\A3\BA debug 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_UartInit()
{
    qcom_uart_para com_uart_cfg;//dbg_uart_cfg;

    com_uart_cfg.BaudRate = 115200;
    com_uart_cfg.number = 8;
    com_uart_cfg.StopBits = 1;
    com_uart_cfg.parity = 0;
    com_uart_cfg.FlowControl = 0;


    //qcom_uart_rx_pin_set(6, 10);
    //qcom_uart_tx_pin_set(7, 11);
    qcom_uart_rx_pin_set(10, 6);
    qcom_uart_tx_pin_set(11, 7);
    qcom_uart_init();

    g_u32UartFd = qcom_uart_open((A_CHAR *)"UART0");

    qcom_set_uart_config((A_CHAR *)"UART0", &com_uart_cfg);

}

/*************************************************
* Function: QC_Init
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_Init()
{
    QC_UartInit();

    QC_LoadPartition();

    ZC_Printf("MT Init\n");
    g_struQcAdapter.pfunConnectToCloud = QC_ConnectToCloud;
    g_struQcAdapter.pfunListenClient = QC_ListenClient;
    g_struQcAdapter.pfunSendToNet = QC_SendDataToNet;   
    g_struQcAdapter.pfunUpdate = QC_FirmwareUpdate;     
    g_struQcAdapter.pfunUpdateFinish = QC_FirmwareUpdateFinish;
    g_struQcAdapter.pfunSendToMoudle = QC_SendDataToMoudle;  
    g_struQcAdapter.pfunStoreInfo = QC_StoreRegisterInfor;
    g_struQcAdapter.pfunGetStoreInfo = QC_GetStoreInfor;
    g_struQcAdapter.pfunSetTimer = QC_SetTimer;   
    g_struQcAdapter.pfunStopTimer = QC_StopTimer;
    
    g_struQcAdapter.pfunRest = QC_Rest;
    g_u16TcpMss = 1000;
    PCT_Init(&g_struQcAdapter);

    g_struUartBuffer.u32Status = MSG_BUFFER_IDLE;
    g_struUartBuffer.u32RecvLen = 0;

    QC_ReadDataFormFlash();

}

/*************************************************
* Function: QC_WakeUp
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_WakeUp()
{
    PCT_WakeUp();
}

/*************************************************
* Function: QC_Sleep
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void QC_Sleep()
{
    PCT_Sleep();
    
    g_struUartBuffer.u32Status = MSG_BUFFER_IDLE;
    
    g_struUartBuffer.u32RecvLen = 0;
    
    
   
}
/******************************* FILE END *****************/



