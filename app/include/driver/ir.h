/*
 * ir.h
 *
 *  Created on: 2020年1月30日
 *      Author: Administrator
 */

#ifndef APP_INCLUDE_DRIVER_IR_H_
#define APP_INCLUDE_DRIVER_IR_H_
#include "pwm.h"

#define ir_8_77us() os_delay_us(9)
#define ir_17_53us() os_delay_us(17)
#define ir_fre_divide 71
#define ir_reload_num 19999
#define IR_READ_ERR 256

void ICACHE_FLASH_ATTR ir_init();
void ICACHE_FLASH_ATTR ir_send_msg(u16 user_code, u8 data);
void ICACHE_FLASH_ATTR ir_send(u8 ir_data);
void ICACHE_FLASH_ATTR ir_send_0();
void ICACHE_FLASH_ATTR ir_send_1();
void ICACHE_FLASH_ATTR ir_send_head();
void ICACHE_FLASH_ATTR ir_signal_output(u8 Value_Vol);	// 输出参数电平
void ICACHE_FLASH_ATTR ir_signal_input(void);			// 设为输入

void  ir_read_break_callback();
u8 ICACHE_FLASH_ATTR ir_read(u16* user_code, u8* data);
u8 ICACHE_FLASH_ATTR ir_read_user_code(u16* user_code);
u8 ICACHE_FLASH_ATTR ir_read_data(u8* data);
u8 ICACHE_FLASH_ATTR ir_read_bit(u32 time);
extern bool ir_read_ready;
extern bool ir_read_on;
extern u8 ir_read_index;
extern uint32 Each_bit_duration[35];

#endif /* APP_INCLUDE_DRIVER_IR_H_ */
