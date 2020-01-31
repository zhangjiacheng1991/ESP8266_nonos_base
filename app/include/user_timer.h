#ifndef USER_TIMER_H_
#define USER_TIMER_H_
#include "user_config.h"

extern os_timer_t timer_connect_wifi;
extern os_timer_t timer_get_temperature;
os_timer_t timer_get_temperature;
void  ICACHE_FLASH_ATTR  user_timer_check_net(void);
void  ICACHE_FLASH_ATTR  user_timer_get_temperature(void);
void ICACHE_FLASH_ATTR create_timer(os_timer_t* t , os_timer_func_t* pfunc,uint32_t ms,void* parg,bool status);
void ICACHE_FLASH_ATTR init_timer();



#endif
