/*
 * user_gpio.c
 *
 *  Created on: 2020年1月29日
 *      Author: Administrator
 */
#include "user_gpio.h"

void ICACHE_FLASH_ATTR init_gpio(void)
{
	//设置为GPIO14 D5
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U,FUNC_GPIO14);
	//设置输出模式
	GPIO_OUTPUT_SET(GPIO_ID_PIN(14),1);

}


