/********************************************************************************
* @File name: biu.c
* @Author: fangqw
* @Version: 1.1
* @Date: 2021-3-19
* @Description: The function interface。
********************************************************************************/

#include "sys_app.h"
#include "lwip.h"
#include "udp_echoclient.h"

/* --------------------------------------------------------------------------
 *  can ID值
 * --------------------------------------------------------------------------
 */
volatile uint8_t g_carriage_num = 0; 

//#define TEST_TIME

/********************************************************
  * 函数功能: 获取系统时间节拍
  * 输入参数: void
  * 返 回 值: 时间节拍数  1ms间隔
  * 说    明: 无
********************************************************/

uint32_t rc_sys_timeGet(void)
{
	//滴答定时器 1ms间隔
	return HAL_GetTick();
}

/********************************************************
  * 函数功能: 判断时间是否超时 最小1ms
  * 输入参数: 超时开始时间Old_time  超时时间pass
  * 返 回 值: 1 超时 0 未超时
  * 说    明: 无
**********************************************************/
uint8_t rc_sys_timeHasPast(uint32_t Old_time, uint32_t pass)
{
	//判断是否超时
	return (rc_sys_timeGet() - (Old_time) > (pass));
}

/********************************************************
  * 函数功能: 硬件看门狗任务
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 初始化喂狗引脚后，硬件看门狗启用，并需要定时喂狗,
							喂狗时间min=0.9s,max=2.6s
********************************************************/
static void rc_sys_wdi_poll(void)
{
	static uint32_t old_timer = 0;
	static uint8_t init_gpio_flag = 0;
	
	if(init_gpio_flag == 0)
	{
		GPIO_InitTypeDef GPIO_InitStruct ={0};
			//看门狗GPIO初始化
		GPIO_InitStruct.Pin = WDI_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(WDI_PORT, &GPIO_InitStruct);
		init_gpio_flag	= 1;	
	}
	
	if(rc_sys_timeHasPast(old_timer, WDI_TIMEOUT)){
		
		old_timer = rc_sys_timeGet();
		rc_wdi_heard();
		
	}
}
/********************************************************
  * 函数功能: 系统任务状态初始化
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 无
********************************************************/
void rc_sys_app_init(void)
{
	g_carriage_num =  rc_coder_get();
	UserLog("coder = 0x%02X",g_carriage_num);
	rc_can_app_init(CAN1_NAME, 0x00,0x00000000);
	rc_can_app_init(CAN2_NAME, 0x00,0x00000000);
	rc_uart_app_init();
	rc_eeprom_app_init();
#ifdef CH395_EN
	rc_eth_ch395_app_init();
#else
	rc_eth_w5500_app_init();
#endif
	
	rc_sys_led_state_set(run_led,LED_FLICKER, rc_sys_timeGet(), LED_TIMEOUT);
	rc_sys_led_state_set(com_led,LED_FLICKER, rc_sys_timeGet(), LED_TIMEOUT);
	rc_sys_led_state_set(can0_led,LED_FLICKER, rc_sys_timeGet(), LED_TIMEOUT);
	rc_sys_led_state_set(can1_led,LED_FLICKER, rc_sys_timeGet(), LED_TIMEOUT);
	rc_sys_led_state_set(lan_led,LED_FLICKER, rc_sys_timeGet(), LED_TIMEOUT);
	UserLog("app init success");
}

/********************************************************
  * 函数功能: 系统任务调度
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 无
********************************************************/
void sys_app_poll(void)
{
#ifdef TEST_TIME
	static uint32_t timer1 = 0;
	static uint32_t timer2 = 0;
#endif
	rc_eeprom_app_poll();
	rc_sys_i2c_app_poll();
	rc_sys_adc_app_poll();
	rc_sys_led_state_poll();
	rc_sys_can_app_poll();
#ifdef CH395_EN
	rc_sys_ch395_app_poll();
#else
	rc_sys_w5500_app_poll();
#endif
	rc_sys_udp_app_poll();
	rc_sys_uart_app_poll();
	//rc_sys_wdi_poll();
#ifdef TEST_TIME
	timer2 = rc_sys_timeGet();
	printf("%d\r\n",timer2-timer1);
	timer1= timer2;
#endif
}
