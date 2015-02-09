/**
******************************************************************************
* @file     zc_protocol_interface.h
* @authors  cxy
* @version  V1.0.0
* @date     10-Sep-2014
* @brief    Security module
******************************************************************************
*/
#ifndef  __ZC_PROTOCOL_INTERFACE_H__ 
#define  __ZC_PROTOCOL_INTERFACE_H__
#include <zc_common.h>

#define ZC_HS_MSG_LEN                       (40)
#define ZC_HS_DEVICE_ID_LEN                 (8)
#define ZC_HS_SESSION_KEY_LEN               (16)
#define ZC_HS_SESSION_IV_LEN                (16)
#define ZC_MODULE_KEY_LEN                   (112)
#define ZC_DOMAIN_LEN                       (8)
#define ZC_EQVERSION_LEN                    (4)


#define ZC_SEC_ALG_NONE                     (0) 
#define ZC_SEC_ALG_RSA                      (1) 
#define ZC_SEC_ALG_AES                      (2) 


#define ZC_OTA_MAX_CHUNK_LEN                (384)

/****************************************************************************************
*message format: 
*|ZC_SecHead||ZC_MessageHead||ZC_MessageOptHead||ZC_MessageOption|.......|ZC_MessagePayload|
*****************************************************************************************/

/*Security Head*/
typedef struct
{
    u16 u16TotalMsg;
    u8  u8SecType;
    u8  u8Resver;
}ZC_SecHead;

/*ZCloud Message*/
typedef struct
{
    u8  Version;
    u8  MsgId;
    u8  MsgCode;
    u8  OptNum;        
    
    u16 Payloadlen;     //msg payload len + opt len + opt head len
    u8  TotalMsgCrc[2];
    
}ZC_MessageHead;

/*ZCloud Option Head*/
typedef struct
{
    u16 OptCode;
    u16 OptLen;
}ZC_MessageOptHead;

/*******************************Message and Option opcode***********************************/

/*ZCloud Message code*/
typedef enum 
{
    ZC_CODE_EQ_BEGIN = 0,
    ZC_CODE_EQ_DONE,
    ZC_CODE_WIFI_CONNECT,
    ZC_CODE_WIFI_DISCONNECT,
    ZC_CODE_CLOUD_CONNECT,
    ZC_CODE_CLOUD_DISCONNECT,
    ZC_CODE_LOCAL_HANDSHAKE,
    ZC_CODE_DESCRIBE,                 
    ZC_CODE_REST,                

    /*HandShake Code*/
    ZC_CODE_HANDSHAKE_1,
    ZC_CODE_HANDSHAKE_2,
    ZC_CODE_HANDSHAKE_3,
    ZC_CODE_HANDSHAKE_4,

    /*Respone Code*/
    ZC_CODE_HEARTBEAT,   /*no payload*/
    ZC_CODE_EMPTY,		 /*no payload, send by moudle when can recv another msg*/
    ZC_CODE_ACK,         /*user define payload*/
    ZC_CODE_ERR,         /*use ZC_ErrorMsg*/

    /*OTA Code*/
    ZC_CODE_OTA_BEGIN,
    ZC_CODE_OTA_FILE_BEGIN,      /*file name, len, version*/
    ZC_CODE_OTA_FILE_CHUNK,
    ZC_CODE_OTA_FILE_END,
    ZC_CODE_OTA_END,
    
    /*OTA Wifi Moudle Code*/
    ZC_CODE_ZOTA_BEGIN,
    ZC_CODE_ZOTA_FILE_BEGIN,      /*file name, len, version*/
    ZC_CODE_ZOTA_FILE_CHUNK,
    ZC_CODE_ZOTA_FILE_END,
    ZC_CODE_ZOTA_END,
    ZC_CODE_BC_INFO,
    ZC_CODE_CLIENT_ACCESS_REQ,
    ZC_CODE_CLIENT_ACCESS_RSP,
    
    ZC_CODE_CLIENT_QUERY_REQ,
    ZC_CODE_CLIENT_QUERY_RSP,

    ZC_CODE_TOKEN_SET,
}ZC_MsgCode;

typedef enum 
{
    ZC_OPT_TRANSPORT = 0,
    ZC_OPT_SSESSION = 1,
}ZC_OptCode;

/*******************************Message definition***********************************/

/*Error Msg*/
typedef struct{
    u8 ErrorCode;
    //u8 ErrorMsg[0];
}ZC_ErrorMsg;


/*first handshake msg, send by moudle to cloud
encrypt by cloud public key*/
typedef struct
{
    u8 RandMsg[ZC_HS_MSG_LEN];
    u8 DeviceId[ZC_HS_DEVICE_ID_LEN];
    u8 u8Domain[ZC_DOMAIN_LEN];
}ZC_HandShakeMsg1;

/*Second handshake msg, send by cloud to moudle, 
encrypt by moudle public key*/
typedef struct
{
    u8 RandMsg[ZC_HS_MSG_LEN];
    u8 SessionKey[ZC_HS_SESSION_KEY_LEN];
}ZC_HandShakeMsg2;

/*3rd handshake msg, send by moudle to cloud,
encrypt by session key*/
typedef struct
{
    u8 RandMsg[ZC_HS_MSG_LEN];
    u8 u8EqVersion[ZC_EQVERSION_LEN];
    u8 u8WifiVerSion[ZC_EQVERSION_LEN];
    u8 u8WifiType;    
    u8 u8Pad[3];
}ZC_HandShakeMsg3;


/*4th handshake msg, send by cloud to moudle,
encrypt by session key*/
typedef struct
{
    u8 RandMsg[ZC_HS_MSG_LEN];
}ZC_HandShakeMsg4;

typedef struct
{
    u8 TokenKey[ZC_HS_SESSION_KEY_LEN];
}ZC_TokenSetReq;

/*msg code: ZC_CODE_DESCRIBE*/
typedef struct 
{
    //u8  u8WifiSwVersion;            
    //u8  u8HwVersion;              
    //u8  u8ArmSwVersion;          
    //u8  u8ZigbeeSwVersion; 
    u8  u8EqVersion[ZC_EQVERSION_LEN];      //eq vision
    u8  u8ModuleKey[ZC_MODULE_KEY_LEN];
    u8  u8Domain[ZC_DOMAIN_LEN];
    u8  u8DeviceId[ZC_HS_DEVICE_ID_LEN];
}ZC_RegisterReq;


/*msg code: ZC_CODE_OTA_BEGIN*/
typedef struct
{
    u8 u8FileNum;
    u8 u8Pad[3];
    //u8 u8FileType[0];
}ZC_OtaBeginReq;

/*msg code: ZC_CODE_OTA_FILE_BEGIN*/
typedef struct
{
    u8 u8FileType;
    u8 u8FileVersion;
    u8  u8TotalFileCrc[2];
    
    u32 u32FileTotalLen;
}ZC_OtaFileBeginReq;

/*msg code: ZC_CODE_OTA_FILE_CHUNK, max data len is ZC_OTA_MAX_CHUNK_LEN*/
typedef struct
{
    u32 u32Offset;
}ZC_OtaFileChunkReq;

/*BC info£¬ send after connect with cloud£¬ in PCT_SEND_BC_MAX_NUM*/
typedef struct
{
    u8 DeviceId[ZC_HS_DEVICE_ID_LEN];
    u8 RandMsg[ZC_HS_MSG_LEN];
}ZC_BroadCastInfo;

/*BC info£¬ send after connect with cloud£¬ in PCT_SEND_BC_MAX_NUM*/
typedef struct
{
    u8 addr[4];
    u8 DeviceId[ZC_HS_DEVICE_ID_LEN];
}ZC_ClientQueryRsp;

/*find eq*/
typedef struct
{
    u8  u8Domain[ZC_DOMAIN_LEN];
}ZC_ClientQueryReq;


/*ZC_CODE_CLIENT_ACCESS_REQ*/
typedef struct
{
    u8 u8ClientNum;
    u8 u8Pad[3];
    //u8 DeviceId[0];
}ZC_ClientAccessInfo;

/*ZC_CODE_CLIENT_ACCESS_RSP*/
typedef struct
{
    u8 u8BlackClientNum;
    u8 u8WhiteClientNum;    
    u8 u8Pad[2];
    //u8 DeviceId[0];
}ZC_BlackWhiteClientList;

/******************************* Option definition***********************************/

/*ZC_OPT_TRANSPORT*/
typedef struct
{
    u8 DeviceId[ZC_HS_DEVICE_ID_LEN];
}ZC_TransportInfo;

/*ZC_OPT_APPDIRECT*/
typedef struct{
    u32 u32SsessionId;
}ZC_SsessionInfo;


#endif
/******************************* FILE END ***********************************/

