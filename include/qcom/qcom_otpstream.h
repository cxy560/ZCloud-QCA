/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#ifndef __QCOM_OTPSTREAM_H__
#define __QCOM_OTPSTREAM_H__

#ifdef __cplusplus
extern "C" {
#endif
 /* init otp stream operation
	op: 1 for write operation 0 for read operation
	might use before qcom_otp_stream_read if you want to read from the start of otp stream
	return -1 for fail 
*/
int qcom_otp_stream_init(int op);
 /* Reads bytes of data from the otp
	 nbytes for num of read bytes
	return value:  -1 for fail 
*/
int qcom_otp_stream_read(A_UINT8 *buf, unsigned int buflen, unsigned int *nbytes);

 /* Writes bytes of data to the otp
	 nbytes for num of written bytes
	return value:  -1 for fail 
*/
int qcom_otp_stream_write(const A_UINT8 *data, int nbytes);


#ifdef __cplusplus
}
#endif

#endif

