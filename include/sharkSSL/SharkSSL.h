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
 *   $Id: //depot/sw/branches/olca_IOT_3_1_fc/target/import/rev7/include/sharkSSL/SharkSSL.h#1 $
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
#ifndef _SharkSsl_h
#define _SharkSsl_h

#include "TargConfig.h"      /* platform dependancies  */
#include "SharkSSL_cfg.h"    /* SharkSSL configuration */
#ifdef __cplusplus
#include <stddef.h>          /* size_t for new, delete */
#endif

#ifndef SHARKSSL_API
#define SHARKSSL_API
#else  /* Barracuda */
#define SHARKSSL_BA 1
#include <ThreadLib.h>
#include <HttpServerLib.h>
#endif

#include "SharkSslCrypto.h"  /* Crypto API */


#define SHARKSSL_ALERT_LEVEL_WARNING               1
#define SHARKSSL_ALERT_LEVEL_FATAL                 2

#define SHARKSSL_ALERT_CLOSE_NOTIFY                0
#define SHARKSSL_ALERT_UNEXPECTED_MESSAGE          10
#define SHARKSSL_ALERT_BAD_RECORD_MAC              20
#define SHARKSSL_ALERT_DECRYPTION_FAILED           21
#define SHARKSSL_ALERT_RECORD_OVERFLOW             22
#define SHARKSSL_ALERT_DECOMPRESSION_FAILURE       30
#define SHARKSSL_ALERT_HANDSHAKE_FAILURE           40
#define SHARKSSL_ALERT_NO_CERTIFICATE              41  /* SSL 3.0 only */
#define SHARKSSL_ALERT_BAD_CERTIFICATE             42
#define SHARKSSL_ALERT_UNSUPPORTED_CERTIFICATE     43
#define SHARKSSL_ALERT_CERTIFICATE_REVOKED         44
#define SHARKSSL_ALERT_CERTIFICATE_EXPIRED         45
#define SHARKSSL_ALERT_CERTIFICATE_UNKNOWN         46
#define SHARKSSL_ALERT_ILLEGAL_PARAMETER           47
#define SHARKSSL_ALERT_UNKNOWN_CA                  48
#define SHARKSSL_ALERT_ACCESS_DENIED               49
#define SHARKSSL_ALERT_DECODE_ERROR                50
#define SHARKSSL_ALERT_DECRYPT_ERROR               51
#define SHARKSSL_ALERT_EXPORT_RESTRICTION          60
#define SHARKSSL_ALERT_PROTOCOL_VERSION            70
#define SHARKSSL_ALERT_INSUFFICIENT_SECURITY       71
#define SHARKSSL_ALERT_INTERNAL_ERROR              80
#define SHARKSSL_ALERT_USER_CANCELED               90
#define SHARKSSL_ALERT_NO_RENEGOTIATION            100
#define SHARKSSL_ALERT_UNKNOWN_PSK_IDENTITY        115


/**
 * Supported Ciphersuites
 * values returned by SharkSslCon_getCiphersuite
 */
#define TLS_NULL_WITH_NULL_NULL                    0x0000
#define TLS_RSA_WITH_NULL_SHA                      0x0002
#define TLS_RSA_WITH_RC4_128_SHA                   0x0005
#define TLS_PSK_WITH_RC4_128_SHA                   0x008A
#define TLS_RSA_WITH_DES_CBC_SHA                   0x0009
#define TLS_RSA_WITH_3DES_EDE_CBC_SHA              0x000A
#define TLS_DHE_RSA_WITH_DES_CBC_SHA               0x0015
#define TLS_DHE_RSA_WITH_3DES_EDE_CBC_SHA          0x0016
#define TLS_PSK_WITH_3DES_EDE_CBC_SHA              0x008B
#define TLS_RSA_WITH_AES_128_CBC_SHA               0x002F
#define TLS_DHE_RSA_WITH_AES_128_CBC_SHA           0x0033
#define TLS_PSK_WITH_AES_128_CBC_SHA               0x008C
#define TLS_RSA_WITH_AES_256_CBC_SHA               0x0035
#define TLS_DHE_RSA_WITH_AES_256_CBC_SHA           0x0039
#define TLS_RSA_WITH_AES_128_CBC_SHA256            0x003C
#define TLS_RSA_WITH_AES_256_CBC_SHA256            0x003D
#define TLS_DHE_RSA_WITH_AES_128_CBC_SHA256        0x0067
#define TLS_DHE_RSA_WITH_AES_256_CBC_SHA256        0x006B
#define TLS_PSK_WITH_AES_256_CBC_SHA               0x008D
#define TLS_RSA_WITH_AES_128_GCM_SHA256            0x009C
#define TLS_DHE_RSA_WITH_AES_128_GCM_SHA256        0x009E


/**
 * values returned by SharkSslCon_getProtocol
 */
#define SHARKSSL_PROTOCOL_UNKNOWN                  0x00
#define SHARKSSL_PROTOCOL_SSL_3_0                  0x30
#define SHARKSSL_PROTOCOL_TLS_1_0                  0x31
#define SHARKSSL_PROTOCOL_TLS_1_1                  0x32 
#define SHARKSSL_PROTOCOL_TLS_1_2                  0x33


#if SHARKSSL_ENABLE_SESSION_CACHE
typedef struct SharkSslSession SharkSslSession;


typedef struct SharkSslSessionCache
{
   SharkSslSession *cache;
   ThreadMutexBase  cacheMutex;
   U16 cacheSize;
} SharkSslSessionCache;
#endif


typedef enum
{
   SharkSsl_Server,
   SharkSsl_Client
} SharkSsl_Role;


#if (!SHARKSSL_DISABLE_RSA)
typedef const U8 *SharkSslCert;
typedef const U8 *SharkSslCAList;
#endif

struct SharkSsl;
struct SharkSslIntf;
typedef void (*SharkSslIntf_Terminate)(struct SharkSslIntf* o, struct SharkSsl* ssl); 

typedef struct SharkSslIntf
{
   SharkSslIntf_Terminate terminate;
} SharkSslIntf;

#define SharkSslIntf_constructor(o,terminateFunc) (o)->terminate=terminateFunc

typedef struct SharkSslCon SharkSslCon;


typedef struct SharkSsl
{
#ifdef __cplusplus
   void *operator new(size_t s) { return ::baMalloc(s); }
   void operator delete(void* d) { if(d) ::baFree(d); }
   void *operator new(size_t, void *place) { return place; }
   void operator delete(void*, void *) { }

   /**
    *  Default constructor is used as a dummy op with the C constructors
    */
   SharkSsl();

   /** 
    *  Create a SharkSsl object 
    *  (default: no cert, no cache, empty CA list)
    */
   SharkSsl(SharkSsl_Role role,
            #if (!SHARKSSL_DISABLE_RSA)
            SharkSslCert cert     = NULL,
            #endif
            U16 cacheSize         = 0,
            U16 inBufStartSize    = 4096,
            U16 outBufSize        = 4096
            #if (!SHARKSSL_DISABLE_RSA)
            ,
            SharkSslCAList caList = NULL
            #endif
            );

   /* Destroy a SharkSsl object   */
   ~SharkSsl();
   
   /* Create a connection object  */
   SharkSslCon *createCon(void);
   
   /* Destroy a connection object */
   void terminateCon(SharkSslCon *sslCon);
#endif
   #if (SHARKSSL_SSL_SERVER_CODE && SHARKSSL_SSL_CLIENT_CODE)
   SharkSsl_Role role;
   #endif
   U16 outBufSize;
   U16 inBufStartSize;
   U16 nCon;
   #if (!SHARKSSL_DISABLE_RSA)
   SharkSslCert cert;
   #if SHARKSSL_ENABLE_CA_LIST
   SharkSslCAList caList;
   #endif
   #endif
   #if SHARKSSL_ENABLE_SESSION_CACHE
   SharkSslSessionCache sessionCache;
   /* Reserved for the HTTP client library unless SharkSSL is used standalone */
   SharkSslIntf* intf;
   #endif
} SharkSsl;


typedef enum
{
   SharkSslCon_Error = 1,
   SharkSslCon_AllocationError,
   SharkSslCon_Decrypted,
   SharkSslCon_Handshake,
   SharkSslCon_NeedMoreData,
   SharkSslCon_AlertSend,
   SharkSslCon_AlertRecv,
   SharkSslCon_Encrypted,
   SharkSslCon_HandshakeNotComplete,
   SharkSslCon_Certificate
} SharkSslCon_RetVal;



#if (!SHARKSSL_DISABLE_RSA)
typedef struct SharkSslCertDN
{
   U8 *countryName;
   U8 *province;
   U8 *locality;
   U8 *organization;
   U8 *unit;
   U8 *commonName;
} SharkSslCertDN;


typedef struct SharkSslCertInfo
{
   U16 snLen;
   U8  version, CAflag;
   U8 *sn, *timeFrom, *timeTo;
   SharkSslCertDN issuer, subject;
   U8 *subjectAltDNSNames;
   struct SharkSslCertInfo *parent; /* pointer to parent on a cert. chain */
} SharkSslCertInfo;
#endif


#ifdef __cplusplus
extern "C" {
#endif
SHARKSSL_API void  SharkSsl_constructor(
                      SharkSsl *o,
                      SharkSsl_Role role,
                      #if (!SHARKSSL_DISABLE_RSA)
                      SharkSslCert cert,
                      #endif
                      U16 cacheSize,
                      U16 inBufStartSize,
                      U16 outBufSize
                      #if (!SHARKSSL_DISABLE_RSA)
                      ,
                      SharkSslCAList caList
                      #endif
                      );
SHARKSSL_API void  SharkSsl_destructor(SharkSsl *o);
SharkSslCon       *SharkSsl_createCon(SharkSsl *o);
void               SharkSsl_terminateCon(const SharkSsl *, SharkSslCon*);
#if SHARKSSL_ENABLE_SESSION_CACHE
SHARKSSL_API U16   SharkSsl_getCacheSize(SharkSsl *o);
#define SharkSsl_setIntf(o, sharkSslIntf) (o)->intf=sharkSslIntf
#define SharkSsl_getIntf(o) (o)->intf
#endif

#define            SharkSslCon_terminate(o) SharkSsl_terminateCon(0, o)
SharkSslCon_RetVal SharkSslCon_decrypt(SharkSslCon *o, U16 readLen);
SharkSslCon_RetVal SharkSslCon_encrypt(SharkSslCon *o, U8 *buf, U16 maxLen);
U8                 SharkSslCon_isHandshakeComplete(SharkSslCon *o);
U8                 SharkSslCon_encryptMore(SharkSslCon *o);
U8                 SharkSslCon_decryptMore(SharkSslCon *o);
U8                *SharkSslCon_getBuf(SharkSslCon *o);
U16                SharkSslCon_getBufLen(SharkSslCon *o);
U16                SharkSslCon_copyDecData(SharkSslCon *o, U8 *buf, U16 maxLen);
U16                SharkSslCon_getDecData(SharkSslCon *o, U8 **bufPtr);
U8                *SharkSslCon_getHandshakeData(SharkSslCon *o);
U16                SharkSslCon_getHandshakeDataLen(SharkSslCon *o);
U8                *SharkSslCon_getEncData(SharkSslCon *o);
U16                SharkSslCon_getEncDataLen(SharkSslCon *o);
U8                *SharkSslCon_getEncBufPtr(SharkSslCon *o);
U16                SharkSslCon_getEncBufSize(SharkSslCon *o);
U8                 SharkSslCon_getAlertLevel(SharkSslCon *o);
U8                 SharkSslCon_getAlertDescription(SharkSslCon *o);
#define SharkSslCon_getAlertData(o)    SharkSslCon_getEncData(o)
#define SharkSslCon_getAlertDataLen(o) SharkSslCon_getEncDataLen(o)

#if SHARKSSL_ENABLE_INFO_API
U16                SharkSslCon_getCiphersuite(SharkSslCon *o);
U8                 SharkSslCon_getProtocol(SharkSslCon *o);
#endif
#if ((SHARKSSL_SSL_CLIENT_CODE && (!SHARKSSL_DISABLE_RSA)) || \
     (SHARKSSL_SSL_SERVER_CODE && SHARKSSL_ENABLE_CLIENT_AUTH))
SharkSslCertInfo  *SharkSslCon_getCertInfo(SharkSslCon *o);
U8                 SharkSslCon_cloneCertInfo(SharkSslCon *o, SharkSslCertInfo **outCertInfoPtr);
U8                 SharkSslCon_certificateReceived(SharkSslCon *o);
#if (SHARKSSL_ENABLE_CA_LIST)
U8                 SharkSslCon_trustedCA(SharkSslCon *o);
U8                 SharkSslCon_isCAListEmpty(SharkSslCon *o);
#endif
#endif

#if SHARKSSL_SSL_CLIENT_CODE 
U8                 SharkSslCon_selectProtocol(SharkSslCon *o, U8 protocol);
#endif

#if SHARKSSL_ENABLE_SESSION_CACHE
SHARKSSL_API U8    SharkSslSession_release(SharkSslSession *o, SharkSsl *s);
#if SHARKSSL_SSL_CLIENT_CODE 
SHARKSSL_API SharkSslSession *SharkSslCon_acquireSession(SharkSslCon *o);
SHARKSSL_API U8    SharkSslCon_setSession(SharkSslCon *o, SharkSslSession *s); 
SHARKSSL_API U32   SharkSslSession_getLatestAccessTime(SharkSslSession *o);
#define SharkSslCon_resumeSession(o,s) SharkSslCon_setSession(o, s);
#endif
#endif

#if (SHARKSSL_SSL_SERVER_CODE && SHARKSSL_ENABLE_CLIENT_AUTH)
U8 SharkSslCon_requestClientCert(SharkSslCon *o);
#endif

#if SHARKSSL_ENABLE_PEM_API
typedef enum
{
  SHARKSSL_PEM_OK = 0,
  SHARKSSL_PEM_OK_PUBLIC = 1,
  SHARKSSL_PEM_ALLOCATION_ERROR = - 3000,
  SHARKSSL_PEM_KEY_PARSE_ERROR,
  SHARKSSL_PEM_KEY_WRONG_IV,
  SHARKSSL_PEM_KEY_WRONG_LENGTH,
  SHARKSSL_PEM_KEY_PASSPHRASE_REQUIRED,
  SHARKSSL_PEM_KEY_UNRECOGNIZED_FORMAT,
  SHARKSSL_PEM_KEY_UNSUPPORTED_FORMAT,
  SHARKSSL_PEM_KEY_UNSUPPORTED_MODULUS_LENGTH,
  SHARKSSL_PEM_KEY_UNSUPPORTED_ENCRYPTION_TYPE,
  SHARKSSL_PEM_KEY_CERT_MISMATCH,
  SHARKSSL_PEM_CERT_UNRECOGNIZED_FORMAT,
  SHARKSSL_PEM_CERT_UNSUPPORTED_TYPE
} sharkssl_PEM_RetVal;

SHARKSSL_API sharkssl_PEM_RetVal sharkssl_PEM(const char *certPEM, const char *rsaKeyPEM, 
                                              const char *passphrase, SharkSslCert *sharkSslCert);
#endif

#if SHARKSSL_ENABLE_RSA_API
typedef SharkSslCert SharkSslRSAKey;
#if SHARKSSL_ENABLE_PEM_API
/**
 * Conversion from PEM public/private key to RSA public/private key 
 * in internal proprietary format used in the sharkssl_RSA_ functions
 * declared below
 * 'passphrase' is the password of the private key
 * Returns NULL in case of error 
 *
 * Remember to call SharkSslRSAKey_free when the result is not needed anymore!
 * example:
 * {
 *    SharkSslRSAKey RSAKey;   
 *    ...
 *    RSAKey = sharksslPEM_to_RSAKey(key, pass);
 *    if (RSAKey)
 *    {
 *       ...
 *       void SharkSslRSAKey_free(RSAKey);
 *    }
 * }
 */
SharkSslRSAKey sharkssl_PEM_to_RSAKey(const char *PEMKey, const char *passphrase);
void SharkSslRSAKey_free(SharkSslRSAKey RSAKey);
#endif
U16 SharkSslRSAKey_size(SharkSslRSAKey key);
#define SHARKSSL_RSA_NO_PADDING      0
#define SHARKSSL_RSA_PKCS1_PADDING   1
/* use the above as padding parameter in the following functions */ 
int sharkssl_RSA_public_encrypt(U16 len, U8 *in, U8 *out, SharkSslRSAKey key, U8 padding);
int sharkssl_RSA_private_decrypt(U16 len, U8 *in, U8 *out, SharkSslRSAKey privkey, U8 padding);
#endif

#if SHARKSSL_ENABLE_CERTSTORE_API
#ifndef BA_API  /* standalone SharkSSL */
#define BA_API SHARKSSL_API
typedef U8 BaBool;
#endif
#include "DoubleList.h"

typedef struct SharkSslCertStore
{
      DoubleList certList;
      U16 elements; /* number of elements in list */
} SharkSslCertStore;

SHARKSSL_API void  SharkSslCertStore_constructor(SharkSslCertStore *o);
SHARKSSL_API void  SharkSslCertStore_destructor(SharkSslCertStore* o);
#define            SharkSslCertStore_release(o) SharkSslCertStore_destructor(o)
SHARKSSL_API int   SharkSslCertStore_add(SharkSslCertStore *o, const char *cert, U32 certlen);
SHARKSSL_API int   SharkSslCertStore_assemble(SharkSslCertStore *o, SharkSslCAList *outList);

/* the following is to release the "outList" object produced by SharkSslCertStore_assemble */
#define            SharkSslCAList_free(list) baFree((void*)list)
#endif  /* SHARKSSL_ENABLE_CERTSTORE_API */

#ifdef __cplusplus
}


/**
 *  C++ wrapper code - SharkSsl object
 */
inline SharkSsl::SharkSsl() {}

inline SharkSsl::SharkSsl(SharkSsl_Role role, 
                          #if (!SHARKSSL_DISABLE_RSA)
                          SharkSslCert cert, 
                          #endif
                          U16 cacheSize, 
                          U16 inBufStartSize, 
                          U16 outBufSize 
                          #if (!SHARKSSL_DISABLE_RSA)
                          ,
                          SharkSslCAList caList
                          #endif
                          ) 
{
   SharkSsl_constructor(this, 
                        role, 
                        #if (!SHARKSSL_DISABLE_RSA)
                        cert, 
                        #endif
                        cacheSize, 
                        inBufStartSize, 
                        outBufSize
                        #if (!SHARKSSL_DISABLE_RSA)
                        , 
                        caList
                        #endif
                        );
}
inline SharkSsl::~SharkSsl() {
   SharkSsl_destructor(this);
}
inline SharkSslCon *SharkSsl::createCon(void) {
   return SharkSsl_createCon(this);
}
inline void SharkSsl::terminateCon(SharkSslCon *sslCon) {
   SharkSsl_terminateCon(this, sslCon);
}
#endif  /* __cplusplus */

#endif
