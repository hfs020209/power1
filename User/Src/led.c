#include "led.h"

/* --------------------------------------------------------------------------
 *  led״̬����
 * --------------------------------------------------------------------------
 */
volatile led_name_state g_led_name_state[led_max] = {
																								{
																									.led_name 	= run_led,
																									.led_state	= LED_OFF,																							
																									.old_time		= 0,
																									.time_out   = 0,
																									.led_port		= LED_RUN_GPIO_Port,
																									.led_pin		= LED_RUN_Pin,
																								},
																								{
																									.led_name 	= com_led,
																									.led_state	= LED_OFF,
																									.old_time		= 0,
																									.time_out   = 0,
																									.led_port		= LED_COM_GPIO_Port,
																									.led_pin		= LED_COM_Pin,
																								},
																								{
																									.led_name 	= can0_led,
																									.led_state	= LED_OFF,
																									.old_time		= 0,
																									.time_out   = 0,
																									.led_port		= LED_CAN1_GPIO_Port,
																									.led_pin		= LED_CAN1_Pin,
																								},
																								{
																									.led_name 	= can1_led,
																									.led_state	= LED_OFF,
																									.old_time		= 0,
																									.time_out   = 0,
																									.led_port		= LED_CAN2_GPIO_Port,
																									.led_pin		= LED_CAN2_Pin,
																								},
																								{
																									.led_name 	= lan_led,
																									.led_state	= LED_OFF,
																									.old_time		= 0,
																									.time_out   = 0,
																									.led_port		= LED_LAN_GPIO_Port,
																									.led_pin		= LED_LAN_Pin,
																								},
};

/********************************************************
  * ��������: ����LED��״̬
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: LED����״̬�����÷�ת��Ҫ����ʱ�䣬ʱ��Ϊÿ�α仯���ʱ��
********************************************************/
void rc_sys_led_state_set(uint8_t led_name, uint8_t led_state, uint32_t old_time, uint16_t timer_out)
{
	uint8_t i;
	
	for(i = 0; i<led_max; i++){
		
		if(g_led_name_state[i].led_name == led_name){
		
			g_led_name_state[i].led_state = led_state;
			g_led_name_state[i].old_time = old_time;
			g_led_name_state[i].time_out = timer_out;
			break;
		}
	}
}

/********************************************************
  * ��������: LED״̬����
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: ����LED״̬�����ӦLED_IO��ƽ
********************************************************/
void rc_sys_led_state_poll(void)
{
	uint8_t i = 0;
	
	for(i = 0; i<led_max; i++){
	
		if(g_led_name_state[i].led_state == LED_ON){
			
			rc_led_state(g_led_name_state[i].led_port, g_led_name_state[i].led_pin, LED_OFF);
		}
		else if(g_led_name_state[i].led_state == LED_OFF){
			
			rc_led_state(g_led_name_state[i].led_port, g_led_name_state[i].led_pin, LED_ON);
		}
		else if(g_led_name_state[i].led_state == LED_FLICKER){
			
			if(rc_sys_timeHasPast(g_led_name_state[i].old_time, g_led_name_state[i].time_out)){
			
				HAL_GPIO_TogglePin(g_led_name_state[i].led_port, g_led_name_state[i].led_pin);
				
				g_led_name_state[i].old_time = rc_sys_timeGet();  //���¸�ֵʱ��
			}
		}
	}
}




