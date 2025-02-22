#ifndef __USART_APP_H__
#define __USART_APP_H__



#include "sys_app.h"

#define UART_232		//232ͨѶҪ�򿪶���
//#define UART_HALF_DUPLEX//485ȫ˫����Ҫ��������


#define RE_485_EN					HAL_GPIO_WritePin(RE_RS485_GPIO_Port, RE_RS485_Pin, GPIO_PIN_RESET);
#define RE_485_DEN				HAL_GPIO_WritePin(RE_RS485_GPIO_Port, RE_RS485_Pin, GPIO_PIN_SET);

#define DE_485_EN					HAL_GPIO_WritePin(DE_RS485_GPIO_Port, DE_RS485_Pin, GPIO_PIN_SET);
#define DE_485_DEN				HAL_GPIO_WritePin(DE_RS485_GPIO_Port, DE_RS485_Pin, GPIO_PIN_RESET);

#define UART_DEBUG	

#ifdef UART_DEBUG
#define Log(format,...)        			printf(format,##__VA_ARGS__);

#define UserLog(format,...)        	printf(format,##__VA_ARGS__);\
																		printf("\r\n");
							
#define debug_log(format,...)      	printf("DEBUG->") ;\
																		printf(format,##__VA_ARGS__);\
																		printf("\r\n");
														
#define err_log(format,...)        	printf("ERROR[%s]->%s %d:", __FILE__, __func__ ,__LINE__) ;\
																		printf(format,##__VA_ARGS__);\
																		printf("\r\n");
																		
#else
#define UserLog(format,...)
#define debug_log(format,...) 
#define err_log(format,...)
#endif
#define UART_FIFO_LEN 20						//UART����FIFO���

#define UART_LEN_MAX 10							//uart�����ֽڳ���

#define UART_TX_FIFO_MAX 100				//uart���ͻ������

#pragma pack(1)
typedef struct
{
    uint8_t rce16_uart_data[UART_LEN_MAX];
} uart_rx_data_type;
#pragma pack()

#pragma pack(1)
typedef struct
{
		uint16_t len;
    uint8_t rce16_uart_data[UART_TX_FIFO_MAX];
} uart_tx_data_type;
#pragma pack()

/********************************************************
  * ��������: uart��ʼ��ѡ��IO
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: ���ݶ���ѡ��232��485������IO״̬
********************************************************/
void rc_uart_app_init(void);

/********************************************************
  * ��������: uart�����������
  * �������: *pData	:����ָ��
							Size	:���ݳ���
  * �� �� ֵ: ��
  * ˵    ��: ������������䵽buff��
********************************************************/
void rc_uart_Send(uint8_t *pData, uint16_t Size);
	
/********************************************************
  * ��������: usart��Ϣ��������
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: ������յ�usart��Ϣ
********************************************************/
void rc_sys_uart_app_poll(void);
	
#endif
