/*
 * user_smart_config.c
 *
 *  Created on: 2020Äê1ÔÂ27ÈÕ
 *      Author: Administrator
 */
#include "user_smart_config.h"
#include "string.h"
#include "osapi.h"
void ICACHE_FLASH_ATTR smart_config_done(sc_status status, void * pdata)
{
	switch(status)
	{
	case SC_STATUS_WAIT:
		os_printf("SC_STATUS_WAIT\n");
		break;
	case SC_STATUS_FIND_CHANNEL:
		os_printf("SC_STATUS_FIND_CHANNEL\n");
		break;
	case SC_STATUS_GETTING_SSID_PSWD:
		os_printf("SC_STATUS_GETTING_SSID_PSWD\n");
		sc_type* type = pdata;
		if(*type == SC_TYPE_ESPTOUCH)
		{
			os_printf("SC_TYPE_ESPTOUCH\n");
		}
		else
		{
			os_printf("SC_TYPE__AIRKISS\n");
		}
		break;
	case SC_STATUS_LINK:
		os_printf("SC_STATUS_LINK\n");
		struct station_config* sta_conf = pdata;
		wifi_station_set_config(sta_conf);
		wifi_station_disconnect();
		wifi_station_connect();
		break;
	case SC_STATUS_LINK_OVER:
		os_printf("SC_STATUS_LINK_OVER\n");
		if(pdata != NULL)
		{
			uint8 phone_ip[4] = {0};
			memcpy(phone_ip,(uint8*)pdata,4);
			os_printf("Phone_ip : %d.%d.%d.%d" , phone_ip[0]  , phone_ip[1] , phone_ip[2], phone_ip[3]);
		}
		smartconfig_stop();
		break;
	}
}
void ICACHE_FLASH_ATTR init_samrt_config(void)
{
	smartconfig_set_type(SC_TYPE_AIRKISS); //SC_TYPE_ESPTOUCH,SC_TYPE_AIRKISS,SC_TYPE_ESPTOUCH_AIRKISS
	wifi_set_opmode(STATION_MODE);
	smartconfig_start(smart_config_done);
}

