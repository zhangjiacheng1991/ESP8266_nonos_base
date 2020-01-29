/*
 * user_timer.c
 *
 *  Created on: 2020年1月27日
 *      Author: Administrator
 */

#include "user_timer.h"
#include "sta_client.h"
#include "user_smart_config.h"
#include "user_tcp_client.h"
//定时器
os_timer_t timer_connect_wifi;

//定时器回调函数
void ICACHE_FLASH_ATTR user_timer_callback(void)
{
	u8 S_WIFI_STA_Connect;
	struct ip_info ST_ESP8266_IP;
	u8 esp_ip[4];
	//查询WiFi状态
	S_WIFI_STA_Connect = wifi_station_get_connect_status();
	if(S_WIFI_STA_Connect == STATION_GOT_IP)
	{
		wifi_get_ip_info(STATION_IF,&ST_ESP8266_IP);
		esp_ip[0] = ST_ESP8266_IP.ip.addr;
		esp_ip[1] = ST_ESP8266_IP.ip.addr >> 8;
		esp_ip[2] = ST_ESP8266_IP.ip.addr >> 16;
		esp_ip[3] = ST_ESP8266_IP.ip.addr >> 24;
		os_printf("esp_ip = %d.%d.%d.%d\n",esp_ip[0],esp_ip[1],esp_ip[2],esp_ip[3]);
		os_timer_disarm(&timer_connect_wifi);//关闭定时器
		init_tcp_client();
	}
	else if(
				S_WIFI_STA_Connect ==   STATION_WRONG_PASSWORD ||
				S_WIFI_STA_Connect ==   STATION_NO_AP_FOUND ||
				S_WIFI_STA_Connect ==   STATION_CONNECT_FAIL 	)
	{
		os_timer_disarm(&timer_connect_wifi);//关闭定时器
		os_printf("\r\n -------------------------can't connect to WIFI-------------------\r\n");
		init_samrt_config();
		init_tcp_client();
	}
}

//设置定时器
void ICACHE_FLASH_ATTR create_timer(os_timer_t* t , os_timer_func_t* pfunc,uint32_t ms,void* parg)
{
	os_timer_disarm(t);
	os_timer_setfn(t,pfunc,parg);
	os_timer_arm(t,ms,1);
}
void ICACHE_FLASH_ATTR init_timer(void)
{
	create_timer(&timer_connect_wifi , (os_timer_func_t*)user_timer_callback , 1000 , NULL);
}

