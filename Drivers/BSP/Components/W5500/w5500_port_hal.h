#ifndef _W5500_PORT_HAL_
#define _W5500_PORT_HAL_

#include "wizchip_conf.h"
#include "stm32f4xx.h"
#include <string.h>
#include <stdio.h>

#define W5500_SPI_HANDLE    hspi2
#define W5500_CS_PORT       GPIOB
#define W5500_CS_PIN        GPIO_PIN_9
#define W5500_RST_PORT      GPIOB
#define W5500_RST_PIN       GPIO_PIN_6


/* 定义该宏则表示使用自动协商模式，取消则设置为100M全双工模式 */
#define USE_AUTONEGO

/* 定义该宏则表示在初始化网络信息时设置DHCP */
//#define USE_DHCP

extern SPI_HandleTypeDef W5500_SPI_HANDLE;

void w5500_network_info_show(void);
int w5500_init(void);

#endif
