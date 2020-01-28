/*
 * user_tcp_client.c
 *
 *  Created on: 2020年1月27日
 *      Author: Administrator
 */
#include "user_tcp_client.h"
#include "osapi.h"
#include "mem.h"
struct espconn conn;
#define DNS_server "2811g198e8.zicp.vip"
#define  PORT  8888

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
	os_printf("----------------new data----------------------\n");
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
	os_printf("----------------tcp connect success----------------------\n");
}

void ICACHE_FLASH_ATTR tcp_reconnect_callback(void* arg,sint8 err)
{
	os_printf("----------------tcp_reconnect_callback----------------------\n");
}

void ICACHE_FLASH_ATTR tcp_send_callback(void* arg)
{
	os_printf("----------------tcp_send_callback----------------------\n");
}

void ICACHE_FLASH_ATTR tcp_disconnect_callback(void* arg)
{
	os_printf("----------------tcp_disconnect_callback----------------------\n");
}
