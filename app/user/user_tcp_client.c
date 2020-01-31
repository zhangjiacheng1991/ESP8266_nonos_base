/*
 * user_tcp_client.c
 *
 *  Created on: 2020年1月27日
 *      Author: Administrator
 */
#include "user_tcp_client.h"
#include "user_gpio.h"
#include "driver/ir.h"
#include "user_timer.h"
struct espconn conn;
#define DNS_server "2811g198e8.zicp.vip"
#define  PORT  8888

os_timer_t timer_ir_read;

bool ICACHE_FLASH_ATTR  init_tcp_client()
{
	conn.type = ESPCONN_TCP;
	conn.proto.tcp = (esp_tcp*)os_zalloc(sizeof(esp_tcp));
	conn.proto.tcp->local_port = 3456 ;
	conn.proto.tcp->remote_port = PORT;
	conn.proto.tcp->remote_ip[0] = 192;
	conn.proto.tcp->remote_ip[1] = 168;
	conn.proto.tcp->remote_ip[2] = 3;
	conn.proto.tcp->remote_ip[3] = 188;
	//注册回调函数
	espconn_regist_connectcb(&conn , tcp_connect_callback);
	espconn_regist_reconcb(&conn,tcp_reconnect_callback );
	//连接tcpServer
	espconn_connect(&conn);
}

void ICACHE_FLASH_ATTR tcp_recv_callback(void* arg,char* pdata,unsigned short len)
{
	char str[64];
	memset(&str,0,32);
	if(0 == strcmp(pdata,"1"))
	{
		GPIO_OUTPUT_SET(GPIO_ID_PIN(4),1);
		strcpy(str,"gpio4 1\n");
	}
	else if (0 == strcmp(pdata,"2"))
	{
		GPIO_OUTPUT_SET(GPIO_ID_PIN(4),0);
	}
	else if  (0 == strcmp(pdata,"3"))
	{
		strcpy(str,"准备学习红外\n");
		espconn_send((struct espconn*)arg , str , os_strlen(str));
		create_timer(&timer_ir_read , (os_timer_func_t*)ir_read_callback , 500 , NULL,0);
	}
	else if  (0 == strcmp(pdata,"4"))
	{
		GPIO_OUTPUT_SET(GPIO_ID_PIN(14),1);
		strcpy(str,"gpio14 1\n");
	}
	else if  (0 == strcmp(pdata,"5"))
	{
		strcpy(str,"gpio5  5\n");
	}
	else if  (0 == strcmp(pdata,"6"))
	{
		strcpy(str,"gpio5  6\n");
	}
	else
	{
		strcpy(str,"others\n");
	}
	espconn_send((struct espconn*)arg , str , os_strlen(str));
}
//void ICACHE_FLASH_ATTR tcp_callback(void* arg,char* pdata,unsigned short len)
//{
//	os_printf("----------------new data----------------------\n");
//}
void ICACHE_FLASH_ATTR tcp_connect_callback(void* arg)
{
	espconn_regist_sentcb(&conn,tcp_send_callback );
	espconn_regist_recvcb(&conn,tcp_recv_callback );
	espconn_regist_disconcb(&conn,tcp_disconnect_callback );
	char * str = "connect success";
	espconn_send((struct espconn*)arg , str , os_strlen(str));
	//	os_printf("----------------tcp connect success----------------------\n");
}

void ICACHE_FLASH_ATTR tcp_reconnect_callback(void* arg,sint8 err)
{
	//	os_printf("----------------tcp_reconnect_callback----------------------\n");
}

void ICACHE_FLASH_ATTR tcp_send_callback(void* arg)
{
	//	os_printf("----------------tcp_send_callback----------------------\n");
}

void ICACHE_FLASH_ATTR tcp_disconnect_callback(void* arg)
{
	//	os_printf("----------------tcp_disconnect_callback----------------------\n");
}

void ICACHE_FLASH_ATTR  ir_read_callback()
{
			u16 user_code = 0;
			u8 data = 0;
			os_printf("开始读取红外数据");
			u8 result = ir_read(&user_code, &data);
			os_printf("读取红外数据完成");
			if(0 == result)
			{
				os_printf("user code : %U\r data : %U\n",user_code,data);
			}
			else
			{
				os_printf("get ir data err  %d\n",result);
			}
}
