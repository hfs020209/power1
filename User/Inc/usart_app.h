#ifndef __USART_APP_H__
#define __USART_APP_H__



#include "sys_app.h"

#define UART_232		//232通讯要打开定义
//#define UART_HALF_DUPLEX//485全双工需要隐掉定义


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
#define UART_FIFO_LEN 20						//UART接收FIFO深度

#define UART_LEN_MAX 10							//uart接收字节长度

#define UART_TX_FIFO_MAX 100				//uart发送缓存深度

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
  * 函数功能: uart初始化选择IO
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 根据定义选择232或485，调整IO状态
********************************************************/
void rc_uart_app_init(void);

/********************************************************
  * 函数功能: uart发送数据填充
  * 输入参数: *pData	:数据指针
							Size	:数据长度
  * 返 回 值: 无
  * 说    明: 将发送数据填充到buff中
********************************************************/
void rc_uart_Send(uint8_t *pData, uint16_t Size);
	
/********************************************************
  * 函数功能: usart消息处理任务
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 处理接收的usart消息
********************************************************/
void rc_sys_uart_app_poll(void);
	
#endif
