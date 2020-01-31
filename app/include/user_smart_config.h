/*
 * user_smart_config.h
 *
 *  Created on: 2020Äê1ÔÂ27ÈÕ
 *      Author: Administrator
 */

#ifndef APP_INCLUDE_USER_SMART_CONFIG_H_
#define APP_INCLUDE_USER_SMART_CONFIG_H_
#include "smartconfig.h"
#include "user_config.h"
void ICACHE_FLASH_ATTR  init_samrt_config(void);
void ICACHE_FLASH_ATTR smart_config_done(sc_status status, void * pdata);
#endif /* APP_INCLUDE_USER_SMART_CONFIG_H_ */
