#ifndef __CAN_APP_H__
#define __CAN_APP_H__


#include "sys_app.h"

/* Exported macro ------------------------------------------------------------*/

#define CAN_FIFO_LEN 1024						//CAN接收FIFO深度（8字节包数量）

#define CAN0_LEN_MAX 8								//CAN0消息处理最长字节(8的倍数)
#define CAN1_LEN_MAX 8								//CAN1消息处理最长字节(8的倍数)

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
  * 函数功能: CAN过滤器初始化
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 初始CAN配置参数，并启用
********************************************************/
void rc_can_app_init(uint8_t CanName, uint32_t CanFliterId, uint32_t mask);

/********************************************************
  * 函数功能: CAN消息发送
	* 输入参数: *phcan	:hcan1/hcan2
							id		:can ID
							*data	:发送数据指针
							len	:数据长度
	* 返 回 值: 0：发送失败  other:发送数据长度
  * 说    明: 根据ID，数据长度发送can消息
********************************************************/
int rc_can_send(CAN_HandleTypeDef *phcan, uint32_t id, uint8_t *data, uint8_t len);

/********************************************************
  * 函数功能: CAN消息处理任务
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 处理中断接收的can消息
********************************************************/
void rc_sys_can_app_poll(void);

#endif
