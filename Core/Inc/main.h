/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gpio.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "w5500_port_hal.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RE_RS485_Pin GPIO_PIN_2
#define RE_RS485_GPIO_Port GPIOE
#define DE_RS485_Pin GPIO_PIN_3
#define DE_RS485_GPIO_Port GPIOE
#define I2C2_SDA_Pin GPIO_PIN_0
#define I2C2_SDA_GPIO_Port GPIOF
#define I2C2_SCL_Pin GPIO_PIN_1
#define I2C2_SCL_GPIO_Port GPIOF
#define LED_RUN_Pin GPIO_PIN_6
#define LED_RUN_GPIO_Port GPIOF
#define LED_COM_Pin GPIO_PIN_7
#define LED_COM_GPIO_Port GPIOF
#define LED_CAN1_Pin GPIO_PIN_8
#define LED_CAN1_GPIO_Port GPIOF
#define LED_CAN2_Pin GPIO_PIN_9
#define LED_CAN2_GPIO_Port GPIOF
#define LED_LAN_Pin GPIO_PIN_10
#define LED_LAN_GPIO_Port GPIOF
#define W5500_MISO_Pin GPIO_PIN_2
#define W5500_MISO_GPIO_Port GPIOC
#define W5500_MOSI_Pin GPIO_PIN_3
#define W5500_MOSI_GPIO_Port GPIOC
#define WI1_Pin GPIO_PIN_3
#define WI1_GPIO_Port GPIOA
#define WI2_Pin GPIO_PIN_4
#define WI2_GPIO_Port GPIOA
#define WI3_Pin GPIO_PIN_5
#define WI3_GPIO_Port GPIOA
#define WI4_Pin GPIO_PIN_6
#define WI4_GPIO_Port GPIOA
#define WI5_Pin GPIO_PIN_0
#define WI5_GPIO_Port GPIOB
#define TEMP_Pin GPIO_PIN_1
#define TEMP_GPIO_Port GPIOB
#define CAN_ID0_MCU_Pin GPIO_PIN_7
#define CAN_ID0_MCU_GPIO_Port GPIOE
#define CAN_ID1_MCU_Pin GPIO_PIN_8
#define CAN_ID1_MCU_GPIO_Port GPIOE
#define CAN_ID2_MCU_Pin GPIO_PIN_9
#define CAN_ID2_MCU_GPIO_Port GPIOE
#define CAN_ID3_MCU_Pin GPIO_PIN_10
#define CAN_ID3_MCU_GPIO_Port GPIOE
#define CAN_ID4_MCU_Pin GPIO_PIN_11
#define CAN_ID4_MCU_GPIO_Port GPIOE
#define CAN_ID5_MCU_Pin GPIO_PIN_12
#define CAN_ID5_MCU_GPIO_Port GPIOE
#define W5500_SCK_Pin GPIO_PIN_10
#define W5500_SCK_GPIO_Port GPIOB
#define CAN2_RX_Pin GPIO_PIN_12
#define CAN2_RX_GPIO_Port GPIOB
#define CAN2_TX_Pin GPIO_PIN_13
#define CAN2_TX_GPIO_Port GPIOB
#define W5500_INT_Pin GPIO_PIN_6
#define W5500_INT_GPIO_Port GPIOC
#define ETH_RESET_Pin GPIO_PIN_8
#define ETH_RESET_GPIO_Port GPIOA
#define USART1_TX_Pin GPIO_PIN_9
#define USART1_TX_GPIO_Port GPIOA
#define USART1_RX_Pin GPIO_PIN_10
#define USART1_RX_GPIO_Port GPIOA
#define CAN1_RX_Pin GPIO_PIN_11
#define CAN1_RX_GPIO_Port GPIOA
#define CAN1_TX_Pin GPIO_PIN_12
#define CAN1_TX_GPIO_Port GPIOA
#define EEP_CS_Pin GPIO_PIN_15
#define EEP_CS_GPIO_Port GPIOA
#define USART3_TX_Pin GPIO_PIN_10
#define USART3_TX_GPIO_Port GPIOC
#define USART3_RX_Pin GPIO_PIN_11
#define USART3_RX_GPIO_Port GPIOC
#define EEP_SCK_Pin GPIO_PIN_3
#define EEP_SCK_GPIO_Port GPIOB
#define EEP_MISO_Pin GPIO_PIN_4
#define EEP_MISO_GPIO_Port GPIOB
#define EEP_MOSI_Pin GPIO_PIN_5
#define EEP_MOSI_GPIO_Port GPIOB
#define W5500_RST_Pin GPIO_PIN_6
#define W5500_RST_GPIO_Port GPIOB
#define W5500_CS_Pin GPIO_PIN_9
#define W5500_CS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define WDI_PORT          GPIOC
#define WDI_PIN   				GPIO_PIN_15 
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
