/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RE_RS485_GPIO_Port, RE_RS485_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DE_RS485_GPIO_Port, DE_RS485_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, LED_RUN_Pin|LED_COM_Pin|LED_CAN1_Pin|LED_CAN2_Pin
                          |LED_LAN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ETH_RESET_GPIO_Port, ETH_RESET_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(EEP_CS_GPIO_Port, EEP_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PEPin PEPin */
  GPIO_InitStruct.Pin = RE_RS485_Pin|DE_RS485_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PFPin PFPin PFPin PFPin
                           PFPin */
  GPIO_InitStruct.Pin = LED_RUN_Pin|LED_COM_Pin|LED_CAN1_Pin|LED_CAN2_Pin
                          |LED_LAN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PEPin PEPin PEPin PEPin
                           PEPin PEPin */
  GPIO_InitStruct.Pin = CAN_ID0_MCU_Pin|CAN_ID1_MCU_Pin|CAN_ID2_MCU_Pin|CAN_ID3_MCU_Pin
                          |CAN_ID4_MCU_Pin|CAN_ID5_MCU_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = W5500_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(W5500_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.Pin = ETH_RESET_Pin|EEP_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = W5500_RST_Pin|W5500_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/********************************************************
  * 函数功能: LED引脚操作
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 控制IO电平
********************************************************/
void rc_led_state(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t led_vaule)
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, (GPIO_PinState)led_vaule);
}

/********************************************************
  * 函数功能: 硬件看门狗喂狗
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 定时翻转引脚喂狗
********************************************************/
void rc_wdi_heard(void)
{
	static uint8_t wdi_vaule = 0;
	
	wdi_vaule = wdi_vaule == 0 ? 1 : 0;
	
	HAL_GPIO_WritePin(WDI_PORT,WDI_PIN ,(GPIO_PinState)wdi_vaule);
}

/********************************************************
  * 函数功能: 获取CAN_ID编码值
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 根据io输入状态获取编码值
********************************************************/
uint8_t rc_coder_get(void)
{
	uint8_t coder_num = 0;
	uint8_t bit0 = 0;
	uint8_t bit1 = 0;
	uint8_t bit2 = 0;
	uint8_t bit3 = 0;
	uint8_t bit4 = 0;
	uint8_t bit5 = 0;
	
	bit0 = HAL_GPIO_ReadPin(CAN_ID0_MCU_GPIO_Port, CAN_ID0_MCU_Pin);
	bit1 = HAL_GPIO_ReadPin(CAN_ID1_MCU_GPIO_Port, CAN_ID1_MCU_Pin);
	bit2 = HAL_GPIO_ReadPin(CAN_ID2_MCU_GPIO_Port, CAN_ID2_MCU_Pin);
	bit3 = HAL_GPIO_ReadPin(CAN_ID3_MCU_GPIO_Port, CAN_ID3_MCU_Pin);
	bit4 = HAL_GPIO_ReadPin(CAN_ID4_MCU_GPIO_Port, CAN_ID4_MCU_Pin);
	bit5 = HAL_GPIO_ReadPin(CAN_ID5_MCU_GPIO_Port, CAN_ID5_MCU_Pin);
	
	bit0 = bit0&0x01;
	bit1 = bit1&0x01;
	bit2 = bit2&0x01;
	bit3 = bit3&0x01;
	bit4 = bit4&0x01;
	bit5 = bit5&0x01;
	
	
	coder_num = (bit5<<5) | (bit4<<4) | (bit3<<3) | (bit2<<2) | (bit1<<1) | bit0; 
	return coder_num;
}

/* USER CODE END 2 */
