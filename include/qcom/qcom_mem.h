/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __QCOM__MEM_H__
#define __QCOM__MEM_H__

/**
 * This function is a replacement of standard malloc(). It's a Memroy Management API to alloc a buffer.
 *
 * @param size  size of buffer to be allocated
 * @return              the pointer to the buffer on success or NULL on failure
 */
void *mem_alloc(int size);

/**
 * This function is a replacement of standard free().
 *
 * @param ptr           pointer to the buffer to be freed.
 */
void mem_free(void *ptr);

/* alloc a buffer  & clear*/
void *mem_calloc(unsigned int n, unsigned int size);

/* realloc a poniter */
void *mem_realloc(void *p, unsigned int size);

/**
 * This function tells you the available size of heap.
 *
 * @return      Available heap size
 */
int mem_left(void);

#define qcom_mem_alloc mem_alloc
#define qcom_mem_free  mem_free
#define qcom_mem_calloc mem_calloc
#define qcom_mem_realloc mem_realloc
#define qcom_mem_left mem_left

#endif
