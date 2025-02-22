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


/* ����ú����ʾʹ���Զ�Э��ģʽ��ȡ��������Ϊ100Mȫ˫��ģʽ */
#define USE_AUTONEGO

/* ����ú����ʾ�ڳ�ʼ��������Ϣʱ����DHCP */
//#define USE_DHCP

extern SPI_HandleTypeDef W5500_SPI_HANDLE;

void w5500_network_info_show(void);
int w5500_init(void);

#endif
