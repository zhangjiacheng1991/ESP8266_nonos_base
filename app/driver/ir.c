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
u8 ir_read_ready =  0;
bool ir_read_on = false;
u8 ir_read_index = 0 ;
uint32 Each_bit_duration[35];

uint32 last_time = 0;
void ICACHE_FLASH_ATTR ir_init()
{
//	struct pwm_param pwm_config;
//	uint32 pin_info_list[][3]= {PERIPHS_IO_MUX_MTMS_U,FUNC_GPIO14,14};
//	pwm_config.duty[0] = 0;
//	pwm_config.freq = 38000;
//	pwm_config.period = 38000;
//	pwm_init(pwm_config.period , pwm_config.duty , 1 , pin_info_list);
	ir_signal_output(0);
}

void ICACHE_FLASH_ATTR ir_send_msg(u16 user_code, u8 data)
{
	os_printf("send begin time : %d",system_get_time);
	u16 i;
	u8 user_code_hight = user_code >> 8;
	u8 user_code_low = user_code;
	//引导码
	ir_send_head();
	//发送用户码低八位数据
	ir_send(user_code_low);
	//发送用户码高八位数据
	ir_send(user_code_hight);
	//发送八位数据
	ir_send(data);
	//发送八位数据反码
	ir_send(~user_code_hight);
	ir_send_0();
	os_printf("send end time : %d",system_get_time);
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
	u8 i;
	for(i=0;i<22;i++)
	{
		ir_signal_output(1);
		ir_8_77us();
		ir_signal_output(0);
		ir_17_53us();
		system_soft_wdt_feed();
	}

	for(i=0;i<21;i++)
	{
		ir_signal_output(0);
		ir_8_77us();
		ir_signal_output(0);
		ir_17_53us();
		system_soft_wdt_feed();
	}

}
void ICACHE_FLASH_ATTR ir_send_1()
{
	u8 i;
	for(i=0;i<22;i++)
	{
		ir_signal_output(1);
		ir_8_77us();
		ir_signal_output(0);
		ir_17_53us();
		system_soft_wdt_feed();
	}
	for(i=0;i<64;i++)
	{
		ir_signal_output(0);
		ir_8_77us();
		ir_signal_output(0);
		ir_17_53us();
		system_soft_wdt_feed();
	}
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
	IR_READ_REPIT:
	//	os_printf("ir_read begin\n");
	ir_read_on = true;
	ir_read_ready = 1;
	ir_read_index =0;
	last_time = 0;
	u8 status =1;
	ETS_GPIO_INTR_DISABLE();
	ETS_GPIO_INTR_ATTACH(ir_read_break_callback,NULL);
	gpio_pin_intr_state_set(GPIO_ID_PIN(5),GPIO_PIN_INTR_NEGEDGE);
	ETS_GPIO_INTR_ENABLE();
	memset(&Each_bit_duration,0,sizeof(u32));
	while(ir_read_on)
	{
		system_soft_wdt_feed();
		os_delay_us(5000);
		if(ir_read_ready == 0)
		{
			uint32 arr[34]={0};
			int i=0;
			for(i;i< 34; i++)
			{
				arr[i] = Each_bit_duration[i+1] - Each_bit_duration[i];
			}
			if(arr[0] < 8900)
			{
				os_printf("time1 < 8900\n");
				break;
			}
			ir_read_user_code(user_code);
			u8 ok =  ir_read_data(data);
			if(0 != ok )
			{
				memset(user_code, 0,sizeof(u16));
				memset(data,0,sizeof(u8));
				os_printf("read data err  code : %d\n",ok);
				break;
			}
			memset(&Each_bit_duration,0,34);
			status = 0;
			os_printf("read data ok\n");
			break;
		}
		else if( ir_read_ready == IR_READ_ERR)
		{
			os_printf("ir read err...\n");
		}
		else if(last_time != 0)
		{
			u32 curtime = system_get_time();
			if(curtime - last_time > 3000000)
			{
				status = 1;
				os_printf("ir read err please reset...\n");
				break;
			}
		}
	}
	return status;
}

void  ir_read_break_callback()
{
	u32 S_GPIO_INT = 0 ;
	S_GPIO_INT = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS,S_GPIO_INT);
	if(ir_read_index >= 34)
	{
		ir_read_ready = 0;
		ETS_GPIO_INTR_DISABLE();
		return;
	}
	u32 cur_time = system_get_time();
	if(ir_read_index == 0 )
	{

	}
	else  if( cur_time - last_time > 3000000)
	{
		os_printf("cur_time - last_time > 3000000 index : %d  cur : %d last : %d c-l : %d\n",ir_read_index, cur_time  ,last_time,cur_time-last_time);
		ETS_GPIO_INTR_DISABLE();
		ir_read_ready = IR_READ_ERR;
		return;
	}
	else if (cur_time - last_time  < 700)
	{
		//		 os_printf("cur_time - last_time  < 700 index : %d cur : %d  last : %d c-l : %d\n",ir_read_index, cur_time  ,last_time,cur_time-last_time);
		return;
	}
	Each_bit_duration[ir_read_index] = cur_time;
	ir_read_index++;
	last_time = cur_time;
}

u8 ICACHE_FLASH_ATTR ir_read_user_code(u16* user_code)
{
	u16 code = 0;
	u8 i = 0 +1;
	for(i ; i < 16 +1; i ++)
	{
		uint32  time =	Each_bit_duration[ i +1] - Each_bit_duration[ i ];
		//		os_printf("USER CODE %d : %d\n",i,time);
		u8 bit = ir_read_bit(time);
		if( IR_READ_ERR != bit)
		{
			code |= bit;
			code <<= 1;
		}
		else
		{
			os_printf(" ir_read_bit(time) err");
			return 1;
		}
	}
	*user_code = code;
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
		uint32  time =	Each_bit_duration[ i +1] - Each_bit_duration[ i ];
		u8 bit = ir_read_bit(time);
		//		os_printf("USER CODE %d : %d\n",i,bit);
		if( IR_READ_ERR != bit)
		{
			data1 |= bit;
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
		uint32  time =	Each_bit_duration[ i+1 ] - Each_bit_duration[ i ];
		u8 bit = ir_read_bit(time);
		//		os_printf("USER CODE %d : %d\n",i,bit);
		if( IR_READ_ERR != bit)
		{
			data2 |= bit;
			data2 <<= 1;
		}
		else
		{
			return 1;
		}
	}
	if(data1 & data2 != 0)
	{

		return 1;
	}
	*data = data1;
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
	else return IR_READ_ERR;
}

void ICACHE_FLASH_ATTR ir_send_head()
{
	u8 i;
	for(i=0;i<342;i++)
	{
		ir_signal_output(1);
		ir_8_77us();
		ir_signal_output(0);
		ir_17_53us();
		system_soft_wdt_feed();
	}
	for(i=0;i<171;i++)
	{
		ir_signal_output(0);
		ir_8_77us();
		ir_signal_output(0);
		ir_17_53us();
		system_soft_wdt_feed();
	}
}
