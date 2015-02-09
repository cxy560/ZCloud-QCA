
#ifndef _QCA_SNIFFER_H_
#define _QCA_SNIFFER_H_

/** 
 * @brief: sniffer mode 
 * SCAN_CHANNEL_MODE：sniffer前扫描信道，只探测扫描到的AP所在信道,有点是能加快锁定信道的速度
 * ALL_CHANNEL_MODE：扫描所有信道（1~13） 
*/
typedef enum  
{
	SCAN_CHANNEL_MODE = 1,			  
	ALL_CHANNEL_MODE				 
}tSnifferMode;


/** 
 * @brief: sniffer回调函数指针
*/
typedef void (*sniffer_cb)(char *ssid, char *password, unsigned char response);

/** 
 * @brief: 启动sniffer
 *	mode： sniffer扫描模式
 *	sniffer_func：sniffer成功后的回调函
*/
/**
***************************************************************************
*@brief		：	启动sniffer
*@param[in]	：	mode：			sniffer扫描模式
*				sniffer_func：	sniffer成功后的回调函
*@return	：	none
*@see		：	
***************************************************************************
*/
void sniffer_start(tSnifferMode mode, sniffer_cb sniffer_func);

/**
***************************************************************************
*@brief		：	其它线程判断是否需要挂起线程
*@param[in]	：	0	不挂起
*				1	挂起
*@return	：	none
*@see		：	sniffer关键时刻其他线程最好不要打断，最好挂起，
*				已知必须挂起的任务是扫描AP,因为扫描AP需要几秒且会导致sniffer
*				不能正常接收数据
***************************************************************************
*/
int qca_get_thread_suspend(void);


#endif

