#ifndef __LED_H__
#define __LED_H__



#include "sys_app.h"


/* Exported macro ------------------------------------------------------------*/

#define LED_ON 		 		 1
#define LED_OFF 				 0
#define LED_FLICKER 		 2

#define LED_TIMEOUT 500


/* Exported constants --------------------------------------------------------*/

enum led_name{
	
	run_led = 0,
	com_led,
	can0_led,
	can1_led,
	lan_led,
	led_max,
};

/* Exported types ------------------------------------------------------------*/

typedef struct led_state{
	
	uint8_t        led_name;
	uint8_t 			 led_state;
	uint32_t       old_time;
	uint16_t       time_out;
	GPIO_TypeDef * led_port;
	uint16_t 			 led_pin;
}led_name_state;

/* Exported functions --------------------------------------------------------*/

/********************************************************
  * 函数功能: 设置LED的状态
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: LED三种状态，设置翻转需要设置时间，时间为每次变化间隔时间
********************************************************/
void rc_sys_led_state_set(uint8_t led_name, uint8_t led_state, uint32_t old_time, uint16_t timer_out);

/********************************************************
  * 函数功能: LED状态任务
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 根据LED状态输出对应LED_IO电平
********************************************************/
void rc_sys_led_state_poll(void);

#endif
