/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */


#ifndef _ROM_MEM_REUSE_ADDR_H_
#define _ROM_MEM_REUSE_ADDR_H_


/*------------------- rom mem unsed -------------------*/
#define ROM_MEM_ADDRS_1    0x0042bb50    //usb_local_pool_136
#define ROM_MEM_SIZE_1     1712        


#define ROM_MEM_ADDRS_2    0x0042c610    //g_bmi_buffer
#define ROM_MEM_SIZE_2     576        



#define ROM_MEM_ADDRS_3    0x00434bc0    //g_pBTCOEXCmnFn
#define ROM_MEM_SIZE_3     404        


#define ROM_MEM_ADDRS_4    0x00434048    //_argos_internal_api                                                       
#define ROM_MEM_SIZE_4     356        




/*------------------- reused as -------------------*/



/* address 1 : 1712B */
#define ROM_MEM_cdr_timer (ROM_MEM_ADDRS_1)                                     //MAX_CDR_TIMER = 16
#define ROM_MEM_cdr_timer_SIZE 448												//MAX_CDR_TIMER * sizeof(cdr_timer_t) = 16 * 28 = 448

#define ROM_MEM_fds (ROM_MEM_cdr_timer + ROM_MEM_cdr_timer_SIZE)                //MAX_FDSET_NUM = 24
																				//MAX_FDSET_NUM * sizeof(fd_set) = 24*13*4 = 1248

/* address 2 : 576B */
#define ROM_MEM_cdr_sock_context        (ROM_MEM_ADDRS_2)						//MAX_SOCKETS = 12
																				//MAX_SOCKETS * sizeof(sock_cb_t) = 12 * 32 = 384

#define ROM_MEM_fd_array (ROM_MEM_cdr_sock_context + 384)						//MAX_FDSET_NUM=24
#define ROM_MEM_fd_array_SIZE 96       											//MAX_FDSET_NUM * sizeof(long) = 24*4 = 96


/* address 3 : 404B */                                              
#define ROM_MEM_g_backup_restore_table  (ROM_MEM_ADDRS_3)        				//struct _BACKUP_RESTORE_TABLE_
																				//sizeof(struct _BACKUP_RESTORE_TABLE_) = 360


/* address 4 : 356B */
#define ROM_MEM_aPendList (ROM_MEM_ADDRS_4)                         			//tPendQuery aPendList[5]
																				//sizeof(tPendQuery) * 5 = 44 * 5 = 220

#define ROM_MEM_g_cdr_host_thread_state (ROM_MEM_aPendList + 220)  				//sizeof(struct cdr_host_thread_state_s)
																				//sizeof(struct cdr_host_thread_state_s) = 84

#endif
