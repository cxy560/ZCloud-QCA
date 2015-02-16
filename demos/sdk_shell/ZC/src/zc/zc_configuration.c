/**
******************************************************************************
* @file     zc_configuration.c
* @authors  cxy
* @version  V1.0.0
* @date     10-Sep-2014
* @brief    configuration
******************************************************************************
*/
#include <zc_configuration.h>
#include <zc_protocol_controller.h>

ZC_ConfigDB g_struZcConfigDb;
ZC_RegisterInfo g_struRegisterInfo;
u8 g_u8DefaultCloudKey[ZC_CLOUD_KEY_MAX_LEN] = 
{
    0xb0, 0x7e, 0xab, 0x09,
    0x73, 0x4e, 0x78, 0x12,
    0x7e, 0x8c, 0x54, 0xcd,
    0xbb, 0x93, 0x3c, 0x16,
    0x96, 0x23, 0xaf, 0x7a,
    0xfc, 0xd2, 0x8b, 0xd1,
    0x43, 0xa2, 0xbb, 0xc8,
    0x77, 0xa0, 0xca, 0xcd,
    0x01, 0x00, 0x01, 0x00
};
u8 g_u8DefaultTokenKey[ZC_HS_SESSION_KEY_LEN] = 
{
    0x01, 0x02, 0x03, 0x04,
    0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05
};

/*************************************************
* Function: ZC_ConfigInitPara
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void ZC_ConfigInitPara()
{
    g_struZcConfigDb.u32MagicFlag = ZC_MAGIC_FLAG;
    g_struZcConfigDb.struSwitchInfo.u32SecSwitch = 1;
    g_struZcConfigDb.struSwitchInfo.u32TraceSwitch = 0;
    g_struZcConfigDb.struSwitchInfo.u32WifiConfig = 0;
    g_struZcConfigDb.struSwitchInfo.u32TestAddrConfig = 0;

    memcpy(g_struZcConfigDb.struCloudInfo.u8CloudAddr, "www.ablecloud.cn", ZC_CLOUD_ADDR_MAX_LEN);
    memcpy(g_struZcConfigDb.struCloudInfo.u8CloudKey, g_u8DefaultCloudKey, ZC_CLOUD_KEY_MAX_LEN);
    memcpy(g_struZcConfigDb.struCloudInfo.u8TokenKey, g_u8DefaultTokenKey, ZC_HS_SESSION_KEY_LEN);

    g_struZcConfigDb.struConnection.u32MagicFlag = 0xFFFFFFFF;
}


/*************************************************
* Function: ZC_ConfigPara
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void ZC_ConfigPara(u8 *pu8Data)
{
    ZC_Configuration *pstruConfig;
    pstruConfig = (ZC_Configuration*)pu8Data;

    g_struZcConfigDb.struSwitchInfo.u32SecSwitch = pstruConfig->u32SecSwitch;
    g_struZcConfigDb.struSwitchInfo.u32TraceSwitch = pstruConfig->u32TraceSwitch;
    g_struZcConfigDb.struSwitchInfo.u32WifiConfig = pstruConfig->u32WifiConfig;
    g_struZcConfigDb.struSwitchInfo.u32TestAddrConfig = pstruConfig->u32TestAddrConfig;

    g_struZcConfigDb.struSwitchInfo.u32ServerIp = pstruConfig->u32IpAddr;
    memcpy(g_struZcConfigDb.struSwitchInfo.u8Password, pstruConfig->u8Password, ZC_PASSWORD_MAX_LEN);
    memcpy(g_struZcConfigDb.struSwitchInfo.u8Ssid, pstruConfig->u8Ssid, ZC_SSID_MAX_LEN);
    
    memcpy(g_struZcConfigDb.struCloudInfo.u8CloudAddr, pstruConfig->u8CloudAddr, ZC_CLOUD_ADDR_MAX_LEN);
    memcpy(g_struZcConfigDb.struCloudInfo.u8CloudKey, pstruConfig->u8CloudKey, ZC_CLOUD_KEY_MAX_LEN);

    g_struProtocolController.pstruMoudleFun->pfunWriteFlash((u8*)&g_struZcConfigDb, sizeof(ZC_ConfigDB));
}

/*************************************************
* Function: ZC_StoreRegisterInfo
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void ZC_StoreRegisterInfo(u8 *pu8Data)
{
    ZC_RegisterReq *pstruRegisterMsg;
    pstruRegisterMsg = (ZC_RegisterReq *)pu8Data;
    
    memcpy(g_struRegisterInfo.u8PrivateKey, pstruRegisterMsg->u8ModuleKey, ZC_MODULE_KEY_LEN);
    memcpy(g_struRegisterInfo.u8DeviciId, pstruRegisterMsg->u8DeviceId, ZC_HS_DEVICE_ID_LEN);
    memcpy(g_struRegisterInfo.u8DeviciId + ZC_HS_DEVICE_ID_LEN, pstruRegisterMsg->u8Domain, ZC_DOMAIN_LEN);
    memcpy(g_struRegisterInfo.u8EqVersion, pstruRegisterMsg->u8EqVersion, ZC_EQVERSION_LEN);
}


/*************************************************
* Function: ZC_StoreTokenKey
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void ZC_StoreTokenKey(u8 *pu8Data)
{
    memcpy(g_struZcConfigDb.struCloudInfo.u8TokenKey, pu8Data, ZC_HS_SESSION_KEY_LEN);
    g_struProtocolController.pstruMoudleFun->pfunWriteFlash((u8*)&g_struZcConfigDb, sizeof(ZC_ConfigDB));
}

/*************************************************
* Function: ZC_StoreConnectionInfo
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void ZC_StoreConnectionInfo(u8 *pu8Ssid, u8 *pu8Password)
{
    g_struZcConfigDb.struConnection.u32MagicFlag = ZC_MAGIC_FLAG;
    memcpy(g_struZcConfigDb.struConnection.u8Ssid, pu8Ssid, ZC_SSID_MAX_LEN);
    memcpy(g_struZcConfigDb.struConnection.u8Password, pu8Password, ZC_PASSWORD_MAX_LEN);
    g_struProtocolController.pstruMoudleFun->pfunWriteFlash((u8*)&g_struZcConfigDb, sizeof(ZC_ConfigDB));
}
/*************************************************
* Function: ZC_GetStoreInfor
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void ZC_GetStoreInfor(u8 u8Type, u8 **pu8Data)
{
    switch(u8Type)
    {
        case ZC_GET_TYPE_CLOUDKEY:
            *pu8Data = g_struZcConfigDb.struCloudInfo.u8CloudKey;
            break;
        case ZC_GET_TYPE_DEVICEID:
            *pu8Data = g_struRegisterInfo.u8DeviciId;
            break;
        case ZC_GET_TYPE_PRIVATEKEY:
            *pu8Data = g_struRegisterInfo.u8PrivateKey;
            break;
        case ZC_GET_TYPE_VESION:
            *pu8Data = g_struRegisterInfo.u8EqVersion;
            break;
        case ZC_GET_TYPE_TOKENKEY:
            *pu8Data = g_struZcConfigDb.struCloudInfo.u8TokenKey;
            break;
    }
}


/******************************* FILE END ***********************************/

