/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __QCOM__BASE_H__
#define __QCOM__BASE_H__

/*
 * Intended for inclusion only in pseudoHost files,
 * this pulls in some basic definitions.
 */

#include "qcom/basetypes.h"
//#include "athdefs.h"

#define PREPACK
#define POSTPACK
#define PREPACK64
#define POSTPACK64
//#include "targaddrs.h"

typedef enum {
    A_ERROR = -1,               /* Generic error return */
    A_OK = 0,                   /* success */
                                /* Following values start at 1 */
    A_DEVICE_NOT_FOUND,         /* not able to find PCI device */
    A_NO_MEMORY,                /* not able to allocate memory, not available */
    A_MEMORY_NOT_AVAIL,         /* memory region is not free for mapping */
    A_NO_FREE_DESC,             /* no free descriptors available */
    A_BAD_ADDRESS,              /* address does not match descriptor */
    A_WIN_DRIVER_ERROR,         /* used in NT_HW version, if problem at init */
    A_REGS_NOT_MAPPED,          /* registers not correctly mapped */
    A_EPERM,                    /* Not superuser */
    A_EACCES,                   /* Access denied */
    A_ENOENT,                   /* No such entry, search failed, etc. */
    A_EEXIST,                   /* The object already exists (can't create) */
    A_EFAULT,                   /* Bad address fault */
    A_EBUSY,                    /* Object is busy */
    A_EINVAL,                   /* Invalid parameter */
    A_EMSGSIZE,                 /* Inappropriate message buffer length */
    A_ECANCELED,                /* Operation canceled */
    A_ENOTSUP,                  /* Operation not supported */
    A_ECOMM,                    /* Communication error on send */
    A_EPROTO,                   /* Protocol error */
    A_ENODEV,                   /* No such device */
    A_EDEVNOTUP,                /* device is not UP */
    A_NO_RESOURCE,              /* No resources for requested operation */
    A_HARDWARE,                 /* Hardware failure */
    A_PENDING,                  /* Asynchronous routine; will send up results la
ter (typically in callback) */
    A_EBADCHANNEL,              /* The channel cannot be used */
    A_DECRYPT_ERROR,            /* Decryption error */
    A_PHY_ERROR,                /* RX PHY error */
    A_CONSUMED,                 /* Object was consumed */
    A_CLONE,                    /* The buffer is cloned */
} A_STATUS;

#define A_SUCCESS(x)        (x == A_OK)
#define A_FAILED(x)         (!A_SUCCESS(x))

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif
#define AR6006_HOST_INTEREST_ADDRESS                0x00428800
#if 1
PREPACK64 struct host_interest_s { 
    /*
 *      * Pointer to application-defined area, if any.
 *           * Set by Target application during startup.
 *                */
    A_UINT32               hi_app_host_interest;                      /* 0x00 */

    /* Pointer to register dump area, valid after Target crash. */
    A_UINT32               hi_failure_state;                          /* 0x04 */

    /* Pointer to debug logging header */
    A_UINT32               hi_dbglog_hdr;                             /* 0x08 */
    
    /* shared memory to communicate with hostproxy */
    A_UINT32               hi_hostproxy_ctrl;                         /* 0x0c */

    /*
 *      * General-purpose flag bits, similar to AR6000_OPTION_* flags.
 *           * Can be used by application rather than by OS.
 *                */
    A_UINT32               hi_option_flag;                            /* 0x10 */

    /*
 *      * Boolean that determines whether or not to
 *           * display messages on the serial port.
 *                */
    A_UINT32               hi_serial_enable;                          /* 0x14 */

    /* Start address of DataSet index, if any */
    A_UINT32               hi_dset_list_head;                         /* 0x18 */

    /* Override Target application start address */
    A_UINT32               hi_app_start;                              /* 0x1c */

    /* Clock and voltage tuning */
    A_UINT32               hi_skip_clock_init;                        /* 0x20 */
    A_UINT32               hi_core_clock_setting;                     /* 0x24 */
    A_UINT32               hi_cpu_clock_setting;                      /* 0x28 */
    A_UINT32               hi_system_sleep_setting;                   /* 0x2c */
    A_UINT32               hi_xtal_control_setting;                   /* 0x30 */
    A_UINT32               hi_pll_ctrl_setting_24ghz;                 /* 0x34 */
    A_UINT32               hi_pll_ctrl_setting_5ghz;                  /* 0x38 */
    A_UINT32               hi_ref_voltage_trim_setting;               /* 0x3c */
    A_UINT32               hi_clock_info;                             /* 0x40 */

    /*
 *      * Flash configuration overrides, used only
 *           * when firmware is not executing from flash.
 *                * (When using flash, modify the global variables
 *                     * with equivalent names.)
 *                          */
    A_UINT32               hi_bank0_addr_value;                       /* 0x44 */
    A_UINT32               hi_bank0_read_value;                       /* 0x48 */
    A_UINT32               hi_bank0_write_value;                      /* 0x4c */
    A_UINT32               hi_bank0_config_value;                     /* 0x50 */

    /* Pointer to Board Data  */
    A_UINT32               hi_board_data;                             /* 0x54 */

    /*
 *      * Indication of Board Data state:
 *           *    0: board data is not yet initialized.
 *                *    1: board data is initialized; unknown size
 *                     *   >1: number of bytes of initialized board data (varies with board type)
 *                          */
    A_UINT32               hi_board_data_initialized;                 /* 0x58 */

    A_UINT32               hi_dset_RAM_index_table;                   /* 0x5c */

    A_UINT32               hi_desired_baud_rate;                      /* 0x60 */
    A_UINT32               hi_dbglog_config;                          /* 0x64 */
    A_UINT32               hi_end_RAM_reserve_sz;                     /* 0x68 */
    A_UINT32               hi_mbox_io_block_sz;                       /* 0x6c */

    A_UINT32               hi_num_bpatch_streams;                     /* 0x70 -- unused */
    A_UINT32               hi_mbox_isr_yield_limit;                   /* 0x74 */

    A_UINT32               hi_refclk_hz;                              /* 0x78 */
    A_UINT32               hi_ext_clk_detected;                       /* 0x7c */
    A_UINT32               hi_dbg_uart_txpin;                         /* 0x80 */
    A_UINT32               hi_dbg_uart_rxpin;                         /* 0x84 */
    A_UINT32               hi_hci_uart_baud;                          /* 0x88 */
    A_UINT32               hi_hci_uart_pin_assignments;               /* 0x8C */
        /* NOTE: byte [0] = tx pin, [1] = rx pin, [2] = rts pin, [3] = cts pin */
    A_UINT32               hi_hci_uart_baud_scale_val;                /* 0x90 */
    A_UINT32               hi_hci_uart_baud_step_val;                 /* 0x94 */

    A_UINT32               hi_allocram_start;                         /* 0x98 */
    A_UINT32               hi_allocram_sz;                            /* 0x9c */
    A_UINT32               hi_hci_bridge_flags;                       /* 0xa0 */
    A_UINT32               hi_hci_uart_support_pins;                  /* 0xa4 */
        /* NOTE: byte [0] = RESET pin (bit 7 is polarity), bytes[1]..bytes[3] are for future use */
    A_UINT32               hi_hci_uart_pwr_mgmt_params;               /* 0xa8 */
        /* 0xa8 - [1]: 0 = UART FC active low, 1 = UART FC active high
 *          *        [31:16]: wakeup timeout in ms
 *                   */
    /* Pointer to extended board Data  */
    A_UINT32               hi_board_ext_data;                         /* 0xac */
    A_UINT32               hi_board_ext_data_config;                  /* 0xb0 */
        /* 
 *          * Bit [0]  :   valid
 *                   * Bit[31:16:   size 
 *                            */
   /*
 *      * hi_reset_flag is used to do some stuff when target reset.  
 *           * such as restore app_start after warm reset or  
 *                * preserve host Interest area, or preserve ROM data, literals etc.
 *                     */
    A_UINT32                hi_reset_flag;                            /* 0xb4 */
    /* indicate hi_reset_flag is valid */        
    A_UINT32                hi_reset_flag_valid;                      /* 0xb8 */        
    A_UINT32               hi_hci_uart_pwr_mgmt_params_ext;           /* 0xbc */
        /* 0xbc - [31:0]: idle timeout in ms
 *          */
        /* ACS flags */     
    A_UINT32               hi_acs_flags;                              /* 0xc0 */
    A_UINT32               hi_console_flags;                          /* 0xc4 */     
    A_UINT32               hi_nvram_state;                            /* 0xc8 */
    A_UINT32               hi_option_flag2;                           /* 0xcc */

    /* If non-zero, override values sent to Host in WMI_READY event. */
    A_UINT32               hi_sw_version_override;                    /* 0xd0 */
    A_UINT32               hi_abi_version_override;                   /* 0xd4 */

    /* Percentage of high priority RX traffic to total expected RX traffic - 
 *      * applicable only to ar6004 */
    A_UINT32               hi_hp_rx_traffic_ratio;                    /* 0xd8 */

    /* test applications flags */
    A_UINT32               hi_test_apps_related    ;                  /* 0xdc */
    /* location of test script */
    A_UINT32               hi_ota_testscript;                         /* 0xe0 */
    /* location of CAL data */
    A_UINT32               hi_cal_data;                               /* 0xe4 */

    /* Number of packet log buffers */
    A_UINT32               hi_pktlog_num_buffers;                     /* 0xe8 */

    /* wow extension configuration */
    A_UINT32               hi_wow_ext_config;                         /* 0xec */

    A_UINT32               hi_pwr_save_flags;                        /*  0xf0 */

    /* Spatial Multiplexing Power Save (SMPS) options */
    A_UINT32               hi_smps_options;                           /* 0xf4 */
        /* rx coalescing filter configuration */
    A_UINT32               hi_rx_coalesce_filter_config;              /* 0xf8 */    

    A_UINT32               hi_bt_sharing_info;                        /* 0xfc */

#if 1 //(HOST_INTEREST_MAX_SIZE > 0x100)

    A_UINT32               hi_i2c_slv_cfg;                          /* 0x100 */

    A_UINT32			   hi_reserved_3;                                /* 0x104*/
    A_UINT32               hi_reserved_4;                                /* 0x108 */
    A_UINT32               hi_reserved_5;                                  /* 0x10c */
    A_UINT32               hi_reserved_6;                             /* 0x110 */

    /* RTOS-specific pointer-sized value */
    A_UINT32               hi_rtos;                                   /* 0x114 */
#endif /*HOST_INTEREST_MAX_SIZE*/
} POSTPACK64;
#endif

/* Pointed to by HOST_INTEREST->hi_rtos */
struct rtos_s {
    void *entry_targ; /* Two entry points, one for Target domain */
    void *entry_host; /* and a second for pseudo-Host domain. */
};

#if defined(AR6002_REV7)
#define HOST_INTEREST ((struct host_interest_s *)AR6006_HOST_INTEREST_ADDRESS)
#endif

extern void *memset(void *dest, int val, unsigned int nbyte);
extern void *memcpy(void *dest, const void *src, unsigned int nbyte);
extern int   memcmp(const void *string1, const void *string2, unsigned int nbyte);

#define A_PRINTF cmnos_printf
#define A_MEMCPY          memcpy
#define A_MEMSET          memset
#define A_STRCPY          strcpy
#define A_STRLEN          strlen

extern int cmnos_printf(const char * format, ...);

#endif /* __QCOM__BASE_H__ */
