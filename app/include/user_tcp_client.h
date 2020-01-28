/*
 * user_tcp_client.h
 *
 *  Created on: 2020Äê1ÔÂ27ÈÕ
 *      Author: Administrator
 */

#ifndef APP_INCLUDE_USER_TCP_CLIENT_H_
#define APP_INCLUDE_USER_TCP_CLIENT_H_
#include "user_interface.h"
#include "c_types.h"
#include "espconn.h"
bool ICACHE_FLASH_ATTR  init_tcp_client();

void ICACHE_FLASH_ATTR tcp_recv_callback(void* arg,char* pdata,unsigned short len);
//void ICACHE_FLASH_ATTR tcp_callback(void* arg,char* pdata,unsigned short len);
void ICACHE_FLASH_ATTR tcp_connect_callback(void* arg);
void ICACHE_FLASH_ATTR tcp_reconnect_callback(void* arg,sint8 err);
void ICACHE_FLASH_ATTR tcp_send_callback(void* arg);
void ICACHE_FLASH_ATTR tcp_disconnect_callback(void* arg);


#endif /* APP_INCLUDE_USER_TCP_CLIENT_H_ */
