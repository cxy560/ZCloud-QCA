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
 *   $Id: //depot/sw/branches/olca_IOT_3_1_fc/target/import/rev7/include/sharkSSL/SharkSSL_cfg.h#2 $
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
 
#ifndef _SharkSsl_cfg_h
#define _SharkSsl_cfg_h

/****** Start of QCA Configuration *******************/

#define KINGFISHER_THREADX_PLATFORM
//#define SSL_ONLY_AES_128 1

#ifdef KINGFISHER_THREADX_PLATFORM

/******************************************
	Special SSL Feature Enable for Different Features
********************************************/

#ifdef SSL_ONLY_AES_128
#define SHARKSSL_SSL_SERVER_CODE                    0
#define SHARKSSL_SSL_CLIENT_CODE                    1

#define SHARKSSL_ENABLE_TLS_1_2                     0
#define SHARKSSL_ENABLE_TLS_1_1                     1
#define SHARKSSL_ENABLE_SSL_3_0                     0
#define SHARKSSL_USE_AES_256                        0
#define SHARKSSL_USE_AES_128                        1
#define SHARKSSL_ENABLE_AES_GCM                     0
#define SHARKSSL_USE_3DES                           0
#define SHARKSSL_USE_ARC4                           0
#define SHARKSSL_USE_DES                            0
#define SHARKSSL_USE_NULL_CIPHER                    0

#define SHARKSSL_ENABLE_AES_CTR_MODE                0

#define SHARKSSL_ENABLE_DHE_RSA                     0

#else
#define SHARKSSL_SSL_SERVER_CODE                    1
#define SHARKSSL_SSL_CLIENT_CODE                    1

#define SHARKSSL_ENABLE_TLS_1_2                     1
#define SHARKSSL_ENABLE_TLS_1_1                     1
#define SHARKSSL_ENABLE_SSL_3_0                     1

#define SHARKSSL_USE_AES_256                        1
#define SHARKSSL_USE_AES_128                        1
#define SHARKSSL_ENABLE_AES_GCM                     1
#define SHARKSSL_USE_3DES                           1
#define SHARKSSL_USE_ARC4                           1
#define SHARKSSL_USE_DES                            1
#define SHARKSSL_USE_NULL_CIPHER                    1

#define SHARKSSL_ENABLE_AES_CTR_MODE                0

#define SHARKSSL_ENABLE_DHE_RSA                     1
#endif /*SSL_ONLY_AES_128*/

/*****************************************
		Common SSL Feature Enable 
****************************************/
#define SHARKSSL_ENABLE_PSK                         0
#define SHARKSSL_ENABLE_RSA_API                     1
#define SHARKSSL_ENABLE_RSA_PKCS1                   1
#define SHARKSSL_ENABLE_PEM_API                     1
#define SHARKSSL_DISABLE_RSA	0

/*certStore and certInfo */
#define SHARKSSL_ENABLE_CERTSTORE_API 				1
#define SHARKSSL_ENABLE_CLONE_CERTINFO              1


/*****************************************
		System Configuration 
****************************************/
#define SHARKSSL_ENABLE_RSA_BLINDING                1
#define SHARKSSL_ENABLE_SESSION_CACHE               1 

#define SHARKSSL_BIGINT_EXP_SLIDING_WINDOW_K        4
#define SHARKSSL_BIGINT_MULT_LOOP_UNROLL            1

#define SHARKSSL_DES_CIPHER_LOOP_UNROLL             1
#define SHARKSSL_AES_CIPHER_LOOP_UNROLL             1

#define SHARKSSL_USE_RNG_TINYMT 0 
#define SHARKSSL_USE_CAU 0


/***following macros are defined for compilation **** */
#define SHARKSSL_OPTIMIZED_BIGINT_ASM 0
#define __COLDFIRE__ 0
#define __ICCARM__	0

/*If Not enable AES_256 */
#if !SHARKSSL_USE_AES_256
#define SHARKSSL_DHE_RSA_WITH_AES_256_CBC_SHA256 0
#define SHARKSSL_DHE_RSA_WITH_AES_256_CBC_SHA 0
#define SHARKSSL_SSL_RSA_WITH_AES_256_CBC_SHA256 0
#define SHARKSSL_SSL_RSA_WITH_AES_256_CBC_SHA 0
#endif /*!SHARKSSL_USE_AES_256*/

#if !SHARKSSL_ENABLE_DHE_RSA
#define SHARKSSL_DHE_RSA_WITH_AES_128_CBC_SHA256   0
#define SHARKSSL_DHE_RSA_WITH_AES_128_CBC_SHA 0
#define SHARKSSL_DHE_RSA_WITH_3DES_EDE_CBC_SHA 0
#endif /*!SHARKSSL_ENABLE_DHE_RSA*/

#if !SHARKSSL_ENABLE_AES_GCM
#define SHARKSSL_SSL_RSA_WITH_AES_128_GCM_SHA256   0
#endif /*!SHARKSSL_ENABLE_AES_GCM*/

#if !SHARKSSL_ENABLE_TLS_1_2
#define SHARKSSL_SSL_RSA_WITH_AES_128_CBC_SHA256   0
#define SHARKSSL_DHE_RSA_WITH_AES_256_CBC_SHA256	0
#define SHARKSSL_DHE_RSA_WITH_AES_128_GCM_SHA256 0
#define SHARKSSL_DHE_RSA_WITH_AES_128_CBC_SHA256 0
#define SHARKSSL_SSL_RSA_WITH_AES_256_CBC_SHA256 0
#define SHARKSSL_SSL_RSA_WITH_AES_128_GCM_SHA256 0
#endif /*!SHARKSSL_ENABLE_TLS_1_2*/

#if !SHARKSSL_USE_3DES
#define SHARKSSL_SSL_RSA_WITH_3DES_EDE_CBC_SHA     0
#endif /*!SHARKSSL_USE_3DES*/

#if !SHARKSSL_ENABLE_AES_GCM
#define SHARKSSL_DHE_RSA_WITH_AES_128_GCM_SHA256   0
#endif /*!SHARKSSL_ENABLE_AES_GCM*/

#if !SHARKSSL_USE_ARC4
#define SHARKSSL_SSL_RSA_WITH_ARC4_128_SHA   0
#endif /*!SHARKSSL_USE_ARC4*/
/*PSK */
#if !SHARKSSL_ENABLE_PSK
#define SHARKSSL_PSK_WITH_AES_256_CBC_SHA 0
#define SHARKSSL_PSK_WITH_AES_128_CBC_SHA 0
#define SHARKSSL_PSK_WITH_3DES_EDE_CBC_SHA 0
#define SHARKSSL_PSK_WITH_ARC4_128_SHA 0
#endif
#endif /*KINGFISHER_THREADX_PLATFORM */

/****** End of QCA COnfiguration********************/

/**
 * ciphers selection - at least one must be 1
 */
#ifndef SHARKSSL_USE_AES_256
#define SHARKSSL_USE_AES_256                        1
#endif

#ifndef SHARKSSL_USE_AES_128
#define SHARKSSL_USE_AES_128                        1
#endif

/**
 * AES-GCM requires AES - ciphersuites supported only by TLS 1.2
 */
#ifndef SHARKSSL_ENABLE_AES_GCM
#define SHARKSSL_ENABLE_AES_GCM                     1
#endif

#ifndef SHARKSSL_USE_3DES
#define SHARKSSL_USE_3DES                           1
#endif

#ifndef SHARKSSL_USE_ARC4
#define SHARKSSL_USE_ARC4                           1
#endif

#ifndef SHARKSSL_USE_DES
#define SHARKSSL_USE_DES                            0
#endif

#ifndef SHARKSSL_USE_NULL_CIPHER
#define SHARKSSL_USE_NULL_CIPHER                    0
#endif


#ifndef SHARKSSL_USE_SHA1_MAC_ONLY
#define SHARKSSL_USE_SHA1_MAC_ONLY                  1
#endif


/**
 * select 1 to enable SERVER side TLS
 */
#ifndef SHARKSSL_SSL_SERVER_CODE
#define SHARKSSL_SSL_SERVER_CODE                    1
#endif


/**
 * select 1 to accept client hello v2.0 format 
 * (sent by some browsers)
 */
#ifndef SHARKSSL_ACCEPT_CLIENT_HELLO_2_0
#define SHARKSSL_ACCEPT_CLIENT_HELLO_2_0            1
#endif


/**
 * select 1 to enable client authentication from server
 */
#ifndef SHARKSSL_ENABLE_CLIENT_AUTH
#define SHARKSSL_ENABLE_CLIENT_AUTH                 1 
#endif


/**
 * select 1 to enable CLIENT side TLS
 */
#ifndef SHARKSSL_SSL_CLIENT_CODE
#define SHARKSSL_SSL_CLIENT_CODE                    1
#endif


/**
 * select 1 to enable RSA blinding (more secure, more ROM, more RAM)
 */
#ifndef SHARKSSL_ENABLE_RSA_BLINDING
#define SHARKSSL_ENABLE_RSA_BLINDING                1
#endif


/**
 * select 1 to enable session caching
 */
#ifndef SHARKSSL_ENABLE_SESSION_CACHE
#define SHARKSSL_ENABLE_SESSION_CACHE               1
#endif


/**
 * select 1 to enable TLS 1.2 (supporting SHA-256 and AES-GCM)
 * enabling TLS 1.2 will enable also TLS 1.1
 */
#ifndef SHARKSSL_ENABLE_TLS_1_2
#define SHARKSSL_ENABLE_TLS_1_2                     1
#endif
/**
 * TLS 1.2 requires SHA-256, do not modify the following #if..#elif
 */
#if SHARKSSL_ENABLE_TLS_1_2       
#undef  SHARKSSL_USE_SHA_256
#define SHARKSSL_USE_SHA_256                        1
#undef  SHARKSSL_USE_DES
#ifdef KINGFISHER_THREADX_PLATFORM
#define SHARKSSL_USE_DES                            1
#else
#define SHARKSSL_USE_DES                            0
#endif /*KINGFISHER_THREADX_PLATFORM*/
#undef  SHARKSSL_USE_SHA1_MAC_ONLY
#define SHARKSSL_USE_SHA1_MAC_ONLY                  0

#elif (!defined(SHARKSSL_USE_SHA_256))
#define SHARKSSL_USE_SHA_256                        0

#endif


/**
 * select 1 to enable TLS 1.1 (more secure, slower than TLS 1.0)
 */
#ifndef SHARKSSL_ENABLE_TLS_1_1
#define SHARKSSL_ENABLE_TLS_1_1                     1
#endif


/**
 * select 1 to enable SSL 3.0 (backward compatibility)
 */
#ifndef SHARKSSL_ENABLE_SSL_3_0
#define SHARKSSL_ENABLE_SSL_3_0                     0
#endif


/**
 * select 1 to enable DHE_RSA ciphersuites
 */
#ifndef SHARKSSL_ENABLE_DHE_RSA
#define SHARKSSL_ENABLE_DHE_RSA                     1
#endif


/**
 * select 1 to enable PSK ciphersuites - experimental
 */
#ifndef SHARKSSL_ENABLE_PSK
#define SHARKSSL_ENABLE_PSK                         0
#endif


/**
 * select 1 to enable RSA API (sharkssl_RSA_public_encrypt, 
 * sharkssl_RSA_private_decrypt, SharkSslRSAKey_size)
 */
#ifndef SHARKSSL_ENABLE_RSA_API
#define SHARKSSL_ENABLE_RSA_API                     1
#endif


/**
 * select 1 to enable PKCS1 padding in RSA API (RSA_API must be enabled)
 */
#ifndef SHARKSSL_ENABLE_RSA_PKCS1
#define SHARKSSL_ENABLE_RSA_PKCS1                   1
#endif


/**
 * select 1 to enable PEM certs/keys decoding 
 * if RSA_API is enabled, then also the function 
 * sharkssl_PEM_to_RSAKey is available
 */
#ifndef SHARKSSL_ENABLE_PEM_API 
#define SHARKSSL_ENABLE_PEM_API                     1
#endif


/**
 * select 1 to enable cipher/protocol retrieval functions 
 */
#ifndef SHARKSSL_ENABLE_INFO_API
#define SHARKSSL_ENABLE_INFO_API                    1
#endif


/**
 * select 1 to enable certificate chain support
 */
#ifndef SHARKSSL_ENABLE_CERT_CHAIN
#define SHARKSSL_ENABLE_CERT_CHAIN                  1
#endif


/**
 * select 1 to enable CA check 
 * (client or server with client auth)
 */
#ifndef SHARKSSL_ENABLE_CA_LIST
#define SHARKSSL_ENABLE_CA_LIST                     1
#endif 


/**
 * select 1 to enable certificate storage 
 */
#ifndef SHARKSSL_ENABLE_CERTSTORE_API
#define SHARKSSL_ENABLE_CERTSTORE_API               1
#endif


/**
 * select 1 to enable SharkSslCon_cloneCertInfo function
 */
#ifndef SHARKSSL_ENABLE_CLONE_CERTINFO
#define SHARKSSL_ENABLE_CLONE_CERTINFO              1
#endif


/**
 * select 1 (small ROM footprint, slow) or 0 (large, fast)
 */
#ifndef SHARKSSL_MD5_SMALL_FOOTPRINT
#define SHARKSSL_MD5_SMALL_FOOTPRINT                0
#endif

#ifndef SHARKSSL_SHA1_SMALL_FOOTPRINT
#define SHARKSSL_SHA1_SMALL_FOOTPRINT               0
#endif

#ifndef SHARKSSL_SHA256_SMALL_FOOTPRINT
#define SHARKSSL_SHA256_SMALL_FOOTPRINT             0
#endif


/**
 * select a window size between 1 (slower, less RAM) and 5
 */
#ifndef SHARKSSL_BIGINT_EXP_SLIDING_WINDOW_K
#define SHARKSSL_BIGINT_EXP_SLIDING_WINDOW_K        4
#endif


/**
 * select 0 (slower, less ROM) or 1 (20% faster, more ROM)
 */
#ifndef SHARKSSL_BIGINT_MULT_LOOP_UNROLL
#define SHARKSSL_BIGINT_MULT_LOOP_UNROLL            1
#endif


/**
 * select 1 to include AES CTR mode (USE_AES_xxx must be enabled)
 */
#ifndef SHARKSSL_ENABLE_AES_CTR_MODE
#define SHARKSSL_ENABLE_AES_CTR_MODE                1
#endif


/**
 * select 0 (45% less ROM) or 1 (10-15% faster)
 */
#ifndef SHARKSSL_DES_CIPHER_LOOP_UNROLL
#define SHARKSSL_DES_CIPHER_LOOP_UNROLL             1
#endif


/**
 * select 0 (35% less ROM) or 1 (10-15% faster)
 */
#ifndef SHARKSSL_AES_CIPHER_LOOP_UNROLL
#define SHARKSSL_AES_CIPHER_LOOP_UNROLL             1
#endif


/**
 * select 1 if your architecture supports unaligned memory 
 * access (x86, ARM-Cortex-M3, ColdFire)
 */
#ifndef SHARKSSL_UNALIGNED_ACCESS
#ifdef UNALIGNED_ACCESS
#define SHARKSSL_UNALIGNED_ACCESS                   1
#else
#define SHARKSSL_UNALIGNED_ACCESS                   0
#endif
#endif


/**
 * select 8, 16 or 32 according to your architecture
 */
#ifndef SHARKSSL_BIGINT_WORDSIZE
#define SHARKSSL_BIGINT_WORDSIZE                   32
#endif


#endif
