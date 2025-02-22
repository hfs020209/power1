#include "usart_app.h"
#include "stdarg.h"

uint8_t UART1_PRINTF_BUFF[UART_LEN_MAX];


uart_rx_data_type g_uart_packet[UART_FIFO_LEN];
volatile uint16_t uart_fifo_tail = 0;
volatile uint16_t uart_fifo_hader = 0;

uart_tx_data_type rc_uart_tx_pack[2]={0};

static uint8_t uart1_tx_flag=0;	//发送buff切换标志位

static uint8_t uart1_flag=0;		//串口收发状态标志位

void Uart1_printf(const char *format, ...)
{
	uint32_t length;
    va_list args;
		
    va_start(args, format);
    length = vsnprintf((char *)UART1_PRINTF_BUFF, sizeof(UART1_PRINTF_BUFF), (char *)format, args);
    va_end(args);
	
		HAL_UART_Transmit(&huart1, UART1_PRINTF_BUFF, length, 0xffff);
}

/********************************************************
  * 函数功能: uart接收消息
  * 输入参数: *pData	:数据指针
							Size	:数据长度
							Timeout	:超时时间
  * 返 回 值: 无
  * 说    明: 只需要执行一次，后续每收到固定长度触发中断
********************************************************/
static uint8_t rc_uart_Receive(uint8_t *pData, uint16_t Size)
{
	uint8_t ret = HAL_UART_Receive_DMA( &huart1, pData, Size);
	return ret;
}

/********************************************************
  * 函数功能: uart发送
  * 输入参数: *pData	:数据指针
							Size	:数据长度
  * 返 回 值: 无
  * 说    明: DMA发送消息
********************************************************/
static uint8_t rc_uart_send_buff(uint8_t *pData, uint16_t Size)
{
	uint8_t ret = HAL_OK;
#ifdef UART_HALF_DUPLEX
	RE_485_DEN;
	DE_485_EN;
#endif
	ret = HAL_UART_Transmit_DMA(&huart1, pData, Size);

	return ret;
}

/********************************************************
  * 函数功能: uart发送数据填充
  * 输入参数: *pData	:数据指针
							Size	:数据长度
  * 返 回 值: 无
  * 说    明: 将发送数据填充到buff中
********************************************************/
void rc_uart_Send(uint8_t *pData, uint16_t Size)
{
	uint8_t flag = uart1_tx_flag;
	memcpy(&rc_uart_tx_pack[flag].rce16_uart_data[rc_uart_tx_pack[flag].len],pData,Size);
	rc_uart_tx_pack[flag].len += Size;
}

/********************************************************
  * 函数功能: usart发送数据buff
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 如果发送ready，并且buff内有数据,则发送数据
********************************************************/
static uint8_t rc_sys_uart_tx_work(void)
{
	uint8_t ret = HAL_OK;
	if((uart1_flag & 0x02) == 0)
	{
		if(rc_uart_tx_pack[uart1_tx_flag].len!=0)
		{
			uart1_flag |= 0x02;
			ret = rc_uart_send_buff(rc_uart_tx_pack[uart1_tx_flag].rce16_uart_data,rc_uart_tx_pack[uart1_tx_flag].len);
			if(ret == HAL_OK)
			{
				rc_uart_tx_pack[uart1_tx_flag].len = 0;
				
				if(uart1_tx_flag==0)
					uart1_tx_flag=1;
				else
					uart1_tx_flag=0;
			}
			else
			{
#ifdef UART_HALF_DUPLEX
				RE_485_EN;
				DE_485_DEN;
#endif
				uart1_flag &= 0xFD;
				err_log("uart1 tx dma error :%d",ret);
			}
		}
	}
	else
	{
		ret = HAL_BUSY;
	}
	
	return ret;
}

/********************************************************
  * 函数功能: uart初始化选择IO
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 根据定义选择232或485，调整IO状态
********************************************************/
void rc_uart_app_init(void)
{
#ifdef UART_232
	RE_485_DEN;
	DE_485_DEN;
#else
	#ifdef	UART_HALF_DUPLEX
		RE_485_EN;
		DE_485_DEN;
	#else
		RE_485_DEN;
		DE_485_EN;
	#endif
#endif
		uint8_t cnt=0;
		while( HAL_OK != rc_uart_Receive(UART1_PRINTF_BUFF, UART_LEN_MAX))
		{
			HAL_Delay(10);
			cnt++;
			if(cnt>10)
			{
				err_log("uart1 rx error");
				Error_Handler();
			}
		}
		
		uart1_flag |= 1;
		UserLog("uart1 start rx");
}

/********************************************************
  * 函数功能: usart消息处理任务
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 处理接收的usart消息
********************************************************/
void rc_sys_uart_app_poll(void)
{
	
	if(uart_fifo_hader != uart_fifo_tail)
	{
		
		/*添加数据处理开始*/
		rc_uart_Send(g_uart_packet[uart_fifo_hader].rce16_uart_data,UART_LEN_MAX);
		/*添加数据处理结束*/
		uart_fifo_hader++;
		if(uart_fifo_hader >= UART_FIFO_LEN){    //队列溢出
	
			uart_fifo_hader = 0;
		
		}	
	}
	
	rc_sys_uart_tx_work();
	
}


/********************************************************
  * 函数功能: usart接收中断处理
  * 输入参数: huart
  * 返 回 值: 无
  * 说    明: 处理接收的usart消息
********************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == huart1.Instance)
	{
		memcpy(g_uart_packet[uart_fifo_tail].rce16_uart_data,UART1_PRINTF_BUFF,UART_LEN_MAX);

		uart_fifo_tail++;			//队列尾递增	
	
		if(uart_fifo_tail >= UART_FIFO_LEN){    //队列溢出
	
			uart_fifo_tail = 0;
		
		}					  

	}
}

/********************************************************
  * 函数功能: usart发送完成中断处理
  * 输入参数: huart
  * 返 回 值: 无
  * 说    明: 发送完成需要重开中断
********************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == huart1.Instance)
	{
#ifdef UART_HALF_DUPLEX
	RE_485_EN;
	DE_485_DEN;
#endif
		
		uart1_flag &= 0xFD;
	}
}
