/**
 *     ____             _________                __                _
 *    / __ \___  ____ _/ /_  __(_)___ ___  ___  / /   ____  ____ _(_)____
 *   / /_/ / _ \/ __ `/ / / / / / __ `__ \/ _ \/ /   / __ \/ __ `/ / ___/
 *  / _, _/  __/ /_/ / / / / / / / / / / /  __/ /___/ /_/ / /_/ / / /__
 * /_/ |_|\___/\__,_/_/ /_/ /_/_/ /_/ /_/\___/_____/\____/\__, /_/\___/
 *                                                       /____/
 *
 *                 SharkSSL Embedded SSL/TLS Stack
 ****************************************************************************
 *   PROGRAM MODULE
 *
 *   $Id: //depot/sw/branches/olca_IOT_3_1_fc/target/import/rev7/include/sharkSSL/SharkSslCrypto.h#1 $
 *
 *   COPYRIGHT:  Real Time Logic LLC, 2010
 *
 *   This software is copyrighted by and is the sole property of Real
 *   Time Logic LLC.  All rights, title, ownership, or other interests in
 *   the software remain the property of Real Time Logic LLC.  This
 *   software may only be used in accordance with the terms and
 *   conditions stipulated in the corresponding license agreement under
 *   which the software has been supplied.  Any unauthorized use,
 *   duplication, transmission, distribution, or disclosure of this
 *   software is expressly forbidden.
 *
 *   This Copyright notice may not be removed or modified without prior
 *   written consent of Real Time Logic LLC.
 *
 *   Real Time Logic LLC. reserves the right to modify this software
 *   without notice.
 *
 *               http://www.realtimelogic.com
 *               http://www.sharkssl.com
 ****************************************************************************
 *
 */
#ifndef _SharkSslCrypto_h
#define _SharkSslCrypto_h

#define SHARKSSL_LIB 1
#include "SharkSSL.h"


typedef struct SharkSslMd5Ctx
{
    U32 total[2];
    U32 state[4];
    U8  buffer[64];
} SharkSslMd5Ctx;


typedef struct SharkSslSha1Ctx
{
    U32 total[2];
    U32 state[5];
    U8  buffer[64];
} SharkSslSha1Ctx;


#if SHARKSSL_USE_SHA_256
typedef struct SharkSslSha256Ctx
{
    U32 total[2];
    U32 state[8];
    U8  buffer[64];
} SharkSslSha256Ctx;
#endif


#if SHARKSSL_USE_ARC4
typedef struct SharkSslArc4Ctx
{
    U8 x;
    U8 y;
    U8 b[256];
} SharkSslArc4Ctx;
#endif


#if (SHARKSSL_USE_DES || SHARKSSL_USE_3DES)
typedef struct SharkSslDesCtx
{
   #if SHARKSSL_USE_CAU
   #if SHARKSSL_USE_3DES
   U32 key[6];
   #else
   U32 key[2];
   #endif

   #else

   #if SHARKSSL_USE_3DES
   U32 key[96];
   #else
   U32 key[32];
   #endif
   #endif

   #if (SHARKSSL_USE_DES && SHARKSSL_USE_3DES)
   U8  tdea;
   #endif
} SharkSslDesCtx;


typedef enum
{
   SharkSslDesCtx_Decrypt,
   SharkSslDesCtx_Encrypt
} SharkSslDesCtx_Type;
#endif


#if (SHARKSSL_USE_AES_128 || SHARKSSL_USE_AES_256)
typedef struct SharkSslAesCtx
{
    #if SHARKSSL_USE_AES_256
    U32 key[60];     
    #else
    U32 key[44];     
    #endif
    U16 nr;          
} SharkSslAesCtx;

typedef enum 
{
   SharkSslAesCtx_Decrypt,
   SharkSslAesCtx_Encrypt
} SharkSslAesCtx_Type;

#if SHARKSSL_ENABLE_AES_GCM
typedef struct SharkSslAesGcmCtx
{
   SharkSslAesCtx super;
   U8 M0[16][16];
} SharkSslAesGcmCtx;
#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

/* SharkSslCrypto.c */
SHARKSSL_API int   sharkssl_entropy(U32);
SHARKSSL_API int   sharkssl_rng(U8*, U16);

SHARKSSL_API void  SharkSslMd5Ctx_constructor(SharkSslMd5Ctx*);
SHARKSSL_API void  SharkSslMd5Ctx_append(SharkSslMd5Ctx*, const U8*, U32);
SHARKSSL_API void  SharkSslMd5Ctx_finish(SharkSslMd5Ctx*, U8 digest[16]);
SHARKSSL_API int   sharkssl_md5(const U8*, U16, U8*);

SHARKSSL_API void  SharkSslSha1Ctx_constructor(SharkSslSha1Ctx*);
SHARKSSL_API void  SharkSslSha1Ctx_append(SharkSslSha1Ctx*, const U8*, U32);
SHARKSSL_API void  SharkSslSha1Ctx_finish(SharkSslSha1Ctx*, U8 digest[20]);
SHARKSSL_API int   sharkssl_sha1(const U8*, U16, U8*);

#if SHARKSSL_USE_SHA_256
SHARKSSL_API void  SharkSslSha256Ctx_constructor(SharkSslSha256Ctx*);
SHARKSSL_API void  SharkSslSha256Ctx_append(SharkSslSha256Ctx*, const U8*, U32);
SHARKSSL_API void  SharkSslSha256Ctx_finish(SharkSslSha256Ctx*, U8 digest[32]);
SHARKSSL_API int   sharkssl_sha256(const U8*, U16, U8*);
#endif

#if SHARKSSL_USE_ARC4
SHARKSSL_API void  SharkSslArc4Ctx_constructor(SharkSslArc4Ctx *ctx, 
                                               const U8 *key, U8 keyLen);
#define SharkSslArc4Ctx_destructor(ctx) memset(ctx, 0, sizeof(SharkSslArc4Ctx))
SHARKSSL_API void  SharkSslArc4Ctx_crypt(SharkSslArc4Ctx *ctx, U8 *input, U8 *output, U16 len);          
#endif

#if (SHARKSSL_USE_DES || SHARKSSL_USE_3DES)
SHARKSSL_API void  SharkSslDesCtx_constructor(SharkSslDesCtx *ctx, 
                                              SharkSslDesCtx_Type type,
                                              const U8 *key, U8 keyLen);
#define SharkSslDesCtx_destructor(ctx)  memset(ctx, 0, sizeof(SharkSslDesCtx))
SHARKSSL_API void  SharkSslDesCtx_encrypt(SharkSslDesCtx *ctx, U8 input[8], U8 output[8]);
#if SHARKSSL_USE_CAU
SHARKSSL_API void  SharkSslDesCtx_decrypt(SharkSslDesCtx *ctx, U8 input[8], U8 output[8]);
#else
#define SharkSslDesCtx_decrypt(ctx,i,o) SharkSslDesCtx_encrypt((ctx),(i),(o))
#endif
SHARKSSL_API void  SharkSslDesCtx_cbc_encrypt(SharkSslDesCtx *ctx, U8 vect[8], 
                                              U8 *input, U8 *output, U16 len);
SHARKSSL_API void  SharkSslDesCtx_cbc_decrypt(SharkSslDesCtx *ctx, U8 vect[8], 
                                              U8 *input, U8 *output, U16 len);
#endif

#if (SHARKSSL_USE_AES_128 || SHARKSSL_USE_AES_256)
SHARKSSL_API void  SharkSslAesCtx_constructor(SharkSslAesCtx *ctx, 
                                              SharkSslAesCtx_Type type,
                                              const U8 *key, U8 keyLen);
#define SharkSslAesCtx_destructor(ctx) memset(ctx, 0, sizeof(SharkSslAesCtx))
SHARKSSL_API void  SharkSslAesCtx_decrypt(SharkSslAesCtx *ctx, U8 input[16], U8 output[16]);
SHARKSSL_API void  SharkSslAesCtx_encrypt(SharkSslAesCtx *ctx, U8 input[16], U8 output[16]);
SHARKSSL_API void  SharkSslAesCtx_cbc_encrypt(SharkSslAesCtx *ctx, U8 vect[16], 
                                              U8 *input, U8 *output, U16 len);
SHARKSSL_API void  SharkSslAesCtx_cbc_decrypt(SharkSslAesCtx *ctx, U8 vect[16], 
                                              U8 *input, U8 *output, U16 len);
#if SHARKSSL_ENABLE_AES_CTR_MODE
SHARKSSL_API void  SharkSslAesCtx_ctr_mode(SharkSslAesCtx *ctx, U8 ctr[16], 
                                           U8 *input, U8 *output, U16 len);
#endif                          
#if SHARKSSL_ENABLE_AES_GCM
SHARKSSL_API void  SharkSslAesGcmCtx_constructor(SharkSslAesGcmCtx *ctx,
                                                 const U8 *key, U8 keyLen);
#define SharkSslAesGcmCtx_destructor(ctx) memset(ctx, 0, sizeof(SharkSslAesGcmCtx))
SHARKSSL_API int   SharkSslAesGcmCtx_encrypt(SharkSslAesGcmCtx *ctx, 
                                             const U8 vect[12], U8 tag[16],
                                             const U8 *auth, U16 authlen,
                                             U8 *input, U8 *output, U16 len);
SHARKSSL_API int   SharkSslAesGcmCtx_decrypt(SharkSslAesGcmCtx *ctx, 
                                             const U8 vect[12], U8 tagres[16],
                                             const U8 *auth, U16 authlen,
                                             U8 *input, U8 *output, U16 len);
#endif
#endif

#ifdef __cplusplus
}
#endif


#endif /* _SharkSslCrypto_h */
