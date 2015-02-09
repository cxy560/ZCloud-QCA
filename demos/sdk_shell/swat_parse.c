/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
#include "swat_wmiconfig_common.h"
#include "swat_parse.h"
#include "qcom/socket_api.h"
#include "swat_bench_core.h"
#include "swat_bench_iniche_1.1.2.h"
#include "qcom_i2c.h"

extern int atoul(char *buf);
//unsigned char valid_5g_channels[] = {183,184,185,187,188,189,192,196,16	,34	,36	,38	,40	,42	,44	,46	,48	,52	,56	,60	,64	,100,104,108,112,116,120,124,128,132,136,140,149,153,157,161,165};
unsigned char valid_5g_channels[] = {36, 40, 44, 48, 149,153,157,161,165};


A_INT32
swat_iwconfig_scan_handle(A_INT32 argc, A_CHAR * argv[])
{
    A_UCHAR wifi_state = 0;
    qcom_get_state(&wifi_state);

    //if(wifi_state!=2)
    {
        //if not connected, scan
        if (argc == 2) {
            swat_wmiconfig_all_bss_scan();
        } else if (argc == 3) {
            swat_wmiconfig_spec_bss_scan(argv[2]);
        } else {
            SWAT_PTF("iwconfig scan [ssid*]\n");
            return -1;
        }
    }

    return 0;
}

A_INT32
swat_wmiconfig_handle(A_INT32 argc, A_CHAR * argv[])
{
    A_INT32 ret = 0;

    if (argc == 1) {
        swat_wmiconfig_information();
        return 0;
    }

    ret += swat_wmiconfig_connect_handle(argc, argv);

#if defined(SWAT_WMICONFIG_SOFTAP)
    ret += swat_wmiconfig_softAp_handle(argc, argv);
#endif
#if defined(SWAT_WMICONFIG_WEP)
    ret += swat_wmiconfig_wep_handle(argc, argv);
#endif
#if defined(SWAT_WMICONFIG_WPA)
    ret += swat_wmiconfig_wpa_handle(argc, argv);
#endif
#if defined(SWAT_WMICONFIG_WPS)
     ret += swat_wmiconfig_wps_handle(argc, argv);
#endif
#if defined(SWAT_WMICONFIG_P2P)
    /* ret += swat_wmiconfig_p2p_handle(argc, argv); */
#endif
#if defined(SWAT_WMICONFIG_IP)
    ret += swat_wmiconfig_ip_handle(argc, argv);
#endif
#if defined(SWAT_WMICONFIG_MISC)
    ret += swat_wmiconfig_misc_handle(argc, argv);
#endif
    if (ret == 0) {
        SWAT_PTF("Unknown wmiconfig command!\n");
    }
}


A_INT32
swat_wmiconfig_connect_handle(A_INT32 argc, A_CHAR * argv[])
{
    if (!A_STRCMP(argv[1], "--connect")) {
        if (argc == 3) {
            swat_wmiconfig_connect_ssid((char *)argv[2]);
        } else {
            A_INT8 ssid[32];
            A_UINT8 i, j;

            j = 0;
            A_MEMSET(ssid, 0, 32);
            for (i=2; i<argc; i++)
            {
                if ((j + strlen(argv[i])) > 32)
                {
                    SWAT_PTF("ssid length is bigger than 32\n");
                    return SWAT_ERROR;
                }
                else
                {
                    do
                    {
                        ssid[j] = *argv[i];
                        j++;
                    } while (*argv[i]++);
                }

				ssid[j-1] = 0x20; //space
            }
			ssid[j-1] = 0;

            swat_wmiconfig_connect_ssid( (char *) ssid);
        }

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--disc")) {
        swat_wmiconfig_connect_disc();
        return SWAT_OK;

    }

    return SWAT_NOFOUND;
}

#if defined(SWAT_WMICONFIG_MISC)
A_INT32
swat_wmiconfig_misc_handle(A_INT32 argc, A_CHAR * argv[])
{
    A_INT32 data = 0;
    A_UINT32 wifiMode;
    A_INT32 data0 = 0;
    A_INT32 data1 = 0;
    A_UINT8 promiscuous = 0;

    if (!A_STRCMP(argv[1], "--help")) {
        swat_wmiconfig_help();
        return SWAT_OK;
    }
   if (!A_STRCMP(argv[1], "--hostname")) {
		   if (argc < 3) {
		            printf("Missing parameter\n");
			     	printf("wmiconfig --hostname get\nwmiconfig --hostname set <name> \n");
		            return SWAT_ERROR;
        	}
		extern A_STATUS qcom_set_dhcpc_hostname(A_UCHAR* pstr);
		extern A_STATUS qcom_get_dhcpc_hostname(A_UCHAR* pstr);
		u_char hostname[33]={0};
		if (!A_STRCMP(argv[2], "get")){
			
			qcom_get_dhcpc_hostname(hostname);
			printf("\r\nDHCP hostname: %s\n", hostname);

	 	}else if (!A_STRCMP(argv[2], "set")){
	 
			qcom_set_dhcpc_hostname((A_UCHAR*)(argv[3]));
			printf("\r\nDHCP hostname: %s\n", argv[3]);
		}else{
 		     printf("wmiconfig --hostname get\nwmiconfig --hostname set <name> \n");
	            return SWAT_ERROR;
		}
		return SWAT_OK;
	   }
   if (!A_STRCMP(argv[1], "--psram")) {
   	     if (argc < 4) {
	            printf("Missing parameter\n");
		     printf("wmiconfig --psram read <addr(DEC)>\nwmiconfig --psram write <addr(DEC)> <val(DEC)>\n");
	            return SWAT_ERROR;
        	}
	extern A_INT32 qcom_psram_read(A_UINT32 offset, A_UCHAR *buf, A_UINT32 size);
	extern A_INT32 qcom_psram_write(A_UINT32 offset, A_UCHAR  *buf, A_UINT32 size);
	A_UINT32  addr,ret,value=0;
	addr = atoi(argv[3]);
	 if (!A_STRCMP(argv[2], "read")){
		ret=qcom_psram_read(addr,(A_UCHAR *)&value,4);
		if(ret>0)
			printf("\r\nPSRAM Read Success addr =0x%x, value = %x\n", addr, value);
		else
			printf("\r\nPSRAM Read Failed!\n");
	 }else if (!A_STRCMP(argv[2], "write")){
		value= atoi(argv[4]);
		ret=qcom_psram_write(addr,(A_UCHAR *)&value,4);
		if(ret>0)
			printf("\r\nPSRAM Write Success addr =0x%x, value = %x\n", addr, value);
		else
			printf("\r\nPSRAM Write Failed!\n");
	}else{
 		     printf("wmiconfig --psram read <addr(DEC)>\nwmiconfig --psram write <addr(DEC)> <val(DEC)>\n");
	            return SWAT_ERROR;
	}
	return SWAT_OK;
    }
	if (!A_STRCMP(argv[1], "--i2c"))	{

   	     if (argc < 4) {
	            printf("Missing parameter\n");
		     printf("wmiconfig --i2c read <addr(DEC)>\nwmiconfig --i2c write <addr(DEC)> <val(DEC)>\n");
	            return SWAT_ERROR;
        	}

		extern A_INT32 eeprom24l_read(A_INT8 DevAddr, A_INT32 addr, A_UCHAR *data, A_INT32 length);
		extern A_INT32 eeprom24l_write(A_INT8 DevAddr , A_INT32 addr, A_UCHAR *data, A_INT32 length);

		A_UINT32  addr,value=0,vtemp;
		addr = atoi(argv[3]);
		 if (!A_STRCMP(argv[2], "read")){
		 	qcom_i2c_init(12,6,5);
			 if (eeprom24l_read(0,addr, (unsigned char *)&value, 4) == 1)
				printf("\r\nI2C Read Success addr =0x%x, value = %x\n", addr, value);
			else
				printf("\r\nI2C Read Failed!\n");
		 }else if (!A_STRCMP(argv[2], "write")){
			vtemp= atoi(argv[4]);
			if (eeprom24l_write(0,addr, (unsigned char *)&vtemp, 4) == 1)
				printf("\r\nI2C Write Success addr =0x%x, value = %x\n", addr, vtemp);
			else
				printf("\r\nI2C Write Failed!\n");
		}else{
	 		     printf("wmiconfig --i2c read <addr(DEC)>\nwmiconfig --psram write <addr(DEC)> <val(DEC)>\n");
		            return SWAT_ERROR;
		}
	
return SWAT_OK;
	}
    if (!A_STRCMP(argv[1], "--wdt")) {
   	     if (argc < 4) {
	            printf("Missing parameter\n");
		     printf("wmiconfig --wdt <state> <timeout>\n\tstate:<0>disable <1>1000ms timer feed <2>cust feed\n");
	            return SWAT_ERROR;
        	}
	   extern void qcom_watchdog(int enable, int timeout);
	   int state=0, timeout=5;
	   if(argc >= 4)
	   {
			state = atoi(argv[2]);
			timeout = atoi(argv[3]);
	   }		
	   qcom_watchdog(state, timeout);
	   return SWAT_OK;
    }
   if (!A_STRCMP(argv[1], "--test")) {
	   if(!A_STRCMP(argv[2], "spi_flash")){
		if(!A_STRCMP(argv[3], "read")){
			A_UINT32 val =0,addr0;
			int cs_pin;
			extern void test_spi_flash_read(int cs_pin,unsigned char *addr, unsigned char *buf, int size);
			addr0=atoi(argv[5]);
			cs_pin = atoi(argv[4]);
			if((9!=cs_pin)&&(19!=cs_pin)){
				printf("cs_pin =9 or 19\n");	
				return SWAT_ERROR;
			}
			test_spi_flash_read(cs_pin,(unsigned char *)&addr0,(unsigned char *)&val,4);
			SWAT_PTF("addr:0x%x val=0x%x \n",addr0,val);		 	
	  		return SWAT_OK;
		 }
		printf("wmiconfig --test spi_flash read <cs_pin> <addr(DEC)>\n");
	   }
    }
    if (!A_STRCMP(argv[1], "--pmparams")) {
        A_UINT32 i;
        A_UINT8 iflag = 0, npflag = 0, dpflag = 0, txwpflag = 0, ntxwflag = 0, pspflag = 0;
        A_INT32 idlePeriod;
        A_INT32 psPollNum;
        A_INT32 dtimPolicy;
        A_INT32 tx_wakeup_policy;
        A_INT32 num_tx_to_wakeup;
        A_INT32 ps_fail_event_policy;

        if (argc < 3) {
            SWAT_PTF("Missing parameter\n");
            return SWAT_ERROR;
        }

        for (i = 2; i < argc; i++) {
            if (!A_STRCMP(argv[i], "--idle")) {
                if ((i + 1) == argc) {
                    SWAT_PTF("Missing parameter\n");
                    return SWAT_ERROR;
                }
                A_SSCANF(argv[i + 1], "%u", &idlePeriod);
                iflag = 1;
                i++;
            } else if (!A_STRCMP(argv[i], "--np")) {
                if ((i + 1) == argc) {
                    SWAT_PTF("Missing parameter\n");
                    return SWAT_ERROR;
                }
                A_SSCANF(argv[i + 1], "%d", &psPollNum);
                npflag = 1;
                i++;
            } else if (!A_STRCMP(argv[i], "--dp")) {
                if ((i + 1) == argc) {
                    SWAT_PTF("Missing parameter\n");
                    return SWAT_ERROR;
                }
                A_SSCANF(argv[i + 1], "%d", &dtimPolicy);
                dpflag = 1;
                i++;
            } else if (!A_STRCMP(argv[i], "--txwp")) {
                if ((i + 1) == argc) {
                    SWAT_PTF("Missing parameter\n");
                    return SWAT_ERROR;
                }
                A_SSCANF(argv[i + 1], "%d", &tx_wakeup_policy);
                txwpflag = 1;
                i++;
            } else if (!A_STRCMP(argv[i], "--ntxw")) {
                if ((i + 1) == argc) {
                    SWAT_PTF("Missing parameter\n");
                    return SWAT_ERROR;
                }
                A_SSCANF(argv[i + 1], "%d", &num_tx_to_wakeup);
                ntxwflag = 1;
                i++;
            } else if (!A_STRCMP(argv[i], "--psfp")) {
                if ((i + 1) == argc) {
                    SWAT_PTF("Missing parameter\n");
                    return SWAT_ERROR;
                }
                A_SSCANF(argv[i + 1], "%d", &ps_fail_event_policy);
                pspflag = 1;
                i++;
            } else if ((i % 2) == 0) {
                SWAT_PTF("wmiconfig --pmparams --idle <time in ms>\n");
                SWAT_PTF("                     --np < > \n");
                SWAT_PTF("                     --dp <1=Ignore 2=Normal 3=Stick 4=Auto>\n");
                SWAT_PTF("                     --txwp <1=wake 2=do not wake>\n");
                SWAT_PTF("                     --ntxw < > \n");
                SWAT_PTF("                     --psfp <1=send fail event 2=Ignore event>\n");
                return SWAT_ERROR;
            }
        }

        if (!iflag) {
            idlePeriod = 0;
        }

        if (!npflag) {
            psPollNum = 10;
        }

        if (!dpflag) {
            dtimPolicy = 3;
        }

        if (!txwpflag) {
            tx_wakeup_policy = 2;
        }

        if (!ntxwflag) {
            num_tx_to_wakeup = 1;
        }

        if (!pspflag) {
            ps_fail_event_policy = 2;
        }

        swat_wmiconfig_pmparams(idlePeriod, psPollNum, dtimPolicy, tx_wakeup_policy,
                                num_tx_to_wakeup, ps_fail_event_policy);
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--pwrmode")) {
        if (argc < 3) {
            SWAT_PTF("Missing parameter\n");
            return SWAT_ERROR;
        }

        A_SSCANF(argv[2], "%d", &data);

        if (data == 0) {
            swat_wmiconfig_power_mode_set(2);   //MAX_PERF_POWER
        } else if (data == 1) {
            qcom_op_get_mode(&wifiMode);
            if (wifiMode == 1) {
                SWAT_PTF("Setting REC Power is not allowed MODE_AP \n");
                return SWAT_ERROR;
            }

            swat_wmiconfig_power_mode_set(1);   //REC_POWER
        } else {
            SWAT_PTF("Unknown power mode.\n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--driver")) {
        if (argc != 3) {
            SWAT_PTF("Missing parameter\n");
            return SWAT_ERROR;
        }

        if (!A_STRCMP(argv[2], "up")) {
            swat_wmiconfig_reset();
        } else if (!A_STRCMP(argv[2], "down")) {
            ;
        } else {
            SWAT_PTF("Invalid parameter\n");;
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--reset")) {
        swat_wmiconfig_reset();
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--rssi")) {
        swat_wmiconfig_rssi_get();
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--settxpower")) {
        if (argc != 3) {
            SWAT_PTF("Missing parameter\n");
            return SWAT_ERROR;
        }

        A_SSCANF(argv[2], "%d", &data);
        swat_wmiconfig_tx_power_set((A_UINT32) data);

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--suspend")) {
        swat_wmiconfig_suspenable();
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--suspstart")) {
        A_UINT32 susp_time;

        swat_wmiconfig_devmode_get(&wifiMode);
        if (wifiMode == 0) {
            SWAT_PTF("Store-Recall Mechanism NOT supported in 'MODE_AP' \n");
            return SWAT_ERROR;
        }

        if (argc == 3) {
            /* xiny */
             susp_time = swat_atoi(argv[2]); 

            swat_wmiconfig_suspstart(susp_time);
        } else {
            SWAT_PTF("    --suspstart <time>\n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--version")) {
        swat_wmiconfig_version(SWAT_SW_VERSION);
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--allow_aggr")) {
        if (argc != 4) {
            SWAT_PTF("Invalid parameter\n");
            return SWAT_ERROR;
        }

        data0 = atoul(argv[2]);
        data1 = atoul(argv[3]);
        if (data0 > 0xff || data1 > 0xff) {
            SWAT_PTF("Invalid parameters, should no more than 0xFF\n");
            return SWAT_ERROR;
        }
        swat_wmiconfig_allow_aggr((A_UINT16) data0, (A_UINT16) data1);
        return SWAT_OK;
    }
    
    if (!strcmp(argv[1], "--promisc")) {
        if (argc == 3) {
			promiscuous = atoi(argv[2]);
			swat_wmiconfig_promiscuous_test(promiscuous);
			return SWAT_OK;
        }
        else {
            SWAT_PTF("    --promisc <value>\n");
            return SWAT_ERROR;
        }		
    }	

    return SWAT_NOFOUND;
}
#endif

#if defined(SWAT_WMICONFIG_SOFTAP)

A_INT32
swat_wmiconfig_softAp_handle(A_INT32 argc, A_CHAR * argv[])
{

    A_INT32 data = 0;
    A_CHAR country_code[3];

    if (!A_STRCMP(argv[1], "--mode")) {
        if (3 == argc) {
            swat_wmiconfig_dev_mode_set((A_CHAR *) argv[2]);
        } else if (4 == argc) {
            if (!A_STRCMP(argv[2], "ap")) {
                if (!A_STRCMP(argv[3], "hidden")) {
                    swat_wmiconfig_ap_hidden_set();
                } else if (!A_STRCMP(argv[3], "wps")) {
                    swat_wmiconfig_ap_wps_set();
                } else {
                    SWAT_PTF("wmiconfig --mode ap <hidden|wps>\n");
                    return SWAT_ERROR;
                }
            } else {
                SWAT_PTF("wmiconfig --mode ap <hidden|wps>\n");
                return SWAT_ERROR;
            }
        } else {
            SWAT_PTF("wmiconfig --mode <ap|station>\n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--channel")) {
        if (argc == 3) {
            A_SSCANF(argv[2], "%d", &data);
            /* 2.4G */
            if (data <= 14) {
                A_CHAR acountry_code[3];
                swat_wmiconfig_country_code_get(acountry_code);
                if (0 == A_STRCMP(acountry_code, "US")) {
                    if ((data < 1) || (data > 11)) {
                        SWAT_PTF("wmiconfig --channel <1-11>\n");
                        return SWAT_ERROR;
                    }
                } else if (0 == A_STRCMP(acountry_code, "JP")) {
                    if ((data < 1) || (data > 14)) {
                        SWAT_PTF("wmiconfig --channel <1-14>\n");
                        return SWAT_ERROR;
                    }
                } else {
                    if ((data < 1) || (data > 13)) {
                        SWAT_PTF("wmiconfig --channel <1-13>\n");
                        return SWAT_ERROR;
                    }
                }
            }
            /* 5G */
            else {
                /*
                SWAT_PTF("%d %d %d %d\n", CHNNEL_5G_PART1_START_NO
                    ,CHNNEL_5G_PART1_END_NO
                    ,CHNNEL_5G_PART2_START_NO
                    ,CHNNEL_5G_PART2_END_NO);
                */
                //SWAT_PTF("Not support 5G channel.\n");
                if (!CHANNEL_NO_IS_5G(data)) {
                    SWAT_PTF("wmiconfig --channel <%d-%d>\n", MIN_NO_5G, MAX_NO_5G);
                    return SWAT_ERROR;
                }                

                #if 0
                int i, valid = 0;
                for (i=0 ; i< sizeof(valid_5g_channels); i++) {
                    if (data == valid_5g_channels[i]) {
                        valid = 1;
                        break;
                    }
                }
                
                if (!valid) {
                    SWAT_PTF("invalid channel %d\n", data);
                    return SWAT_ERROR;
                }
				#endif
            }

            swat_wmiconfig_channel_set((A_UINT32) data);
        } else {
            SWAT_PTF("wmiconfig --channel <channel-id>\n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--wmode")) {
        if (argc != 3) {
            SWAT_PTF("wmiconfig --wmode <b,g,n,ht40>\n");
            return SWAT_ERROR;
        }

        if (0 == A_STRCMP(argv[2], "a")) {
            SWAT_PTF("Not support a\n");
            return SWAT_ERROR;
        } else if (0 == A_STRCMP(argv[2], "b")) {
            swat_wmiconfig_wifi_mode_set((A_UINT8)QCOM_11B_MODE);
        } else if (0 == A_STRCMP(argv[2], "g")) {
            swat_wmiconfig_wifi_mode_set((A_UINT8)QCOM_11G_MODE);
        } else if (0 == A_STRCMP(argv[2], "n")) {
            swat_wmiconfig_wifi_mode_set((A_UINT8)QCOM_11N_MODE);
        } else if (0 == A_STRCMP(argv[2], "ht40")) {
            swat_wmiconfig_wifi_mode_set((A_UINT8)QCOM_HT40_MODE);
        } else {
            SWAT_PTF("Unknown wmode, only support b/g/n/ht40\n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--listen")) {
        if (3 == argc) {
            A_SSCANF(argv[2], "%d", &data);
            swat_wmiconfig_listen_time_set((A_UINT32) data);
        } else {
            SWAT_PTF("Invalid listen time or paramter error\n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--scanctrl")) {
        if (3 == argc || 4 == argc) {
            A_SSCANF(argv[2], "%d", &data);
            swat_wmiconfig_scan_ctl_set((A_UINT32) data);   //xiny
        } else {
            SWAT_PTF("Invalid paramter format\n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--setscanpara")) {
        if (4 == argc) {
			A_UINT16 max_dwell_time;
			A_UINT16 pass_dwell_time;
            A_SSCANF(argv[2], "%d", &data);
			max_dwell_time = (A_UINT16)data;
            A_SSCANF(argv[3], "%d", &data);
			pass_dwell_time = (A_UINT16)data;
            swat_wmiconfig_scan_para_set(max_dwell_time, pass_dwell_time);
        } else {
            SWAT_PTF("wmiconfig --setscanpara <max_dwell_time> <pass_dwell_time>\n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--ap")) {
        if (argc < 3) {
            SWAT_PTF("Missing parameter\n");
            return SWAT_ERROR;
        }

        if (!A_STRCMP(argv[2], "bconint")) {
            if (4 == argc) {
                A_SSCANF(argv[3], "%d", &data);
                swat_wmiconfig_bcon_int_set((A_UINT16) data);
            } else {
                SWAT_PTF("Invalid paramter format\n");
                return SWAT_ERROR;
            }

        }
        if (!A_STRCMP(argv[2], "country")) {
            if (4 == argc) {
                country_code[0] = argv[3][0];
                country_code[1] = argv[3][1];
                country_code[2] = 0x20;
                swat_wmiconfig_country_code_set(country_code);
            } else {
                SWAT_PTF("Invalid paramter format\n");
                return SWAT_ERROR;
            }
        }

		if (!A_STRCMP(argv[2], "inact")) {
			if (4 == argc) {
				A_SSCANF(argv[3], "%d", &data);
				swat_wmiconfig_inact_set((A_UINT16) data);
			} else {
				SWAT_PTF("Invalid paramter format\n");
				return SWAT_ERROR;
			}
	
		}

		
        return SWAT_OK;
    }

    return SWAT_NOFOUND;
}
#endif

#if defined(SWAT_WMICONFIG_WEP)

A_INT32
swat_wmiconfig_wep_handle(A_INT32 argc, A_CHAR * argv[])
{

    A_UINT32 key_index;

    if (!A_STRCMP(argv[1], "--wepkey")) {

        if (argc == 4) {
            A_SSCANF(argv[2], "%d", &key_index);

            if ((key_index > 4) || (key_index < 1)) {
                SWAT_PTF("key_index: 1-4 \n");
                return SWAT_ERROR;
            }

            swat_wmiconfig_wep_key_set(argv[3], (A_CHAR) key_index);
        } else {
            SWAT_PTF("	  --wepkey <key_index> <key>\n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--wep")) {

        if (argc == 4) {
            A_SSCANF(argv[2], "%d", &key_index);
            if ((key_index > 4) || (key_index < 1)) {
                SWAT_PTF("key_index: 1-4 \n");
                return SWAT_ERROR;
            }

            /* support open mode now*/
            swat_wmiconfig_wep_key_index_set(key_index,1);
        } else {
            SWAT_PTF("	  --wep <def_keyix> <mode>\n");
            return SWAT_ERROR;
        }
        return SWAT_OK;
    }

    return SWAT_NOFOUND;

}
#endif

#if defined(SWAT_WMICONFIG_WPA)

A_INT32
swat_wmiconfig_wpa_handle(A_INT32 argc, A_CHAR * argv[])
{
    if (!A_STRCMP(argv[1], "--p")) {
        if (3 == argc) {
            swat_wmiconfig_wep_passowrd_set(argv[2]);
        } else {
            SWAT_PTF("	  wmiconfig --p <passphrase>\n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--wpa")) {
        if (argc == 5) {
            //Do nothing
        } else {
            SWAT_PTF("	  --wpa <ver> <ucipher> <mcipher>\n");
            return SWAT_ERROR;
        }
        return SWAT_OK;
    }

    return SWAT_NOFOUND;
}
#endif

#if 1

#if defined(SWAT_WMICONFIG_WPS)

A_INT32
swat_wmiconfig_wps_handle(A_INT32 argc, A_CHAR * argv[])
{
    if (!A_STRCMP(argv[1], "--wps")) {
        A_UINT32 connect = 0;

        if (argc < 4) {
            SWAT_PTF
                ("wmiconfig --wps <connect:0-not connect after wps 1:connect after wps> <mode:pin/push> [pin:<=8 characters]\n");
            return SWAT_ERROR;
        }

        A_SSCANF(argv[2], "%d", &connect);
        if (!A_STRCMP(argv[3], "pin")) {
            if (argc < 5) {
                SWAT_PTF("Parameter number is too less.\n");
                return SWAT_ERROR;
            }

            if (strlen(argv[4]) > 8) {
                SWAT_PTF("pin length must be less than 8.\n");
                return SWAT_ERROR;
            }

            swat_wmiconfig_wps_enable(1);
            swat_wmiconfig_wps_start(connect, 0, (A_INT8 *)argv[4]);
        } else if (!A_STRCMP(argv[3], "push")) {
            swat_wmiconfig_wps_enable(1);
            swat_wmiconfig_wps_start(connect, 1, 0);
        } else {
            SWAT_PTF("Invalid wps mode.\n");
            return SWAT_ERROR;
        }

        return SWAT_OK;
    }

    return SWAT_NOFOUND;
}
#endif

#if defined(SWAT_WMICONFIG_P2P)
A_INT32
swat_wmiconfig_p2p_handle(A_INT32 argc, A_CHAR * argv[])
{

    if (!A_STRCMP(argv[1], "--p2p")) {
        /* swat_wmiconfig_p2p(argc - 1, &argv[1]); */
        return SWAT_OK;
    }

    return SWAT_NOFOUND;

}
#endif
#if defined(SWAT_WMICONFIG_IP)

A_INT32
swat_wmiconfig_ip_handle(A_INT32 argc, A_CHAR * argv[])
{
    A_INT32 data;

    if (!A_STRCMP(argv[1], "--ipstatic")) {
        if (argc < 5) {
            SWAT_PTF("--ipstatic x.x.x.x(ip) x.x.x.x(msk) x.x.x.x(gw)\n");
            return SWAT_ERROR;
        }

        swat_wmiconfig_ipstatic(argv[2], argv[3], argv[4]);

        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--ipdhcp")) {
        swat_wmiconfig_ipdhcp();
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--ipconfig")) {
        swat_wmiconfig_ipconfig();
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--ip_gethostbyname")) {
        if (argc < 3) {
            SWAT_PTF("--ip_gethostbyname <hostname>\n");
            return SWAT_ERROR;
        }
        swat_wmiconfig_dns(argv[2]);
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--ip_dns_client")) {
        if (argc < 3) {
            SWAT_PTF("--ip_dns_client start/stop\n");
            return SWAT_ERROR;
        }
        swat_wmiconfig_dns_enable(argv[2]);
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--ip_dns_server_addr")) {
        if (argc < 3) {
            SWAT_PTF("--ip_dns_server_addr <xx.xx.xx.xx>\n");
            return SWAT_ERROR;
        }
        swat_wmiconfig_dns_svr_add(argv[2]);
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--ip_dns_delete_server_addr")) {
        if (argc < 3) {
            SWAT_PTF("--ip_dns_delete_server_addr <xx.xx.xx.xx>\n");
            return SWAT_ERROR;
        }
        swat_wmiconfig_dns_svr_del(argv[2]);
        return SWAT_OK;
    }

    if (!A_STRCMP(argv[1], "--ipdhcppool")) {
        if (argc != 5) {
            SWAT_PTF("Missing parameter\n");
            return SWAT_ERROR;
        }

        /* data = A_ATOUL(argv[4]); */
        swat_wmiconfig_dhcp_pool(argv[2], argv[3], data);
        return SWAT_OK;
    }

    return SWAT_NOFOUND;
}
#endif


#if defined(SWAT_BENCH)

A_INT32
swat_benchtx_handle(A_INT32 argc, A_CHAR * argv[])
{
    A_INT32 ret = 0;
    A_UINT32 aidata[4];
    A_UINT32 ipAddress = 0;
    A_UINT32 port = 0;
    A_UINT32 pktSize = 0;
    A_UINT32 seconds = 0;
    A_UINT32 numOfPkts = 0;
    A_UINT32 mode = 0;
    A_UINT32 protocol = 0;
    A_UINT32 delay = 0;

    if (((void *) 0 == argv) || (argc < 8)) {
        SWAT_PTF("benchtx <Rx IP> <port> <protocol> <size> <test mode> <packets|time> <delay>\r\n");
        SWAT_PTF("Ex: benchtx 192.168.1.100 6000 tcp 64 1 1000 0\r\n");
        SWAT_PTF("Parameters:\r\n");
        SWAT_PTF("1 <Rx IP>       : IP address\r\n");
        SWAT_PTF("2 <port>        : Listening Port\r\n");
        SWAT_PTF("3 <protocol>    : tcp or udp\r\n");
        SWAT_PTF("4 <size>        : Packet size in bytes\r\n");
        SWAT_PTF("5 <test mode>   : 0:Time 1:Packets\r\n");
        SWAT_PTF("6 <packets|time>: Seconds or Packets number\r\n");
        SWAT_PTF("7 <delay>       : Delay ms\r\n");
        return -1;
    }

    /* IP Address */
    ret = swat_sscanf(argv[1], "%3d.%3d.%3d.%3d", &aidata[0], &aidata[1], &aidata[2], &aidata[3]);
    if (ret < 0) {
        SWAT_PTF("The ip is wrong, please input it again.\n");
        return -1;
    }
    ipAddress = (aidata[0] << 24) | (aidata[1] << 16) | (aidata[2] << 8) | aidata[3];

    /* Port */
    port = swat_atoi(argv[2]);

    /* TCP or UDP */
    if ((0 != swat_strcmp(argv[3], "tcp"))
        && (0 != swat_strcmp(argv[3], "udp"))) {
        SWAT_PTF("Wrong protocol\r\n");
        return -1;
    }
    ret = swat_strcmp(argv[3], "tcp");
    if (0 == ret) {
        protocol = 0;
    }
    ret = swat_strcmp(argv[3], "udp");
    if (0 == ret) {
        protocol = 1;
    }

    /* Packet Size */
    pktSize = swat_atoi(argv[4]);
    if ((0 == pktSize) || (pktSize > 1400)) {
        SWAT_PTF("Wrong packet size, the rang is [1, 1400]\r\n");
        return -1;
    }

    /* Test Mode */
    mode = swat_atoi(argv[5]);
    if ((0 != mode)
        && (1 != mode)) {
        SWAT_PTF("Wrong mode input 0 or 1\r\n");
        return -1;
    }
    if (0 == mode) {
        seconds = swat_atoi(argv[6]);
        if (0 == seconds) {
            SWAT_PTF("Wrong seconds\r\n");
            return -1;
        }
    }
    if (1 == mode) {
        numOfPkts = swat_atoi(argv[6]);
        if (0 == numOfPkts) {
            SWAT_PTF("Wrong packets number\r\n");
            return -1;
        }
    }

    /* Do nothing */
    delay = swat_atoi(argv[7]);
    swat_bench_tx_test(ipAddress, port, protocol, pktSize, mode, seconds, numOfPkts, delay);

    return 0;
}

A_INT32
swat_benchrx_handle(A_INT32 argc, A_CHAR * argv[])
{
    A_INT32 ret = 0;
    A_UINT32 aidata[4];
    A_UINT32 ipAddress = 0;
    A_UINT32 localIp = 0;
    A_UINT32 mcastIp = 0;
    A_UINT32 port = 0;
    A_UINT32 protocol = 0;

    if (((void *) 0 == argv) || (argc > 5) || (argc < 3)) {
        SWAT_PTF("benchrx <protocol> <port> <multicast IP*> <local IP*>\r\n");
        SWAT_PTF("Ex: benchrx tcp 6000 192.168.1.100 \r\n");
        SWAT_PTF("Parameters:\r\n");
        SWAT_PTF("1 <protocol>    : tcp or udp\r\n");
        SWAT_PTF("2 <port>        : Listening Port\r\n");
        SWAT_PTF("3 <multicast IP>: Multicast IP Address\r\n");
        SWAT_PTF("4 <local IP>    : Local IP Address\r\n");

        return -1;
    }

    /* TCP or UDP */
    if ((0 != swat_strcmp(argv[1], "tcp"))
        && (0 != swat_strcmp(argv[1], "udp"))) {
        SWAT_PTF("Wrong protocol\r\n");
        return -1;
    }
    ret = swat_strcmp(argv[1], "tcp");
    if (0 == ret) {
        protocol = 0;
    }
    ret = swat_strcmp(argv[1], "udp");
    if (0 == ret) {
        protocol = 1;
    }

    /* Port */
    port = swat_atoi(argv[2]);

    /* IP Check */
    if (4 == argc) {
        ret =
            swat_sscanf(argv[3], "%3d.%3d.%3d.%3d", &aidata[0], &aidata[1], &aidata[2], &aidata[3]);
        if (ret < 0) {
            SWAT_PTF("The ip is wrong, please input it again.\n");
            return -1;
        }
        ipAddress = (aidata[0] << 24) | (aidata[1] << 16) | (aidata[2] << 8) | aidata[3];

        /* Check Multicast IP Or Local IP */
        if ((aidata[0] & 0xf0) == 0xE0) //224.xxx.xxx.xxx - 239.xxx.xxx.xxx
        {
            mcastIp = ipAddress;
        } else {
            localIp = ipAddress;
        }
    }
    if (5 == argc) {
        ret =
            swat_sscanf(argv[3], "%3d.%3d.%3d.%3d", &aidata[0], &aidata[1], &aidata[2], &aidata[3]);
        if (ret < 0) {
            SWAT_PTF("The ip is wrong, please input it again.\n");
            return -1;
        }
        mcastIp = (aidata[0] << 24) | (aidata[1] << 16) | (aidata[2] << 8) | aidata[3];

        if ((aidata[0] & 0xf0) != 0xE0) {
            SWAT_PTF("The mcast ip is wrong, please input it again.\n");
            return -1;
        }

        ret =
            swat_sscanf(argv[4], "%3d.%3d.%3d.%3d", &aidata[0], &aidata[1], &aidata[2], &aidata[3]);
        if (ret < 0) {
            SWAT_PTF("The ip is wrong, please input it again.\n");
            return -1;
        }
        localIp = (aidata[0] << 24) | (aidata[1] << 16) | (aidata[2] << 8) | aidata[3];
    }

    swat_bench_rx_test(protocol, port, localIp, mcastIp);

    return 0;
}

A_INT32
swat_benchquit_handle(A_INT32 argc, A_CHAR * argv[])
{
    if (argc > 1) {
        SWAT_PTF("input benchquit\n");
        return -1;
    }

    swat_bench_quit_config();

#if 0
    qcom_msleep(1000);

    extern void qcom_task_kill_all(void);
    qcom_task_kill_all();

    extern void swat_socket_close_all(void);
    swat_socket_close_all();
#endif

    return 0;
}

A_INT32
swat_benchdbg_handle(A_INT32 argc, A_CHAR * argv[])
{

    /* swat_bench_dbg(); */
    return 0;
}

#endif

#if defined(SWAT_PING)
extern A_INT32 isdigit(A_INT32 c);
extern void qcom_ping_request(A_UINT32 host, A_UINT32 size, A_UINT32 count, A_UINT32 interval);

A_INT32
inet_aton(const A_CHAR *name,
          /* [IN] dotted decimal IP address */
          A_UINT32 * ipaddr_ptr
          /* [OUT] binary IP address */
    )
{                               /* Body */

    A_INT8 ipok = FALSE;

    A_UINT32 dots;

    A_UINT32 byte;

    A_UINT32 addr;

    addr = 0;

    dots = 0;

    for (;;) {

        if (!isdigit(*name))
            break;

        byte = 0;

        while (isdigit(*name)) {

            byte *= 10;

            byte += *name - '0';

            if (byte > 255)
                break;

            name++;

        }                       /* Endwhile */

        if (byte > 255)
            break;

        addr <<= 8;

        addr += byte;

        if (*name == '.') {

            dots++;

            if (dots > 3)
                break;

            name++;

            continue;

        }

        /* Endif */
        if ((*name == '\0') && (dots == 3)) {

            ipok = TRUE;

        }

        /* Endif */
        break;

    }                           /* Endfor */

    if (!ipok) {

        return 0;

    }

    /* Endif */
    if (ipaddr_ptr) {

        *ipaddr_ptr = addr;

    }

    /* Endif */
    return -1;

}                               /* Endbody */

A_INT32
swat_ping_handle(A_INT32 argc, A_CHAR *argv[])
{
    A_UINT32 hostaddr;
    A_INT32 error;
    A_UINT32 count, size, interval;
    A_UINT32 i;

    if (argc < 2) {
        SWAT_PTF("Usage: %s <host_ip> [ -c <count> -s <size> -i <interval> ]\n", argv[0]);

        return -1;
    }

    error = inet_aton(argv[1], &hostaddr);
    if (error != -1) {
        if (strlen(argv[1]) > 32) {
            SWAT_PTF("host name cannot be more then 32 Bytes\n");

            return -1;
        } else {
            if (A_OK != qcom_dnsc_get_host_by_name(argv[1], &hostaddr) ) {
                SWAT_PTF("Can't get IP addr by host name %s\n", argv[1]);
                return -1;
            } else {
                SWAT_PTF("Get IP addr %s by host name %s\n", (char *)_inet_ntoa(hostaddr), argv[1]);
            }
        }
    }

    count = 1;
    size = 64;
    interval = 0;

    if (argc > 2 || argc <= 8) {
        for (i = 2; i < argc; i += 2) {
            if (!A_STRCMP(argv[i], "-c")) {
                if ((i + 1) == argc) {
                    SWAT_PTF("Missing parameter\n");
                    return -1;
                }
                A_SSCANF(argv[i + 1], "%u", &count);
            } else if (!A_STRCMP(argv[i], "-s")) {
                if ((i + 1) == argc) {
                    SWAT_PTF("Missing parameter\n");
                    return -1;
                }
                A_SSCANF(argv[i + 1], "%u", &size);
            } else if (!A_STRCMP(argv[i], "-i")) {
                if ((i + 1) == argc) {
                    SWAT_PTF("Missing parameter\n");
                    return -1;
                }
                A_SSCANF(argv[i + 1], "%u", &interval);
            }
        }
    } else {
        SWAT_PTF("Usage: %s <host> [ -c <count> -s <size> -i <interval> ] \n", argv[0]);

        return -1;
    }

    if (size > 7000) {          /*CFG_PACKET_SIZE_MAX_TX */
        SWAT_PTF("Error: Invalid Parameter %s \n", argv[5]);

        //SWAT_PTF("Enter size <= %d \n",CFG_PACKET_SIZE_MAX_TX);

        return -1;
    }

    SWAT_PTF("Pinging %s with %u bytes of data, count %u, interval %u\n", argv[1], size, count,
             interval);

    qcom_ip_ping(hostaddr, size, count, interval);

    return 0;
}

#endif

#endif
void
swat_time()
{

   cmnos_printf("[%dh%dm%ds:]", (swat_time_get_ms() / 1000) / 3600,
                 ((swat_time_get_ms() / 1000) % 3600) / 60, (swat_time_get_ms() / 1000) % 60); 

}
