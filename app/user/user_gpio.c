/*
 * user_gpio.c
 *
 *  Created on: 2020��1��29��
 *      Author: Administrator
 */
#include "user_gpio.h"

void ICACHE_FLASH_ATTR init_gpio(void)
{
	//����ΪGPIO14 D5
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U,FUNC_GPIO14);
	//�������ģʽ
	GPIO_OUTPUT_SET(GPIO_ID_PIN(14),1);

}


