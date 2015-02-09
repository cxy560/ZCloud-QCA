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
 *   $Id: //depot/sw/branches/olca_IOT_3_1_fc/target/import/rev7/include/sharkSSL/TargConfig.h#3 $
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
#ifndef _SharkSsl_TargConfig_h
#define _SharkSsl_TargConfig_h

/**
 *  baMalloc  should return 32-bit aligned addresses when succesful,
 *                          (void*)0 when not succesful.
 *  baRealloc should return 32-bit aligned addresses when succesful,
 *                          (void*)0 when not succesful or NOT available.
 */

#include "qcom/qcom_common.h"
//#define DBG_MEM 1

#define NDEBUG
#ifndef NDEBUG
#define baAssert(x)        ((x) ? 0 : (printf("wrong assertion %s %d\n", __FILE__, __LINE__)))
#else
#define baAssert(x)
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif


#ifdef DBG_MEM
#include "ssl_utils.h"
#define baMalloc(s)        ssl_dbg_malloc(s)
#define baRealloc(m, s)    ssl_dbg_realloc(m,s)   
#define baFree(m)          if (m) {ssl_dbg_free(m, NULL,0);}
#else
#define baMalloc(s)        qcom_mem_alloc(s)     /* should return 32-bit aligned address */
#define baRealloc(m, s)    qcom_mem_realloc(m,s)  /* as above - see also #define below    */
#define baFree(m)          if (m) {qcom_mem_free(m);}
#endif /*DBG_MEM*/
#undef strstr
#define strstr(str1,str2) A_STRSTR(str1,str2)

#undef memmove
#define memmove(dst, src,size) A_MEMMOVE(dst,src,size)

#undef memcmp
#define memcmp(str1, str2,size) A_MEMCMP(str1,str2,size)

#undef strcmp
#define strcmp(str1, str2) A_STRCMP(str1,str2)

#undef strlen
#define strlen(str) A_STRLEN(str)
/**
 *  set SHARKSSL_UNALIGNED_MALLOC to 1 if either malloc
 *  or realloc don't return a 32-bit aligned address
 */
#define SHARKSSL_UNALIGNED_MALLOC     0

#define B_LITTLE_ENDIAN 

typedef A_UINT8      U8;
typedef A_INT8      S8;
typedef A_UINT16     U16;
typedef A_INT16     S16; 
typedef A_UINT32 	U32; 
typedef A_INT32		S32; 
typedef A_UINT64 U64;
typedef A_INT64	S64;

/**
 * baGetUnixTime demo implementation
 */
#ifdef _SHARKSSL_C_

//this api needs to be replaced if time api is supported.
static U32 sharkSslDemoTime = 0;

U32 baGetUnixTime(void)
{
   return sharkSslDemoTime++;
}

#else
extern
#ifdef __cplusplus
"C"
#endif
U32 baGetUnixTime(void);

#endif  /* _SHARKSSL_C_ */

/**
 * possible baGetUnixTime alternate implementation
 * #define baGetUnixTime()    0    -- TO BE IMPLEMENTED
 */


typedef struct ThreadMutexBase
{
   U32 mutex;                      /* TO BE IMPLEMENTED */
} ThreadMutexBase;

#define ThreadMutex_destructor(o)  /* TO BE IMPLEMENTED */
#define ThreadMutex_set(o)         /* TO BE IMPLEMENTED */
#define ThreadMutex_release(o)     /* TO BE IMPLEMENTED */
#define ThreadMutex_constructor(o) /* TO BE IMPLEMENTED */

#endif  /* _SharkSsl_TargConfig_h */
