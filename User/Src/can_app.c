#include "can_app.h"


/* --------------------------------------------------------------------------
 *  CAN RX
 * --------------------------------------------------------------------------
 */
can_rx_data_type g_can0_packet[CAN_FIFO_LEN];	
volatile uint16_t can0_fifo_tail = 0;
volatile uint16_t can0_fifo_hader = 0;
uint8_t rc_can0_rx_pack[CAN0_LEN_MAX];

can_rx_data_type g_can1_packet[CAN_FIFO_LEN];
volatile uint16_t can1_fifo_tail = 0;
volatile uint16_t can1_fifo_hader = 0;
uint8_t rc_can1_rx_pack[CAN1_LEN_MAX];
/********************************************************
  * ��������: CAN��������ʼ��
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: ��ʼCAN���ò�����������
********************************************************/
void rc_can_app_init(uint8_t CanName, uint32_t CanFliterId, uint32_t mask)
{
	if((CanName >= 2) || (CanFliterId > 0x7ff)){
		return;
	}
	
	CAN_FilterTypeDef canConfig;
	
	HalCANFilter_t filter = {0};

  filter.reg.IDE = 0;
  filter.reg.RTR = 0;
  filter.reg.STID = CanFliterId;
	
	if(CanName == CAN1_NAME){
	
		canConfig.FilterIdHigh = CAN_REG_GET_HIGH(filter.value);
		canConfig.FilterIdLow = CAN_REG_GET_LOW(filter.value);
		canConfig.FilterMaskIdHigh = CAN_REG_GET_HIGH(mask);
		canConfig.FilterMaskIdLow = CAN_REG_GET_LOW(mask);
		canConfig.FilterMode = CAN_FILTERMODE_IDMASK;
		canConfig.FilterScale = CAN_FILTERSCALE_32BIT;
		canConfig.FilterBank = 0;
		canConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
		canConfig.FilterActivation = ENABLE;
		canConfig.SlaveStartFilterBank = 0;
		HAL_CAN_ConfigFilter(&hcan1, &canConfig); //can1	
		HAL_CAN_Start(&hcan1);
		HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
		UserLog("CAN1 init success");
	}
	if(CanName == CAN2_NAME){

		canConfig.FilterIdHigh = CAN_REG_GET_HIGH(filter.value);
		canConfig.FilterIdLow = CAN_REG_GET_LOW(filter.value);
		canConfig.FilterMaskIdHigh = CAN_REG_GET_HIGH(mask);
		canConfig.FilterMaskIdLow = CAN_REG_GET_LOW(mask);
		canConfig.FilterMode = CAN_FILTERMODE_IDMASK;
		canConfig.FilterScale = CAN_FILTERSCALE_32BIT;
		canConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
		canConfig.FilterActivation = ENABLE;
		canConfig.FilterBank = 0;
		canConfig.SlaveStartFilterBank = 0;
		HAL_CAN_ConfigFilter(&hcan2, &canConfig); //can2
		HAL_CAN_Start(&hcan2);
		HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
		UserLog("CAN2 init success");
	}
}

/********************************************************
  * ��������: CAN��Ϣ����
	* �������: *phcan	:hcan1/hcan2
							id		:can ID
							*data	:��������ָ��
							len	:���ݳ���
	* �� �� ֵ: 0������ʧ��  other:�������ݳ���
  * ˵    ��: ����ID�����ݳ��ȷ���can��Ϣ
********************************************************/
int rc_can_send(CAN_HandleTypeDef *phcan, uint32_t id, uint8_t *data, uint8_t len)
{
    CAN_TxHeaderTypeDef header;
    uint32_t mailbox;
    int ret;

    header.DLC = len;
    header.StdId = id;
    header.IDE = CAN_ID_STD;
    header.RTR = CAN_RTR_DATA;
    header.TransmitGlobalTime = DISABLE;
		
    ret = HAL_CAN_AddTxMessage(phcan, &header, data, &mailbox);
    return ret == HAL_OK ? len : 0;
}

/********************************************************
  * ��������: CAN��Ϣ��������
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: �����жϽ��յ�can��Ϣ
********************************************************/
void rc_sys_can_app_poll(void)
{
	//can1 poll
	if(can0_fifo_hader != can0_fifo_tail)
	{
		for(uint8_t i=0;i<(CAN0_LEN_MAX/8);i++)
		{
			if(((i+1)*8)== CAN0_LEN_MAX)
				memcpy(&rc_can0_rx_pack[CAN0_LEN_MAX-8], g_can0_packet[can0_fifo_hader].rce16_can_data, 8);
			else
				memcpy(&rc_can0_rx_pack[i*8], &rc_can0_rx_pack[((i+1)*8)], 8);
		}
		
		/***********�������ݴ���ʼ************/
		rc_can_send(&hcan1, 0x01, rc_can0_rx_pack, 8);
		/***********�������ݴ������************/
		
		can0_fifo_hader++;
		if(can0_fifo_hader >= CAN_FIFO_LEN){    //�������
	
			can0_fifo_hader = 0;
		
		}
	}
	
	//can2 poll
	if(can1_fifo_hader != can1_fifo_tail)
	{
		for(uint8_t i=0;i<(CAN1_LEN_MAX/8);i++)
		{
			if(((i+1)*8)== CAN1_LEN_MAX)
				memcpy(&rc_can1_rx_pack[CAN1_LEN_MAX-8], g_can1_packet[can1_fifo_hader].rce16_can_data, 8);
			else
				memcpy(&rc_can1_rx_pack[i*8], &rc_can1_rx_pack[((i+1)*8)], 8);
		}
		
		/***********�������ݴ���ʼ************/
		rc_can_send(&hcan2, 0x01, rc_can1_rx_pack, 8);
		/***********�������ݴ������************/
		
		can1_fifo_hader++;
		if(can1_fifo_hader >= CAN_FIFO_LEN){    //�������
	
			can1_fifo_hader = 0;
		
		}
	}
}

/********************************************************
  * ��������: CAN_FIFO0�жϽ���
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: �ض���CAN��FIFO0�������жϽ���
********************************************************/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *phcan)
{
    if (phcan->Instance == hcan1.Instance)
    {
				while((CAN1->RF0R & 0x3) != 0) //����FIFO0
				{
					if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &g_can0_packet[can0_fifo_tail].header, g_can0_packet[can0_fifo_tail].rce16_can_data) == HAL_OK)
					{
							can0_fifo_tail++;			//����β����	
						
							if(can0_fifo_tail >= CAN_FIFO_LEN){    //�������
						
								can0_fifo_tail = 0;
							
							}					  

					}				
				}
		}
		else if (phcan->Instance == hcan2.Instance)
    {
				while((CAN2->RF0R & 0x3) != 0) //����FIFO0
				{
					if (HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO0, &g_can1_packet[can1_fifo_tail].header, g_can1_packet[can1_fifo_tail].rce16_can_data) == HAL_OK)
					{
							can1_fifo_tail++;			//����β����	
						
							if(can1_fifo_tail >= CAN_FIFO_LEN){    //�������
						
								can1_fifo_tail = 0;
							
							}
					}
				}
		}

    HAL_CAN_ActivateNotification(phcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}


