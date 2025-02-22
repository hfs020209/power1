#include "usart_app.h"
#include "stdarg.h"

uint8_t UART1_PRINTF_BUFF[UART_LEN_MAX];


uart_rx_data_type g_uart_packet[UART_FIFO_LEN];
volatile uint16_t uart_fifo_tail = 0;
volatile uint16_t uart_fifo_hader = 0;

uart_tx_data_type rc_uart_tx_pack[2]={0};

static uint8_t uart1_tx_flag=0;	//����buff�л���־λ

static uint8_t uart1_flag=0;		//�����շ�״̬��־λ

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
  * ��������: uart������Ϣ
  * �������: *pData	:����ָ��
							Size	:���ݳ���
							Timeout	:��ʱʱ��
  * �� �� ֵ: ��
  * ˵    ��: ֻ��Ҫִ��һ�Σ�����ÿ�յ��̶����ȴ����ж�
********************************************************/
static uint8_t rc_uart_Receive(uint8_t *pData, uint16_t Size)
{
	uint8_t ret = HAL_UART_Receive_DMA( &huart1, pData, Size);
	return ret;
}

/********************************************************
  * ��������: uart����
  * �������: *pData	:����ָ��
							Size	:���ݳ���
  * �� �� ֵ: ��
  * ˵    ��: DMA������Ϣ
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
  * ��������: uart�����������
  * �������: *pData	:����ָ��
							Size	:���ݳ���
  * �� �� ֵ: ��
  * ˵    ��: ������������䵽buff��
********************************************************/
void rc_uart_Send(uint8_t *pData, uint16_t Size)
{
	uint8_t flag = uart1_tx_flag;
	memcpy(&rc_uart_tx_pack[flag].rce16_uart_data[rc_uart_tx_pack[flag].len],pData,Size);
	rc_uart_tx_pack[flag].len += Size;
}

/********************************************************
  * ��������: usart��������buff
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: �������ready������buff��������,��������
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
  * ��������: uart��ʼ��ѡ��IO
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: ���ݶ���ѡ��232��485������IO״̬
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
  * ��������: usart��Ϣ��������
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: ������յ�usart��Ϣ
********************************************************/
void rc_sys_uart_app_poll(void)
{
	
	if(uart_fifo_hader != uart_fifo_tail)
	{
		
		/*������ݴ���ʼ*/
		rc_uart_Send(g_uart_packet[uart_fifo_hader].rce16_uart_data,UART_LEN_MAX);
		/*������ݴ������*/
		uart_fifo_hader++;
		if(uart_fifo_hader >= UART_FIFO_LEN){    //�������
	
			uart_fifo_hader = 0;
		
		}	
	}
	
	rc_sys_uart_tx_work();
	
}


/********************************************************
  * ��������: usart�����жϴ���
  * �������: huart
  * �� �� ֵ: ��
  * ˵    ��: ������յ�usart��Ϣ
********************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == huart1.Instance)
	{
		memcpy(g_uart_packet[uart_fifo_tail].rce16_uart_data,UART1_PRINTF_BUFF,UART_LEN_MAX);

		uart_fifo_tail++;			//����β����	
	
		if(uart_fifo_tail >= UART_FIFO_LEN){    //�������
	
			uart_fifo_tail = 0;
		
		}					  

	}
}

/********************************************************
  * ��������: usart��������жϴ���
  * �������: huart
  * �� �� ֵ: ��
  * ˵    ��: ���������Ҫ�ؿ��ж�
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
