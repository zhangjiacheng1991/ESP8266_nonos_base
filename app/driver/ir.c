/*
 * ir.c
 *
 *  Created on: 2020年1月30日
 *      Author: Administrator
 */
#include "user_config.h"
#include "driver/ir.h"
#include "pwm.h"
#include "ets_sys.h"

os_timer_t ir_read_timer;;

void ICACHE_FLASH_ATTR ir_init()
{
	uint32 duty=1;
	uint32 pin_info_list[3]= {PERIPHS_IO_MUX_MTMS_U,FUNC_GPIO14,GPIO_ID_PIN(14)};
	pwm_init(38000,&duty,50,&pin_info_list);
	memset(&Each_bit_duration,0,34);
	ETS_GPIO_INTR_DISABLE();
	ETS_GPIO_INTR_ATTACH(ir_read_break_callback,NULL);
	gpio_pin_intr_state_set(GPIO_ID_PIN(5),GPIO_PIN_INTR_NEGEDGE);
	ETS_GPIO_INTR_ENABLE();
}

void ICACHE_FLASH_ATTR ir_send_msg(u16 user_code, u8 data)
{
	u16 i;
	u8 user_code_hight = user_code >> 8;
	u8 user_code_low = user_code;
	ir_signal_output(0);
	//发送引导码
	ir_signal_output(1);
	os_delay_us(9000);//9ms载波
	ir_signal_output(0);
	os_delay_us(4500);//4.5ms空闲
	//发送用户码低八位数据
	ir_send(user_code_low);
	//发送用户码高八位数据
	ir_send(user_code_hight);
	//发送八位数据
	ir_send(data);
	//发送八位数据反码
	ir_send(user_code_hight);
	ir_send_0();
}
void ICACHE_FLASH_ATTR ir_send(u8 ir_data)
{
	u8 i;
	for(i =0; i < 8; i ++)
	{
		if((ir_data & 0x01))
		{
			ir_send_1();
		}
		else
		{
			ir_send_0();
		}
		ir_data >>= 1;
	}
}

void ICACHE_FLASH_ATTR ir_send_0()
{
	ir_signal_output(1);
	os_delay_us(560);//9ms载波
	ir_signal_output(0);
	os_delay_us(560);//4.5ms空闲
}
void ICACHE_FLASH_ATTR ir_send_1()
{
	ir_signal_output(1);
	os_delay_us(560);//9ms载波
	ir_signal_output(0);
	os_delay_us(1680);//4.5ms空闲
}

void ICACHE_FLASH_ATTR ir_signal_output(u8 Value_Vol)
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U,	FUNC_GPIO14);	// GPIO14设为IO口
	GPIO_OUTPUT_SET(GPIO_ID_PIN(14),Value_Vol);				// IO14设为输出=X
}
void ICACHE_FLASH_ATTR ir_signal_input(void)
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U,	FUNC_GPIO14);// GPIO14设为IO口
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(14));		// GPIO14失能输出(输入)
}


u8 ICACHE_FLASH_ATTR ir_read(u16* user_code, u8* data)
{
	if(ir_read_ready)
	{
		uint32  time1 =	Each_bit_duration[1] - Each_bit_duration[0];
		if(time1 < 8900)
		{
			return 1;
		}
		time1 = Each_bit_duration[34] - Each_bit_duration[33];
		if(ir_read_bit(time1)  !=0)
		{
			return 1;
		}
		ir_read_user_code(user_code);
		u8 ok =  ir_read_data(data);
		if(0 != ok )
		{
			memset(user_code, 0,sizeof(u16));
			memset(data,0,sizeof(u8));
			os_printf("read data err  code : %d",ok);
		}
		memset(&Each_bit_duration,0,34);
		ir_read_ready = false;
		ir_read_index =0;
		return 0;
	}
	else
	{
//		INFO("获取 红外 失败");
		return 1;
	}
}

void  ir_read_break_callback()
{
	if(ir_read_index >= 34)
	{
		ir_read_ready = true;
		return;
	}
	Each_bit_duration[ir_read_index] = system_get_time()/1000;
	if(Each_bit_duration[ir_read_index] - Each_bit_duration[0] >30000)
	{
		return;
	}
		ir_read_index++;
}

u8 ICACHE_FLASH_ATTR ir_read_user_code(u16* user_code)
{
	memset(user_code,0,sizeof(u16));
	u8 i = 0 +1;
	for(i ; i < 16 +1; i ++)
	{
		uint32  time =	Each_bit_duration[ i +1] - Each_bit_duration[ i ];
		u8 bit = ir_read_bit(time);
		if( IR_READ_BIT_ERR != bit)
		{
			(*user_code) &= bit;
			(*user_code) <<= 1;
		}
		else
		{
			return 1;
		}
	}
	return 0;
}
u8 ICACHE_FLASH_ATTR ir_read_data(u8* data)
{
	u8 data1= 0;
	u8 data2 = 0;
	u8 i = 0;
	i=1+16;
	for(i; i < 1+16+8; i ++)
	{
		uint32  time =	Each_bit_duration[ i ] - Each_bit_duration[ i+1 ];
		u8 bit = ir_read_bit(time);
		if( IR_READ_BIT_ERR != bit)
		{
			data1 &= bit;
			data1 <<= 1;
		}
		else
		{
			return 1;
		}
	}
	i = 1+16+8 ;
	for(i; i < 1+16+8 +8; i ++)
	{
		uint32  time =	Each_bit_duration[ i ] - Each_bit_duration[ i+1 ];
		u8 bit = ir_read_bit(time);
		if( IR_READ_BIT_ERR != bit)
		{
			data2 &= bit;
			data2 <<= 1;
		}
		else
		{
			return 1;
		}
	}
	if(data1 == ~data2)
	{
		*data = data1;
	}
	else
	{
		return 1;
	}
	return 0;
}

u8 ICACHE_FLASH_ATTR ir_read_bit(u32 time)
{
	if(time >2000  &&  time < 2500)
	{
		return 1;
	}
	else if(time >1000  &&  time < 1250)
	{
		return 0;
	}
	else return IR_READ_BIT_ERR;
}
