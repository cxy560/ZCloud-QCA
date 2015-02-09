/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#ifndef __SOCKET_INTERNAL_H__
#define __SOCKET_INTERNAL_H__

typedef struct {
    void *event;
    int completed;
} cdr_sock_completed_t;

typedef struct {
    unsigned int cmd;
    int family;
    int type;
    int proto;
    int ret;
} open_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    struct sockaddr *addr;
    int addrlen;
    int ret;
} bind_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    int backlog;
    int ret;
} listen_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    struct sockaddr *addr;
    int *addrlen;
    int ret;
} accept_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    struct sockaddr *addr;
    int addrlen;
    int ret;
    int timeout;
}connect_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    int level;
    int name;
    void *arg;
    int arglen;
    int ret;
} setsockopt_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    int level;
    int name;
    void *arg;
    int arglen;
    int ret;
} getsockopt_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    char *buf;
    void *txbuf;
    int len;
    int flags;
    int ret;
} send_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    char *buf;
    void *txbuf;	
    int len;
    int flags;
    struct sockaddr *to;
    int tolen;
    int ret;
} sendto_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    char *buf;
    int len;
    int flag;
    int ret;
} recv_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    char *buf;
    int len;
    int flags;
    struct sockaddr *from;
    int *fromlen;
    int ret;
} recvfrom_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    int ret;
} close_priv_t;

void cdr_socket_open_request_handle(struct cdr_req_s *cdr_req);
void cdr_socket_bind_request_handle(struct cdr_req_s *cdr_req);
void cdr_socket_listen_request_handle(struct cdr_req_s *cdr_req);
void cdr_socket_accept_request_handle(struct cdr_req_s *cdr_req);
void cdr_socket_setsockopt_request_handle(struct cdr_req_s *cdr_req);
void cdr_socket_getsockopt_request_handle(struct cdr_req_s *cdr_req);
void cdr_socket_send_request_handle(struct cdr_req_s *cdr_req);
void cdr_socket_sendto_request_handle(struct cdr_req_s *cdr_req);
void cdr_socket_recv_request_handle(struct cdr_req_s *cdr_req);
void cdr_socket_recvfrom_request_handle(struct cdr_req_s *cdr_req);
void cdr_socket_close_request_handle(struct cdr_req_s *cdr_req);
void cdr_socket_connect_request_handle(struct cdr_req_s* cdr_req);
void cdr_socket_connect_timeout_handle(struct cdr_req_s* cdr_req);

#endif
