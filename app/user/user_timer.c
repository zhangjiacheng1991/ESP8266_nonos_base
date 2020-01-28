/*
 * user_timer.c
 *
 *  Created on: 2020��1��27��
 *      Author: Administrator
 */
#include "user_timer.h"
//��ʱ��
os_timer_t timer_1;

//��ʱ���ص�����
void ICACHE_FLASH_ATTR user_timer_callback(void)
{
	os_printf("time out");
}

//���ö�ʱ��
void ICACHE_FLASH_ATTR create_timer(os_timer_t* t , os_timer_func_t* pfunc,uint32_t ms,void* parg)
{
	os_timer_disarm(t);
	os_timer_setfn(t,pfunc,parg);
	os_timer_arm(t,ms,1);
}
void ICACHE_FLASH_ATTR init_timer(void)
{
	create_timer(&timer_1 , (os_timer_func_t*)user_timer_callback , 500 , NULL);
}

