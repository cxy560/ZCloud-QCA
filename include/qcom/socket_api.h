/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __SOCKET_API_H__
#define __SOCKET_API_H__

#include <qcom/stdint.h>
#include <qcom/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * It is a replacement of BSD socket(). The parameters are same.
 *						
 * @return				pointer to created socket cast as a int or SOCKET_ERROR
 */
int qcom_socket(int family,int type,int proto);
/**
 * It is a replacement of BSD bind(). The parameters are same.
 *						
 * @return				0 if OK, else one of the socket error codes
 */
int qcom_bind(int s, struct sockaddr * addr,int addrlen);
/**
 * It is a replacement of BSD listen(). The parameters are same.
 *						
 * @return				0 if OK, else one of the socket error codes
 */
int qcom_listen(int s, int backlog);
/**
 * It is a replacement of BSD accept(). The parameters are same.
 *						
 * @return				pointer to socket cast as a int or SOCKET_ERROR
 */
int qcom_accept(int s, struct sockaddr *addr, int *addrlen);
/**
 * set connect timeout
 *
 * no return value
 */
void qcom_set_connect_timeout(int ms);
/**
 * It is a replacement of BSD connect(). The parameters are same.
 *						
 * @return				0 if ok, else one of the socket error codes
 */
int qcom_connect(int s, struct sockaddr * addr, int   addrlen);
/**
 * It is a replacement of BSD setsockopt(). The parameters are same.
 *						
 * @return				0 if ok, else one of the socket error codes
 */
int qcom_setsockopt(int s, int level, int name, void *arg, int arglen);
/**
 * It is a replacement of BSD getsockopt(). The parameters are same.
 *						
 * @return				0 if ok, else one of the socket error codes
 */
int qcom_getsockopt(int s,int level, int name, void *arg,int arglen);
/**
 * It is a replacement of BSD recv(). The parameters are same.
 *						
 * @return				bytes we receive
 */
int qcom_recv(int s, char *buf, int len, int flag);
/**
 * It is a replacement of BSD send(). The parameters are same.
 *						
 * @return				bytes we send
 */
int qcom_send(int s, char *buf, int len, int flags);
/**
 * It is a replacement of BSD sendto(). The parameters are same.
 *						
 * @return				bytes we send
 */
int qcom_sendto(int s, char *buf, int len, int flags, struct sockaddr *to, int tolen);
/**
 * It is a replacement of BSD recvfrom(). The parameters are same.
 *						
 * @return				bytes we recv
 */
int qcom_recvfrom(int s, char *buf, int len,int flags, struct sockaddr *from, int *fromlen);
/**
 * It is a replacement of BSD close(). The parameters are same.
 *						
 * @return				0 on okay or -1 on error
 */
int qcom_close(int s);
#ifdef __cplusplus
}
#endif

#endif

