/**
******************************************************************************
* @file     zc_hf_adpter.h
* @authors  cxy
* @version  V1.0.0
* @date     10-Sep-2014
* @brief    HANDSHAKE
******************************************************************************
*/

#ifndef  __ZC_QC_ADPTER_H__ 
#define  __ZC_QC_ADPTER_H__

#include <zc_common.h>
#include <zc_protocol_controller.h>
#include <zc_module_interface.h>

typedef struct 
{
    u32 u32FirstFlag;
    qcom_timer_t struHandle;
}QC_TimerInfo;


#define QC_MAX_SOCKET_LEN    (1000)







#ifdef __cplusplus
extern "C" {
#endif
void QC_Init(void);
void QC_WakeUp(void);
void QC_Sleep(void);
void QC_ReadDataFormFlash(void);

#ifdef __cplusplus
}
#endif
#endif
/******************************* FILE END ***********************************/
