/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */
  
#ifndef __TIMETYPE_H__
#define __TIMETYPE_H__

#ifndef __time_t
typedef unsigned long __time_t;
#endif

#ifndef __suseconds_t
typedef unsigned long __suseconds_t;
#endif

#ifndef time_t
typedef unsigned long time_t;
#endif

struct timeval
{
    __time_t tv_sec;        /* Seconds. */
    __suseconds_t tv_usec;    /* Microseconds. */
};

#endif

