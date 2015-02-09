/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */
  
#ifndef __SELECT_API_H__
#define __SELECT_API_H__

#include "timetype.h"

#define MAX_FDSET_NUM 24
#define MAX_FD_NUM 16

typedef struct {
   unsigned long fds_bits[1]; 
} q_fd_set;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * It is a replacement of BSD select(). The parameters are same
 *						
 * @returnnumber of active sockets
 */
int qcom_select(int max, q_fd_set * in, q_fd_set * out, q_fd_set * ev, struct timeval *timeout);

void QCA_FD_ZERO(q_fd_set * set);
void QCA_FD_CLR(int fd, q_fd_set *set);
void QCA_FD_SET(int fd, q_fd_set *set);
int  QCA_FD_ISSET(int fd, q_fd_set *set);
void qca_fdset_clr(int fd);

#undef FD_ZERO
#undef FD_CLR
#undef FD_SET
#undef FD_ISSET

#define FD_ZERO(fdset)  QCA_FD_ZERO(fdset)
#define FD_CLR(fd, fdset)   QCA_FD_CLR(fd, fdset)
#define FD_SET(fd, fdset)   QCA_FD_SET(fd, fdset)
#define FD_ISSET(fd, fdset) QCA_FD_ISSET(fd, fdset)

#ifdef __cplusplus
}
#endif

#endif

