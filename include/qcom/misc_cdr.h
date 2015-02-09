/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */
  
#ifndef __MISC_CDR_H__
#define __MISC_CDR_H__

#include "cdr.h"

typedef struct {
    void *event;
    int completed;
} cdr_misc_completed_t;

typedef struct {
    unsigned int cmd;
    int dev_id;
    int ret;
} wps_init_priv_t;

typedef struct {
    unsigned int cmd;
    char *manufacture;
    char *model_name;
    char *model_num;
    char *serial_num;
    char *dev_name;
}wps_atrrs_t;

typedef struct {
    unsigned int cmd;
    int dev_id;
    wps_atrrs_t wps_atrrs;
    int ret;
}wps_atrrs_priv_t;

typedef struct {
    unsigned int cmd;
    int dev_id;
    int ret;
} wps_cancel_priv_t;

typedef enum {
    UNUSED = 0,
    /*GLB_ATH_TXBUF,*/
    GLB_DHCP_SERVER,
    GLB_DNS_SERVER,
    GLB_DHCP_POOL,
} glb_var_t;

typedef enum {
    GLB_READ = 0,
    GLB_WRITE = 1,
} var_wr_t;

typedef struct {
    unsigned int cmd;
    glb_var_t which;
    void* variable;
    var_wr_t acc;
} glbvar_priv_t;

int cdr_wps_init(int dev_id);
int cdr_wps_set_attrs(int dev_id, wps_atrrs_t *pwps_attrs);
int cdr_wps_cancel(int dev_id);

void cdr_wps_init_request_handle(struct cdr_req_s *cdr_req);
void cdr_wps_set_attrs_handle(struct cdr_req_s *cdr_req);
void cdr_wps_cancel_request_handle(struct cdr_req_s *cdr_req);

void cdr_globle_variable_wr(glb_var_t which, var_wr_t acc, void* val);
void cdr_globle_variable_wr_handle(struct cdr_req_s *cdr_req);

#endif

