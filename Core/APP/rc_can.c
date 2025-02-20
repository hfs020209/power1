#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "main.h"
#include "cmsis_os.h"
#include "common.h"
#include "Hal.h"
#include "app.h"
#include "rc_can.h"

static queue_canInfo queue_canCallBackBuf;

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *phcan)
{
    if(phcan->Instance == hcan.Instance){

        while((CAN1->RF0R & 0x3) != 0){

            HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0,
                &queue_canCallBackBuf.msg.hdr, queue_canCallBackBuf.msg.data);

            queue_canCallBackBuf.type = QUEUE_CAN_ISR_MSG;
            pushCanQueueFromIsr(&queue_canCallBackBuf);     //将CAN消息推送到队列中
        }
    }
}

int Can_Send(uint32_t id, uint8_t *data)
{
    CAN_TxHeaderTypeDef header;
    uint32_t mailbox;
    int ret;

    memset(&header, 0,sizeof(header));
    header.DLC = 8;
    header.StdId = id;
    header.IDE = CAN_ID_STD;
    header.RTR = CAN_RTR_DATA;
    header.TransmitGlobalTime = DISABLE;
    ret = HAL_CAN_AddTxMessage(&hcan, &header, data, &mailbox);

    return ret == HAL_OK ? 8 : 0;
}


void canFilter_init(Sys_Info *config)
{
    CAN_FilterTypeDef canConfig;
    int idx;

    switch (config->canFilterMode){

        default:;break;

        case CAN_FILTERMODE_IDLIST:{

            if(config->canFilterScale == CAN_FILTERSCALE_16BIT){

                for(idx=0; idx<config->canFilterNum; idx++){

                    canConfig.FilterBank = idx;
                    canConfig.FilterIdHigh = config->canFilterZone[idx].canFilterId[0]<<5;
                    canConfig.FilterIdLow = config->canFilterZone[idx].canFilterId[1]<<5;
                    canConfig.FilterMaskIdHigh =config->canFilterZone[idx].canFilterId[2]<<5;
                    canConfig.FilterMaskIdLow = config->canFilterZone[idx].canFilterId[3]<<5;
                }
            }
            
        }break;
    }

    canConfig.FilterMode = config->canFilterMode;
    canConfig.FilterScale = config->canFilterScale;
    canConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    canConfig.FilterActivation = ENABLE;
    canConfig.SlaveStartFilterBank = 0; //can2 use
    HAL_CAN_ConfigFilter(&hcan, &canConfig);
}



void can_start(void)
{
    
        HAL_CAN_Start(&hcan);
        HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	  
}











         

