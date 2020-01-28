/*
 * sta_client.c
 *
 *  Created on: 2020年1月27日
 *      Author: Administrator
 */
#include "sta_client.h"

bool ICACHE_FLASH_ATTR init_client(void)
{
		wifi_set_opmode(0x01);//设置为station模式
		char ssid[32]  = "4-3";
		char pwd[64] = "z15282826696";
		struct station_config stationConf;
		stationConf.bssid_set = 0;
		os_memcpy(&stationConf.ssid , ssid , 32);
		os_memcpy(&stationConf.password , pwd , 64);
		return wifi_station_set_config(&stationConf);
}


