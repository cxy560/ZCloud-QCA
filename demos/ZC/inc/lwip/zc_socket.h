/*******************************************************************************
* @file     zc_module_interface.h
* @authors  cxy
* @version  V1.0.0
* @date     10-Sep-2014
* @brief    Security module
******************************************************************************
*/
#ifndef  __ZC_SOCKET_H__ 
#define  __ZC_SOCKET_H__
#include <zc_common.h>
#include <Socket_api.h>
#include <Select_api.h>
#include <Socket.h>

#ifdef __cplusplus
extern "C" {
#endif
#ifdef QCA
#define zc_socket      qcom_socket
#define zc_connect     qcom_connect
#define zc_bind        qcom_bind     
#define zc_listen      qcom_listen   
#define zc_recv        qcom_recv     
#define zc_recvfrom    qcom_recvfrom 
#define zc_send        qcom_send     
#define zc_sendto      qcom_sendto   
#define zc_select      qcom_select   
#define zc_accept      qcom_accept
#define zc_setsockopt  qcom_setsockopt  
#define zc_close       qcom_close
#define zc_bzero       QCA_FD_ZERO 
#define zc_gethostbyname qcom_dnsc_get_host_by_name
#else  
#define zc_socket      socket
#define zc_connect     connect
#define zc_bind        bind     
#define zc_listen      listen   
#define zc_recv        recv     
#define zc_recvfrom    recvfrom 
#define zc_send        send     
#define zc_sendto      sendto   
#define zc_select      select   
#define zc_accept      accept
#define zc_setsockopt  setsockopt      
#define zc_close       close 
#define zc_bzero       bzero  
#define zc_gethostbyname gethostbyname  
#endif

#ifdef __cplusplus
}
#endif

#endif


/******************************* FILE END **************************/
