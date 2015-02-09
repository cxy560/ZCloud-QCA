/**
 * @file
 */
/******************************************************************************
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$

 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the license is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the license for the specific language governing permissions and
 *    limitations under the license.
 ******************************************************************************/

#ifndef __QCOM_SYSTEM_H__
#define __QCOM_SYSTEM_H__

#ifdef __cplusplus
extern "C" {
#endif
/**
 *This function is used to do the initial configuration before ROM continues to load board data and other content from NVRAM. 
 *It must be called by the first executable image in NVRAM, typically sdk_proxy.out. It takes a 32-bit parameter. Bit[31-8] 
 *is the user application start address passed to ROM (typically 0xa02800). You can take a look at 
 *demos/sdk_shell/sdk_shell.target.ld and sdk_flash/1_0_sdk_flash.sh and see how the parameter is calculated and passed.
 *
 * @param param 32-bit parameters. Bit[31-8] is start address of user application (<<8). Bit[7-0] is reserved for future use.
 */
void qcom_sys_init(unsigned int param);

/**
 *This function is to start WLAN operation. It installs a lot of patches then calls wlan_main() to start the WLAN operation.
 *After wlan_main(), it also does some post-processing to setup WMI and HTC correctly. This function must be called firstly 
 *before other user functions.
 *
 * @param fn		user main function. RTOS will schedule it after init
 */
void qcom_sys_start(void(*fn)(void));

/**
 *This function is to reset to chip. It is only a warm reset by pointing PC to reset vector. We may change it later to support 
 watch dog reset or cold reset.
 */
void qcom_sys_reset(void);

#ifdef __cplusplus
}
#endif

#endif

