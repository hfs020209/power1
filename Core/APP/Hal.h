#ifndef __HAL_H__
#define __HAL_H__

#include "usart.h"
#include "adc.h"
#include "dma.h"
#include "gpio.h"
#include "can.h"
#include "spi.h"

#define LED_STATUS_ON 1
#define LED_STATUS_OFF 0

#define ADC_MSG_MAX 6
#define ADC_SAMP 32

typedef enum {
    LED_C0,
    LED_C1,
    LED_C2,
    LEC_MAX
}gpio_leds;  

typedef struct {
    uint16_t idx;
    uint16_t pin;
    GPIO_TypeDef *port;
    uint8_t status;
}gpio_port_info;

typedef enum {
    ADC_CH_EI_1 = 0,
    ADC_CH_EV_1,
    ADC_CH_T_1,
    ADC_CH_T_2,
    ADC_CH_EI_2,
    ADC_CH_EV_2,
    ADC_CH_MAX
}adc_channel; 

typedef struct{
    uint32_t type;
    float adc[ADC_MSG_MAX];
    uint32_t adcFilterMax[ADC_MSG_MAX];
    uint32_t adcFilterMin[ADC_MSG_MAX];
}queue_adcInfo;    

#define _GPIO_NUM(x) (sizeof(x)/(sizeof(gpio_port_info)))

static void gpio_init(void);
void hal_init(void);
void handle_led_action(uint8_t index, uint8_t cmd);
void DMA_ADC_IRQ_SET(void);
void ADC_EN(void);
void getADC_Conv(void *dis);
float adcToEV(float adc);
//void MX_SPI1_Init(void);



#endif
























