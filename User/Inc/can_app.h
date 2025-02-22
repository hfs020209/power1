#ifndef __CAN_APP_H__
#define __CAN_APP_H__


#include "sys_app.h"

/* Exported macro ------------------------------------------------------------*/

#define CAN_FIFO_LEN 1024						//CAN����FIFO��ȣ�8�ֽڰ�������

#define CAN0_LEN_MAX 8								//CAN0��Ϣ������ֽ�(8�ı���)
#define CAN1_LEN_MAX 8								//CAN1��Ϣ������ֽ�(8�ı���)

#define CAN_REG_GET_HIGH(x) (((x) >> 16) & 0xffff)
#define CAN_REG_GET_LOW(x) ((x)&0xffff)


/* Exported constants --------------------------------------------------------*/

enum can_name
{
  CAN1_NAME=0, 
	CAN2_NAME, 
	CAN_MAX
};


/* Exported types ------------------------------------------------------------*/

#pragma pack(1)
typedef struct
{
    uint8_t reserve : 1;
    uint8_t RTR : 1;
    uint8_t IDE : 1;
    uint32_t EXID : 18;
    uint16_t STID : 11;
} HalCanFilterReg_t;
#pragma pack()

#pragma pack(1)
typedef union
{
    uint32_t value;
    HalCanFilterReg_t reg;
} HalCANFilter_t;
#pragma pack()

#pragma pack(1)
typedef struct
{
    CAN_RxHeaderTypeDef header;
    uint8_t rce16_can_data[8];
} can_rx_data_type;
#pragma pack()

/* Exported functions --------------------------------------------------------*/

/********************************************************
  * ��������: CAN��������ʼ��
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: ��ʼCAN���ò�����������
********************************************************/
void rc_can_app_init(uint8_t CanName, uint32_t CanFliterId, uint32_t mask);

/********************************************************
  * ��������: CAN��Ϣ����
	* �������: *phcan	:hcan1/hcan2
							id		:can ID
							*data	:��������ָ��
							len	:���ݳ���
	* �� �� ֵ: 0������ʧ��  other:�������ݳ���
  * ˵    ��: ����ID�����ݳ��ȷ���can��Ϣ
********************************************************/
int rc_can_send(CAN_HandleTypeDef *phcan, uint32_t id, uint8_t *data, uint8_t len);

/********************************************************
  * ��������: CAN��Ϣ��������
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: �����жϽ��յ�can��Ϣ
********************************************************/
void rc_sys_can_app_poll(void);

#endif
