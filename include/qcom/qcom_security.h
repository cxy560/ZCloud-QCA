/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __QCOM__SECURITY_H__
#define __QCOM__SECURITY_H__
#include "stdint.h"

void qcom_sec_md5_init(void);
void qcom_sec_md5_update(unsigned char *data, int len);
void qcom_sec_md5_final(char md5_val[]);

typedef struct aes_cbc_int_api {

    int ( * _aes_128_cbc_encrypt)(const uint8_t *key, const uint8_t *iv, uint8_t *data,
            size_t data_len);
    int (* _aes_128_cbc_decrypt)(const uint8_t *key, const uint8_t *iv, uint8_t *data,
            size_t data_len);

} AES_CBC_INT_API;

extern AES_CBC_INT_API _aes_cbc_internal_api;
#define AES_CBC_FN(fn) _aes_cbc_internal_api.fn

#define aes_128_cbc_encrypt(key, iv, data, data_len)    \
    AES_CBC_FN(_aes_128_cbc_encrypt((key), (iv), (data), (data_len)))

#define aes_128_cbc_decrypt(key, iv, data, data_len)    \
    AES_CBC_FN(_aes_128_cbc_decrypt((key), (iv), (data), (data_len)))

void * secur_aes_decrypt_init(const unsigned char *key, int len);
void * secur_aes_encrypt_init(const unsigned char *key, int len);
void secur_aes_decrypt(void *ctx, const unsigned char *crypt, unsigned char *plain);
void secur_aes_decrypt_deinit(void *ctx);
void secur_aes_encrypt(void *ctx, const unsigned char *plain, unsigned char *crypt);
void secur_aes_encrypt_deinit(void *ctx);
void secur_md5_digest(unsigned char *text, int text_len, unsigned char *key, int key_len, unsigned char *digest);

#endif
