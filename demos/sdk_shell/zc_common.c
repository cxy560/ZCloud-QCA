/**
******************************************************************************
* @file     zc_common.c
* @authors  cxy
* @version  V1.0.0
* @date     10-Sep-2014
* @brief   
******************************************************************************
*/

#include <zc_common.h>
#include <zc_protocol_controller.h>
#include <zc_sec_engine.h>
 

u32 g_u32TraceSwitch = 0;
u32  g_u32LoopFlag = 0;
u32  g_u32SecSwitch = 0;

/*************************************************
* Function: ZC_TraceData
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void ZC_TraceData(u8* pData, u32 Len)
{
    u32 Index;
    if (0 == g_u32TraceSwitch)
    {
        return;
    }
    if (0 == Len)
    {
        return;
    }
    
    ZC_Printf("++++++++++++++++++++++++++++++++++++++++++++++++\n");
    for (Index = 0; Index + 4 < Len; Index = Index + 4)
    {
        ZC_Printf("0x%02x, 0x%02x, 0x%02x, 0x%02x,\n",
            pData[Index],
            pData[Index + 1],
            pData[Index + 2],
            pData[Index + 3]);
    }
    
    for (; Index < Len - 1; Index++)
    {
        ZC_Printf("0x%02x, ", pData[Index]);
    }
    ZC_Printf("0x%02x", pData[Index]);

    ZC_Printf("\n++++++++++++++++++++++++++++++++++++++++++++++++\n");
}



