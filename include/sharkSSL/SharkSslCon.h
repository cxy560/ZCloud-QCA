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
 *   $Id: //depot/sw/branches/olca_IOT_3_1_fc/target/import/rev7/include/sharkSSL/SharkSslCon.h#2 $
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

/*
* Copyright (c) 2013 Qualcomm Atheros, Inc.
* All Rights Reserved.
* Qualcomm Atheros Confidential and Proprietary.
* $ATH_LICENSE_TARGET_C$
*/

#ifndef _SharkSslCon_h
#define _SharkSslCon_h

#define SHARKSSL_LIB 1
#include "SharkSSL.h"
#include "SharkSslCrypto.h"


#if (SHARKSSL_SSL_SERVER_CODE && SHARKSSL_SSL_CLIENT_CODE)
/* dual-role stack */
#define            SharkSsl_isServer(o) (o->role == SharkSsl_Server)
#define            SharkSsl_isClient(o) (o->role == SharkSsl_Client)

#elif SHARKSSL_SSL_SERVER_CODE
/* server-only stack */
#define            SharkSsl_isServer(o) (1)
#define            SharkSsl_isClient(o) (0)

#elif SHARKSSL_SSL_CLIENT_CODE
/* client-only stack */
#define            SharkSsl_isServer(o) (0)
#define            SharkSsl_isClient(o) (1)

#elif ((!SHARKSSL_ENABLE_RSA_API) && (!SHARKSSL_ENABLE_PEM_API))
/* none selected, no RSA/PEM API either */
#error NEITHER SERVER NOR CLIENT CODE SELECTED
#endif

#define SHARKSSL_CONTENT_TYPE_CHANGE_CIPHER_SPEC   20
#define SHARKSSL_CONTENT_TYPE_ALERT                21
#define SHARKSSL_CONTENT_TYPE_HANDSHAKE            22
#define SHARKSSL_CONTENT_TYPE_APPLICATION_DATA     23

#define SHARKSSL_HANDSHAKETYPE_HELLO_REQUEST       0
#define SHARKSSL_HANDSHAKETYPE_CLIENT_HELLO        1
#define SHARKSSL_HANDSHAKETYPE_SERVER_HELLO        2
#define SHARKSSL_HANDSHAKETYPE_CERTIFICATE         11
#define SHARKSSL_HANDSHAKETYPE_SERVER_KEY_EXCHANGE 12
#define SHARKSSL_HANDSHAKETYPE_CERTIFICATE_REQUEST 13
#define SHARKSSL_HANDSHAKETYPE_SERVER_HELLO_DONE   14
#define SHARKSSL_HANDSHAKETYPE_CERTIFICATE_VERIFY  15
#define SHARKSSL_HANDSHAKETYPE_CLIENT_KEY_EXCHANGE 16
#define SHARKSSL_HANDSHAKETYPE_FINISHED            20
#define SHARKSSL_HANDSHAKETYPE_COMPLETE            0xFF

#define SHARKSSL_HASHALGORITHM_NONE                0x00
#define SHARKSSL_HASHALGORITHM_MD5                 0x01
#define SHARKSSL_HASHALGORITHM_SHA1                0x02
#define SHARKSSL_HASHALGORITHM_SHA224              0x03
#define SHARKSSL_HASHALGORITHM_SHA256              0x04
#define SHARKSSL_HASHALGORITHM_SHA384              0x05
#define SHARKSSL_HASHALGORITHM_SHA512              0x06
#define SHARKSSL_HASHALGORITHM_TLS_MD5_SHA1        0xEE  /* internal usage */

#define SHARKSSL_SIGNATUREALGORITHM_RSA            0x01
#define SHARKSSL_SIGNATUREALGORITHM_DSA            0x02
#define SHARKSSL_SIGNATUREALGORITHM_ECDSA          0x03

#define SHARKSSL_TLS_EMPTY_RENEGOTIATION_INFO_SCSV 0x00FF

#if SHARKSSL_USE_NULL_CIPHER
#define SHARKSSL_SSL_RSA_WITH_NULL_SHA             TLS_RSA_WITH_NULL_SHA
#endif
#if SHARKSSL_USE_ARC4
#define SHARKSSL_SSL_RSA_WITH_ARC4_128_SHA         TLS_RSA_WITH_RC4_128_SHA
#if SHARKSSL_ENABLE_PSK
#define SHARKSSL_PSK_WITH_ARC4_128_SHA             TLS_PSK_WITH_RC4_128_SHA
#endif
#endif

#if SHARKSSL_USE_DES
#define SHARKSSL_SSL_RSA_WITH_DES_CBC_SHA          TLS_RSA_WITH_DES_CBC_SHA
#if SHARKSSL_ENABLE_DHE_RSA
#define SHARKSSL_DHE_RSA_WITH_DES_CBC_SHA          TLS_DHE_RSA_WITH_DES_CBC_SHA
#endif 

#elif defined(KINGFISHER_THREADX_PLATFORM)
#define SHARKSSL_SSL_RSA_WITH_DES_CBC_SHA  0
#define SHARKSSL_DHE_RSA_WITH_DES_CBC_SHA   0

#endif /*SHARKSSL_USE_DES */

#if SHARKSSL_USE_3DES
#define SHARKSSL_SSL_RSA_WITH_3DES_EDE_CBC_SHA     TLS_RSA_WITH_3DES_EDE_CBC_SHA
#if SHARKSSL_ENABLE_DHE_RSA
#define SHARKSSL_DHE_RSA_WITH_3DES_EDE_CBC_SHA     TLS_DHE_RSA_WITH_3DES_EDE_CBC_SHA
#endif
#if SHARKSSL_ENABLE_PSK
#define SHARKSSL_PSK_WITH_3DES_EDE_CBC_SHA         TLS_PSK_WITH_3DES_EDE_CBC_SHA
#endif
#endif
#if SHARKSSL_USE_AES_128
#define SHARKSSL_SSL_RSA_WITH_AES_128_CBC_SHA      TLS_RSA_WITH_AES_128_CBC_SHA
#if SHARKSSL_ENABLE_DHE_RSA
#define SHARKSSL_DHE_RSA_WITH_AES_128_CBC_SHA      TLS_DHE_RSA_WITH_AES_128_CBC_SHA
#endif
#if SHARKSSL_ENABLE_PSK
#define SHARKSSL_PSK_WITH_AES_128_CBC_SHA          TLS_PSK_WITH_AES_128_CBC_SHA
#endif
#if SHARKSSL_ENABLE_TLS_1_2
#define SHARKSSL_SSL_RSA_WITH_AES_128_CBC_SHA256   TLS_RSA_WITH_AES_128_CBC_SHA256
#if SHARKSSL_ENABLE_AES_GCM
#define SHARKSSL_SSL_RSA_WITH_AES_128_GCM_SHA256   TLS_RSA_WITH_AES_128_GCM_SHA256
#endif
#if SHARKSSL_ENABLE_DHE_RSA
#define SHARKSSL_DHE_RSA_WITH_AES_128_CBC_SHA256   TLS_DHE_RSA_WITH_AES_128_CBC_SHA256
#if SHARKSSL_ENABLE_AES_GCM
#define SHARKSSL_DHE_RSA_WITH_AES_128_GCM_SHA256   TLS_DHE_RSA_WITH_AES_128_GCM_SHA256
#endif
#endif
#endif
#endif
#if SHARKSSL_USE_AES_256
#define SHARKSSL_SSL_RSA_WITH_AES_256_CBC_SHA      TLS_RSA_WITH_AES_256_CBC_SHA
#if SHARKSSL_ENABLE_DHE_RSA
#define SHARKSSL_DHE_RSA_WITH_AES_256_CBC_SHA      TLS_DHE_RSA_WITH_AES_256_CBC_SHA
#endif
#if SHARKSSL_ENABLE_PSK
#define SHARKSSL_PSK_WITH_AES_256_CBC_SHA          TLS_PSK_WITH_AES_256_CBC_SHA
#endif
#if SHARKSSL_ENABLE_TLS_1_2
#define SHARKSSL_SSL_RSA_WITH_AES_256_CBC_SHA256   TLS_RSA_WITH_AES_256_CBC_SHA256
#if SHARKSSL_ENABLE_DHE_RSA
#define SHARKSSL_DHE_RSA_WITH_AES_256_CBC_SHA256   TLS_DHE_RSA_WITH_AES_256_CBC_SHA256
#endif
#endif
#endif

#define SHARKSSL_MD5_MAC_LEN                       16
#define SHARKSSL_SHA1_MAC_LEN                      20
#define SHARKSSL_SHA256_MAC_LEN                    32

#define SHARKSSL_FINISHED_MSG_LEN_SSL              36   /* SSL3.0 */
#define SHARKSSL_FINISHED_MSG_LEN_TLS              12   /* TLS1.x */

#define SHARKSSL_RECORD_HEADER_SIZE                5    /* SSL3.0 */
#define SHARKSSL_MAX_SESSION_ID_LEN                32   /* SSL3.0 */
#if SHARKSSL_ENABLE_TLS_1_2
#define SHARKSSL_MAX_DIGEST_LEN                    SHARKSSL_SHA256_MAC_LEN
#else
#define SHARKSSL_MAX_DIGEST_LEN                    SHARKSSL_SHA1_MAC_LEN
#endif
#define SHARKSSL_MAX_DIGEST_PAD_LEN                48   /* MD5 PAD */
#define SHARKSSL_MAX_REC_LEN                       (16384 + 2048) /* SSL 3.0 */
#define SHARKSSL_MAX_REC_PAD_LEN                   SHARKSSL_MAX_BLOCK_LEN

#if   SHARKSSL_USE_AES_256
#define SHARKSSL_MAX_KEY_LEN                       32   /* AES-256 */
#elif SHARKSSL_USE_3DES
#define SHARKSSL_MAX_KEY_LEN                       24   /* 3DES */
#elif (SHARKSSL_USE_AES_128 || SHARKSSL_USE_ARC4)
#define SHARKSSL_MAX_KEY_LEN                       16   /* AES-128, ARC4 */
#elif SHARKSSL_USE_DES
#define SHARKSSL_MAX_KEY_LEN                        8   /* DES */
#elif SHARKSSL_USE_NULL_CIPHER
#define SHARKSSL_MAX_KEY_LEN                        0   /* NULL cipher */
#else
#error At least one cipher must be selected in SharkSSL_cfg.h 
#endif

#if   (SHARKSSL_USE_AES_128 || SHARKSSL_USE_AES_256)
#define SHARKSSL_MAX_BLOCK_LEN                     16   /* AES */
#elif (SHARKSSL_USE_DES || SHARKSSL_USE_3DES)
#define SHARKSSL_MAX_BLOCK_LEN                     8    /* DES */
#else
#define SHARKSSL_MAX_BLOCK_LEN                     0    /* stream cipher only */
#endif

/**
 * SharkSSL internals
 * SHARKSSL_BUF_HEADROOM_SIZE is always 64 + 8 (TLS)
 *
 * Alignment to 32/64-bit boundary required for RISC machines
 */

#ifndef UPTR
#define UPTR                                      U32
#endif
#ifndef SHARKSSL_ALIGNMENT
#define SHARKSSL_ALIGNMENT                         4   /* sizeof(UPTR) */
#endif 

#ifdef KINGFISHER_THREADX_PLATFORM
#define SHARKSSL_ALIGN_SIZE(s)                     (((s) + (SHARKSSL_ALIGNMENT - 1)) & (~(SHARKSSL_ALIGNMENT - 1)))
#define SHARKSSL_ALIGNED_POINTER(p)                (U8*)(((UPTR)((UPTR)(p) + SHARKSSL_ALIGNMENT - 1)) & (~(SHARKSSL_ALIGNMENT - 1)))

#else
#define SHARKSSL_ALIGN_SIZE(s)                     (((s) + (SHARKSSL_ALIGNMENT - 1)) & ((U32)-SHARKSSL_ALIGNMENT))
#define SHARKSSL_ALIGNED_POINTER(p)                (U8*)(((UPTR)((UPTR)(p) + SHARKSSL_ALIGNMENT - 1)) & ((UPTR)-SHARKSSL_ALIGNMENT))
#endif /* NOT KINGFISHER_THREADX_PLATFORM */

#define SHARKSSL_POINTER_IS_ALIGNED(p)             (0 == ((unsigned int)(UPTR)(p) & (SHARKSSL_ALIGNMENT - 1)))

#if SHARKSSL_UNALIGNED_MALLOC
#define SHARKSSL_MALLOC_ALIGN(s)                   ((s) + SHARKSSL_ALIGNMENT)
#define SHARKSSL_PNTR_ALIGNMENT(p)                 SHARKSSL_ALIGNED_POINTER(p)
#else
#define SHARKSSL_MALLOC_ALIGN(s)                   (s)
#define SHARKSSL_PNTR_ALIGNMENT(p)                 (U8*)(p)
#endif


#define SHARKSSL_DIM_ARR(a)                        (sizeof(a)/sizeof(a[0]))
 
#define SHARKSSL_BUF_EXPANDSIZE                    1024                                                                  
#define SHARKSSL_BUF_HEADROOM_SIZE                 (64 + 8)

/**
 * keep room for 
 *   Change Cipher Spec message (HEADER_SIZE + 1)
 *   +
 *   Finished message : maximum of
 *      TLS 3.0 : HEADER_SIZE + SHARKSSL_FINISHED_MSG_LEN_SSL + MAX_DIGEST + MAX PAD 
 *      TLS 1.0 : HEADER_SIZE + SHARKSSL_FINISHED_MSG_LEN_TLS + MAX_DIGEST + MAX PAD 
 *      TLS 1.1 : HEADER_SIZE + MAX_BLOCK + SHARKSSL_FINISHED_MSG_LEN_TLS + MAX_DIGEST + MAX PAD 
 *
 *   Change Cipher here for "False Start" (Google Chrome 8)
 */
#if (SHARKSSL_MAX_BLOCK_LEN <= (SHARKSSL_FINISHED_MSG_LEN_SSL - SHARKSSL_FINISHED_MSG_LEN_TLS))
#define SHARKSSL_HS_PARAM_OFFSET                   SHARKSSL_ALIGN_SIZE(SHARKSSL_RECORD_HEADER_SIZE + 1 + \
                                                                       SHARKSSL_RECORD_HEADER_SIZE + \
                                                                       SHARKSSL_FINISHED_MSG_LEN_SSL + \
                                                                       SHARKSSL_MAX_DIGEST_LEN + \
                                                                       SHARKSSL_MAX_REC_PAD_LEN)
#else
#define SHARKSSL_HS_PARAM_OFFSET                   SHARKSSL_ALIGN_SIZE(SHARKSSL_RECORD_HEADER_SIZE + 1 + \
                                                                       SHARKSSL_RECORD_HEADER_SIZE + \
                                                                       SHARKSSL_MAX_BLOCK_LEN + \
                                                                       SHARKSSL_FINISHED_MSG_LEN_TLS + \
                                                                       SHARKSSL_MAX_DIGEST_LEN + \
                                                                       SHARKSSL_MAX_REC_PAD_LEN)
#endif

#define SHARKSSL_FLAG_HAS_MORE_DATA                0x0001
#define SHARKSSL_FLAG_SPLIT_ENCRYPT                0x0002
#define SHARKSSL_FLAG_READ_ENCRYPTED               0x0004
#define SHARKSSL_FLAG_WRITE_ENCRYPTED              0x0008
#define SHARKSSL_FLAG_DECRYPT_INITIALIZE           0x0010
#define SHARKSSL_FLAG_INVALID_CON                  0x0020
#define SHARKSSL_FLAG_ALERTED_CON                  0x0040
#define SHARKSSL_FLAG_ZERO_LEN_APPLI               0x0080
#define SHARKSSL_FLAG_CREATE_CLIENT_HELLO          0x0100
#define SHARKSSL_FLAG_RESUMED_SESSION              0x0200
#define SHARKSSL_FLAG_CERTIFICATE_REQUEST          0x0400
#define SHARKSSL_FLAG_HAS_CERTIFICATE              0x0800
#define SHARKSSL_FLAG_EMPTY_CERTIFICATE            0x1000
#define SHARKSSL_FLAG_TRUSTED_CA                   0x2000
#define SHARKSSL_FLAG_SECURE_RENEGOTIATION         0x4000
#define SHARKSSL_FLAG_HANDSHAKE_DATA_IN_OUTBUF     0x8000
#define SHARKSSL_FLAG_CACHED_SESSION           0x00010000

#define SHARKSSL_OP_INITIALIZE                     0x01
#define SHARKSSL_OP_DESTRUCTOR                     (SHARKSSL_OP_INITIALIZE | 0x02)
#define SHARKSSL_OP_ENCRYPT                        0x10
#define SHARKSSL_OP_DECRYPT                        0x20
#define SHARKSSL_OP_PUBLIC                         0x40  /* RSA */
#define SHARKSSL_OP_PRIVATE                        0x80  /* RSA */
#define SHARKSSL_OP_CALC_Y                         0x04  /* DH  */
#define SHARKSSL_OP_CALC_KEY                       0x08  /* DH  */

/**
 * flags to be used in SharkSslCipherSuite.flags (SharkSslCon.c)
 */
#define SHARKSSL_CS_DHE                            0x01
#define SHARKSSL_CS_GCM                            0x02
#define SHARKSSL_CS_PSK                            0x04
#define SHARKSSL_CS_TLS12                          0x08


/**
 * macros to split expLen values into real expLen and chainLen
 * expLen is a 16-bit value
 *  - the 4 MSbits are chainLen (0 = single cert, 1 = child + parent, 2 = c + p + gp)
 *  - the 12 LSbits are the real exponent length (in words)
 */
#define SHARKSSL_EXPLEN_GET_EXPLEN(e)              (U16)((e) & 0x0FFF)
#define SHARKSSL_EXPLEN_GET_CHAINLEN(e)            (U16)(((U16)(e)) >> 12)


/**
 * Certificate Authority List
 */
#if (SHARKSSL_ENABLE_CA_LIST  || SHARKSSL_ENABLE_CERTSTORE_API)
#define SHARKSSL_CA_LIST_NAME_SIZE                 8  
#define SHARKSSL_CA_LIST_ELEMENT_SIZE              (SHARKSSL_CA_LIST_NAME_SIZE + 4)
#define SHARKSSL_CA_LIST_INDEX_TYPE                0x00  

#ifdef __IAR_SYSTEMS_ICC__
/* compiler issue? */
#else
#if (SHARKSSL_CA_LIST_NAME_SIZE != SHARKSSL_ALIGN_SIZE(SHARKSSL_CA_LIST_NAME_SIZE))
#error SHARKSSL CA_STORE_API: UNSUPPORTED CA_LIST_NAME_SIZE
#endif
#endif

#if SHARKSSL_ENABLE_CERTSTORE_API
#define SHARKSSL_CA_LIST_PTR_SIZE                  sizeof(U8*)
#define SHARKSSL_CA_LIST_PTR_TYPE                  0xAD
#define SHARKSSL_MAX_SNAME_LEN                     32

#if (SHARKSSL_MAX_SNAME_LEN < SHARKSSL_CA_LIST_NAME_SIZE)
#error SHARKS_MAX_SNAME_LEN must be >= SHARKSSL_CA_LIST_NAME_SIZE
#endif

typedef struct SharkSslCSCert
{
      DoubleLink super;
      U8 *ptr;  /* points to the byte sequence ASN.1 format of the cert */
      char name[SHARKSSL_MAX_SNAME_LEN + 1];  /* subject name of the CA */
} SharkSslCSCert;

#endif  /* SHARKSSL_ENABLE_CERTSTORE_API */
#endif  /* (SHARKSSL_ENABLE_CA_LIST  || SHARKSSL_ENABLE_CERTSTORE_API) */
 

typedef struct SharkSslBuf
{
   #if SHARKSSL_UNALIGNED_MALLOC
   U8  *mem;     /* where the allocated memory begins in this case */
   #endif
   U8  *buf;     /* where the allocated memory begins */
   U8  *data;    /* where the data begins */
   U16  size;    /* number of bytes in the buffer available to the user */
   U16  dataLen; /* length of the data to be processed */
   U16  temp;
} SharkSslBuf;

void    SharkSslBuf_constructor(SharkSslBuf*, U16);
void    SharkSslBuf_destructor(SharkSslBuf*);
U8     *SharkSslBuf_expand(SharkSslBuf*, U16);
void    SharkSslBuf_leftAlignData(SharkSslBuf*);
#define SharkSslBuf_notAllocated(o) (!((o)->buf))
#define SharkSslBuf_dataPtrLeftAligned(o) \
   ((o)->buf + SHARKSSL_BUF_HEADROOM_SIZE)
#define SharkSslBuf_isLeftAligned(o) \
   ((o)->data == SharkSslBuf_dataPtrLeftAligned(o))
#define SharkSslBuf_leftAlignDataPtr(o) do {\
   (o)->data = SharkSslBuf_dataPtrLeftAligned(o); \
   } while (0);


typedef int (*SharkSslCon_cipherFunc)(SharkSslCon*, U8, U8*, U16);
typedef int (*SharkSslCon_digestFunc)(SharkSslCon*, const U8*, U16, U8*);


typedef struct SharkSslCipherSuite
{
   SharkSslCon_cipherFunc cipherFunc;
   #if (SHARKSSL_USE_SHA1_MAC_ONLY == 0)
   SharkSslCon_digestFunc digestFunc;
   #endif
   U16 id;
   U8  keyLen;
   U8  blockLen;
   #if (SHARKSSL_USE_SHA1_MAC_ONLY == 0)
   U8  digestLen;
   #endif
   U8  flags;
} SharkSslCipherSuite;


#if (!SHARKSSL_DISABLE_RSA)   
typedef struct SharkSslCertKey
{
   U8 *mod, *exp;
   U16 modLen, expLen;
} SharkSslCertKey;


typedef struct SharkSslCertParam
{
   SharkSslCertInfo certInfo;
   #if (SHARKSSL_MAX_DIGEST_LEN > (SHARKSSL_MD5_MAC_LEN + SHARKSSL_SHA1_MAC_LEN))
   U8 hash[SHARKSSL_MAX_DIGEST_LEN];
   #else
   U8 hash[SHARKSSL_MD5_MAC_LEN + SHARKSSL_SHA1_MAC_LEN];
   #endif
   U8 *sgn, sgnAlgoID;
   U16 sgnLen;
   SharkSslCertKey certKey;
} SharkSslCertParam;
#endif  /* (!SHARKSSL_DISABLE_RSA) */


#if SHARKSSL_ENABLE_DHE_RSA
typedef struct SharkSslDHParam
{
   U8 *p;     /* prime modulus     */
   U8 *g;     /* generator         */
   U8 *Y;     /* Ys/Yc             */
   U8 *r;     /* random secret     */
   U16 pLen;  /* len of p in bytes */
   U16 gLen;  /* len of g in bytes */
} SharkSslDHParam;
#endif


typedef struct SharkSslHSParam
{
   U8  clientRandom[32];
   U8  serverRandom[32];
   U8  masterSecret[48];
   U8  sharedSecret[2 * (SHARKSSL_MAX_DIGEST_LEN +
                         SHARKSSL_MAX_KEY_LEN +
                         SHARKSSL_MAX_BLOCK_LEN) + SHARKSSL_MAX_DIGEST_LEN];
   SharkSslMd5Ctx      md5Ctx;
   SharkSslSha1Ctx     sha1Ctx;
   #if SHARKSSL_ENABLE_TLS_1_2
   SharkSslSha256Ctx   sha256Ctx;
   #endif
   #if (!SHARKSSL_DISABLE_RSA)   
   SharkSslCertKey     certKey;  /* points to cert's key */
   SharkSslCertParam   certParam;
   #endif
   #if SHARKSSL_ENABLE_DHE_RSA
   SharkSslDHParam     dhParam;
   #endif
} SharkSslHSParam;


void    SharkSslHSParam_constructor(SharkSslHSParam*);
void    SharkSslHSParam_destructor(SharkSslHSParam*);
void    SharkSslHSParam_add(SharkSslHSParam*, U8*, U16);
#define hsParam(o) ((SharkSslHSParam*)(o->outBuf.buf + \
                                       SHARKSSL_BUF_HEADROOM_SIZE + \
                                       SHARKSSL_HS_PARAM_OFFSET))


#if SHARKSSL_ENABLE_SESSION_CACHE
struct SharkSslSession
{
   SharkSslCipherSuite *cipherSuite;
   U32 firstAccess, latestAccess;
   U8  id[SHARKSSL_MAX_SESSION_ID_LEN];
   U8  masterSecret[48];
   U16 nUse;
   U8  major_minor, sFlag;
   #if SHARKSSL_ENABLE_CLONE_CERTINFO
   SharkSslCertInfo *certInfo;
   #endif
};

#define SharkSslSession_cmpMajMin(o,maj,min) ((o)->major_minor == (((maj & 0x0F) << 4) | (min & 0x0F)))
#define SharkSslSession_setMajMin(o,maj,min) do {           \
   baAssert((maj <= 0x0F) && (min <= 0x0F));                \
   (o)->major_minor = (((maj & 0x0F) << 4) | (min & 0x0F)); \
   } while (0);


void    SharkSslSessionCache_constructor(SharkSslSessionCache*, U16);
void    SharkSslSessionCache_destructor(SharkSslSessionCache*);
#define SharkSslSessionCache_lock(o)   ThreadMutex_set(&((o)->cacheMutex))
#define SharkSslSessionCache_unlock(o) ThreadMutex_release(&((o)->cacheMutex))
SharkSslSession *SharkSslSessionCache_newSession(SharkSslSessionCache*, SharkSslCon*, U8*, U16);
SharkSslSession *SharkSslSessionCache_findSession(SharkSslSessionCache*, SharkSslCon*, U8*, U16);
#endif


struct SharkSslCon   /* typedef'd in SharkSsl.h */
{
   #if SHARKSSL_MAX_BLOCK_LEN
   U8 rIV[SHARKSSL_MAX_BLOCK_LEN];
   #endif
   #if SHARKSSL_MAX_KEY_LEN
   U8 rKey[SHARKSSL_MAX_KEY_LEN];
   #endif
   U8 rMacH[64 + SHARKSSL_MAX_DIGEST_LEN];

   #if SHARKSSL_MAX_BLOCK_LEN
   U8 wIV[SHARKSSL_MAX_BLOCK_LEN];
   #endif
   #if SHARKSSL_MAX_KEY_LEN
   U8 wKey[SHARKSSL_MAX_KEY_LEN];
   #endif
   U8 wMacH[64 + SHARKSSL_MAX_DIGEST_LEN];

   SharkSsl *sharkSsl;
   SharkSslCipherSuite *cipherSuite;
   #if SHARKSSL_ENABLE_SESSION_CACHE
   SharkSslSession *session;
   #endif

   void *rCtx, *wCtx;
   #if SHARKSSL_UNALIGNED_MALLOC
   SharkSslCon *mem;
   #endif

   SharkSslBuf inBuf, outBuf;

   U32 flags;
   #if SHARKSSL_ENABLE_SSL_3_0
   U16 rSeq, wSeq;
   #endif
   U8 state;
   U8 reqMajor, reqMinor;
   U8 major, minor;
   U8 ccLen;
   U8 padLen;
   U8 alertLevel, alertDescr; 
};


typedef enum 
{
  SharkSslCon_SenderIsClient,
  SharkSslCon_SenderIsServer
  #if SHARKSSL_ENABLE_SSL_3_0
  #if ((SHARKSSL_SSL_CLIENT_CODE && (!SHARKSSL_DISABLE_RSA)) || \
       (SHARKSSL_SSL_SERVER_CODE && SHARKSSL_ENABLE_CLIENT_AUTH))
  ,
  SharkSslCon_NoSender  /* to calc CertificateVerify signature */
  #endif
  #endif
} SharkSslCon_SendersRole;


/* alternate SharkSsl_createCon for pre-allocated sharkSslCon object */
#define SharkSsl_createCon2(o, sharkSslCon) do {\
   (o)->nCon++;\
   SharkSslCon_constructor(sharkSslCon, o);\
} while(0)


/* public member-function prototypes in SharkSsl.h */
void               SharkSslCon_constructor(SharkSslCon *o, SharkSsl *sharkSsl);
void               SharkSslCon_destructor(SharkSslCon *o);

/* private member-function prototypes */
SharkSslCon_RetVal SharkSslCon_buildFatalMessage(SharkSslCon*, U8);
SharkSslCon_RetVal SharkSslCon_buildAlertMessage(SharkSslCon*, U8, U8);
SharkSslCon_RetVal SharkSslCon_processHandshake(SharkSslCon*, U8*, U16*, U8);
SharkSslCon_RetVal SharkSslCon_processChangeCipherSpec(SharkSslCon*, U8*, U16);
U8                *SharkSslCon_writeSslHeader(SharkSslCon*, U8, U8*, U16);
U16                SharkSslCon_calcSharedSecretLen(SharkSslCon*, SharkSslCipherSuite*);
int                SharkSslCon_calcCryptoParam(SharkSslCon*, U8*, U16, U8*, U16, U8[32], U8[32], U8*);
int                SharkSslCon_calcFinishedHash(SharkSslCon*, SharkSslCon_SendersRole, U8*, U8*);
int                SharkSslCon_buildFinished(SharkSslCon *o, SharkSslCon_SendersRole, U8*, U8*);
void               SharkSslCon_setHMacKey(SharkSslCon*, U8*, U8*, U8*);
void               SharkSslCon_setHMacKeyCP(SharkSslCon*, U8*, U16, U8*, U8, SharkSslCon_digestFunc);
int                SharkSslCon_calcMacAndEncrypt(SharkSslCon*, U8);
void               SharkSslCon_invalidateCon(SharkSslCon*);

#if (!SHARKSSL_DISABLE_RSA)   
/* private cert member functions */
int  SharkSslCertParam_parseCert(SharkSslCertParam*, U8*, U32, U8*);
int  SharkSslCertParam_verifySignature(const SharkSslCertParam*);
int  SharkSslCertParam_createSignature(const SharkSslCertParam*);
U8   SharkSslCertDN_equal(const SharkSslCertDN*, const SharkSslCertDN*);
SHARKSSL_API void SharkSslCertKey_setFromSharkSslCert(SharkSslCertKey*, SharkSslCert);
U8   SharkSslCert_outCertMsg(SharkSslCert, U16, U8*);
U16  SharkSslCert_certMsgLen(SharkSslCert);
#endif

int  SharkSslCon_md5(SharkSslCon*, const U8*, U16, U8*);
int  SharkSslCon_sha1(SharkSslCon*, const U8*, U16, U8*);
#if SHARKSSL_USE_SHA_256
int  SharkSslCon_sha256(SharkSslCon*, const U8*, U16, U8*);
#endif
int  SharkSslCon_HMAC(SharkSslCon *, U8, U8*, U16, U8*, U8, SharkSslCon_digestFunc);
#if (!SHARKSSL_DISABLE_RSA)   
SHARKSSL_API int  SharkSslCertKey_RSA(const SharkSslCertKey*, U8, U8*);
#endif
#if SHARKSSL_ENABLE_DHE_RSA
int  SharkSslDHParam_DH(const SharkSslDHParam*, U8, U8*);
#if SHARKSSL_SSL_SERVER_CODE
void SharkSslDHParam_setParam(SharkSslDHParam *dh);
#endif
#endif
#if SHARKSSL_USE_NULL_CIPHER
int  SharkSslCon_null(SharkSslCon*, U8, U8*, U16);
#endif
#if SHARKSSL_USE_ARC4
int  SharkSslCon_arc4(SharkSslCon*, U8, U8*, U16);
#endif
#if (SHARKSSL_USE_DES || SHARKSSL_USE_3DES)
int  SharkSslCon_des_cbc(SharkSslCon*, U8, U8*, U16);
#endif
#if (SHARKSSL_USE_AES_128 || SHARKSSL_USE_AES_256)
int  SharkSslCon_aes_cbc(SharkSslCon*, U8, U8*, U16);
#if (SHARKSSL_ENABLE_AES_GCM && SHARKSSL_ENABLE_TLS_1_2)
int  SharkSslCon_aes_gcm(SharkSslCon*, U8, U8*, U16);
#endif
#endif

#endif
