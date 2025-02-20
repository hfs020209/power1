#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "main.h"
#include "cmsis_os.h"
#include "common.h"
#include "Hal.h"
#include "app.h"
#include "rc_can.h"



static gpio_port_info gpioleds[] = {
    {LED_C0, GPIO_PIN_0, GPIOC, 0},
    {LED_C1, GPIO_PIN_1, GPIOC, 0},
    {LED_C2, GPIO_PIN_2, GPIOC, 0},
};  

//static const float Rp = 50000.0; //10K          ADC部分参数     
//static const float T2 = (273.15+25.0);;//T2     //ADC计算电压电流所用参数
//static const float Bx = 3950.0;//B
//static const float Ka = 273.15;
uint16_t adcBuf[ADC_MSG_MAX];
static queue_adcInfo adcSamp[2];
static uint32_t adcSampIdx;
static uint32_t adcSampConvCpIdx;


static void gpio_init(void)
{
    int idx;
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    for(idx=0; idx<_GPIO_NUM(gpio_leds); idx++){

        HAL_GPIO_WritePin(gpioleds[idx].port,
                gpioleds[idx].pin, GPIO_PIN_RESET);
        }

    for(idx=0; idx<_GPIO_NUM(gpioleds); idx++){
        
        GPIO_InitStruct.Pin = gpioleds[idx].pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(gpioleds[idx].port, &GPIO_InitStruct);
        }

    for(idx=0; idx<_GPIO_NUM(gpioleds); idx++){

        HAL_GPIO_WritePin(gpioleds[idx].port, gpioleds[idx].pin, LED_STATUS_OFF);
        }

   // memset(&GPIO_InitStruct, 0, sizeof(GPIO_TypeDef));
} 

void hal_init(void)
{
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    rc_printf("preparing");
    gpio_init();
    MX_CAN_Init();
    DMA_ADC_IRQ_SET();
	MX_SPI1_Init();
	  app_start();
}    
    

void handle_led_action(uint8_t index, uint8_t cmd)
{
    if (cmd == 0) {
        HAL_GPIO_WritePin(gpioleds[index].port, gpioleds[index].pin, LED_STATUS_OFF);
    } else if (cmd == 1) {
        HAL_GPIO_WritePin(gpioleds[index].port, gpioleds[index].pin, LED_STATUS_ON);
    } else if (cmd == 2) {
        HAL_GPIO_TogglePin(gpioleds[index].port, gpioleds[index].pin);
    }
} 
    

void DMA_ADC_IRQ_SET(void)
{
    MX_DMA_Init();
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn,
      configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+1, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    MX_ADC1_Init();
    HAL_NVIC_SetPriority(ADC1_2_IRQn,
        configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+1, 0);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
}    
    
void ADC_EN(void)   //初始化ADC数据承接体、计数器值。开启DMA
{
#if 1
    osDelay(300);
    HAL_ADCEx_Calibration_Start(&hadc1);
    osDelay(300);
#endif

    memset(adcBuf, 0, sizeof(adcBuf));
    memset(&adcSamp, 0, sizeof(adcSamp));
    adcSampConvCpIdx = 0;
    adcSampIdx = 0;

    HAL_ADC_Start_DMA(&hadc1,(uint32_t *)adcBuf, ADC_MSG_MAX);
  
}   


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef * hadc) //回调函数，累加并找出最大/最小值
{
    uint32_t idx;

    adcSampConvCpIdx++;

    for(idx=0; idx<ADC_MSG_MAX; idx++){

        adcSamp[adcSampIdx].adc[idx] += (float)adcBuf[idx];//(float)
    }
  //rc_printf("EV: %d\n",adcBuf[2]);
    if(adcSampConvCpIdx == 1){

        for(idx=0; idx<ADC_MSG_MAX; idx++){
            
            adcSamp[adcSampConvCpIdx].adcFilterMax[idx] = adcBuf[idx];
            adcSamp[adcSampConvCpIdx].adcFilterMin[idx] = adcBuf[idx];
        } 
    } else {

        for(idx=0; idx<ADC_MSG_MAX; idx++){

            if(adcSamp[adcSampConvCpIdx].adcFilterMax[idx] < adcBuf[idx])
                adcSamp[adcSampIdx].adcFilterMax[idx] = adcBuf[idx];
            if(adcSamp[adcSampConvCpIdx].adcFilterMin[idx] > adcBuf[idx])
                adcSamp[adcSampIdx].adcFilterMin[idx] = adcBuf[idx];   
        }
    }

    if(adcSampConvCpIdx >= ADC_SAMP){

        adcSampIdx = !adcSampIdx;
        memset(&(adcSamp[adcSampIdx]), 0, sizeof(queue_adcInfo));
        adcSampConvCpIdx = 0;
    }

    memset(adcBuf, 0, sizeof(adcBuf));
    HAL_ADC_Start(&hadc1);
		HAL_ADC_Start_DMA(&hadc1,(uint32_t *)adcBuf, ADC_MSG_MAX);
}
   


void getADC_Conv(void *dis)
{
    int idx;
    queue_adcInfo *avg = dis;

    portDISABLE_INTERRUPTS();
    memcpy(avg, &(adcSamp[!adcSampIdx]), sizeof(queue_adcInfo));
    portENABLE_INTERRUPTS();

    for(idx=0; idx<ADC_MSG_MAX; idx++){
        
        avg->adc[idx] -= (float)avg ->adcFilterMax[idx];
        avg->adc[idx] -= (float)avg ->adcFilterMin[idx];
        avg->adc[idx] /= (float)(ADC_SAMP-2);
    }
}


float adcToEV(float adc)
{
    float EV;
    Sys_Info *sys;

    sys = getGlobalMsgNoneLock();
    EV = 16.0f * (adc*3.3f/4096.0f);

    //if(EV < 0)
       // EV = 0;
    
    return EV;
}

    































