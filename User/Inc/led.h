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
  * ��������: ����LED��״̬
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: LED����״̬�����÷�ת��Ҫ����ʱ�䣬ʱ��Ϊÿ�α仯���ʱ��
********************************************************/
void rc_sys_led_state_set(uint8_t led_name, uint8_t led_state, uint32_t old_time, uint16_t timer_out);

/********************************************************
  * ��������: LED״̬����
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: ����LED״̬�����ӦLED_IO��ƽ
********************************************************/
void rc_sys_led_state_poll(void);

#endif
