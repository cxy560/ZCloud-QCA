/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#ifndef _SSL_LIB_H
#define _SSL_LIB_H

#define	SSL_ERR  (-1)	 
#define	SSL_OK	1

#define SSL_INVALID_SOCK (-1)
#define SSL_MAX_CA_LEN		1000
#define SSL_MAX_CERT_LEN	1000
#define SSL_MAX_PRIV_KEY_LEN	1000
#define CRED_NAME_MAX_LEN	16


#define SSL_PROTOCOL_UNKNOWN                  0x00
#define SSL_PROTOCOL_SSL_3_0                  0x30
#define SSL_PROTOCOL_TLS_1_0                  0x31
#define SSL_PROTOCOL_TLS_1_1                  0x32 
#define SSL_PROTOCOL_TLS_1_2                  0x33

#define SSL_STATE_OK 0x03


typedef void * (*SSL_METHOD)(void);

typedef struct ssl_con {
	A_INT32 fd;
	void *con;
}SSL;


typedef struct ssl_ctx{
    void *sharkSslCtx;
    SSL_METHOD sslMethod;
    A_UINT8 sslVer;
    char certName[CRED_NAME_MAX_LEN];
    char privKeyName[CRED_NAME_MAX_LEN];
    char caName[CRED_NAME_MAX_LEN];
    A_UINT8 *passKey;
    void *certStore;
}SSL_CTX;

extern int SSL_library_init(void );
extern void *SSLv3_server_method(void);
extern void *SSLv3_client_method(void);

extern void *TLSv1_server_method(void);
extern void *TLSv1_client_method(void);

extern void *TLSv1_1_server_method(void);
extern void *TLSv1_1_client_method(void);

extern void *TLSv1_2_server_method(void);
extern void *TLSv1_2_client_method(void);

extern SSL_CTX * SSL_CTX_new(SSL_METHOD method);
extern void SSL_CTX_free(SSL_CTX * ctx);
extern SSL* SSL_new(SSL_CTX *ctx);
extern int SSL_connect(SSL *ssl);
extern int SSL_accept(SSL *ssl);
extern int SSL_shutdown(SSL *ssl);
extern void SSL_free(SSL *ssl);
extern int SSL_read(SSL *ssl, char *buf, int num);
extern int SSL_write(SSL *ssl, char *buf, int num);
extern int SSL_set_fd(SSL *ssl, int fd);
extern int SSL_get_fd(const SSL *ssl);
extern int SSL_version(const SSL *ssl);
extern char *SSL_get_version(SSL *ssl);
extern const char *SSL_get_cipher(SSL *ssl);
extern int SSL_state(SSL *ssl);

extern int SSL_CTX_add_file(char *name, char *buf, unsigned int len);
extern int SSL_CTX_check_private_key(const SSL_CTX *ctx);
extern int SSL_CTX_del_file(char *name);
extern int SSL_CTX_load_verify_locations(SSL_CTX *ctx, char *name);
extern int SSL_CTX_use_PrivateKey_file(SSL_CTX *ctx, char *name);
extern int SSL_CTX_use_certificate_file(SSL_CTX *ctx, char *name);
extern void SSL_CTX_set_default_passwd_cb_userdata(SSL_CTX *ctx,void *u);

extern const char *SSL_get_cipher(SSL *ssl);
extern void *SSL_get_peer_certificate(const SSL *ssl);

#endif /*_SSL_LIB_H */
