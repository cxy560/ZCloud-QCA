/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __QCOM__UTILS_H__
#define __QCOM__UTILS_H__

#define A_MEMSET  memset
#define A_MEMCPY  memcpy
#define A_MEMZERO(addr, size) memset((char *)(addr), (int)0, (int)(size))
#define A_MEMCMP(p1, p2, nbytes) memcmp((void *)(p1), (void *)(p2), (int)(nbytes))
#define A_MEMMOVE(dst, src, size)  memmove(dst, src, size)

#define A_STRLEN   strlen
#define A_STRCPY   strcpy
#define A_STRNCPY  strncpy
#define A_STRCMP   strcmp
#define A_STRNCMP  strncmp
#define A_STRSTR(str1,str2) strstr(str1,str2) 

#define A_LE_READ_4(p)                                              \
        ((A_UINT32)(                                                \
        (((A_UINT8 *)(p))[0]      ) | (((A_UINT8 *)(p))[1] <<  8) | \
        (((A_UINT8 *)(p))[2] << 16) | (((A_UINT8 *)(p))[3] << 24)))

#define A_LE_READ_2(p)                                              \
        ((A_UINT16)(                                                \
        (((A_UINT8 *)(p))[0]) | (((A_UINT8 *)(p))[1] <<  8)))


extern int sscanf(const char *s, const char *format, ... );
#define A_SSCANF                  sscanf

int snprintf(char *s, unsigned int n, const char *fmt, ...);
int sprintf(char *s, const char *fmt, ...);
int printf(const char *fmt, ...);

#define qcom_snprintf snprintf
#define qcom_sprintf sprintf
#define qcom_printf printf

int atoi(char *buf);

void qcom_thread_msleep(unsigned long ms);

#endif /* __QCOM__UTILS_H__ */
