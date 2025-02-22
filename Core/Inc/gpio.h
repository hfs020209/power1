/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

/********************************************************
  * 函数功能: LED引脚操作
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 控制IO电平
********************************************************/
void rc_led_state(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t led_vaule);

/********************************************************
  * 函数功能: 硬件看门狗喂狗
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 定时翻转引脚喂狗
********************************************************/
void rc_wdi_heard(void);

/********************************************************
  * 函数功能: 获取CAN_ID编码值
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 根据io输入状态获取编码值
********************************************************/
uint8_t rc_coder_get(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

