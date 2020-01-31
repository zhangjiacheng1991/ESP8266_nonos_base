#ifndef USER_TIMER_H_
#define USER_TIMER_H_
#include "user_config.h"

void  ICACHE_FLASH_ATTR  user_timer_check_net(void);
void  ICACHE_FLASH_ATTR  user_timer_get_temperature(void);
void ICACHE_FLASH_ATTR create_timer(os_timer_t* t , os_timer_func_t* pfunc,uint32_t ms,void* parg);
void ICACHE_FLASH_ATTR init_timer();



#endif
