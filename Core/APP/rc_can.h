#ifndef __RC_CAN_H__
#define __RC_CAN_H__


#include "usart.h"
#include "adc.h"
#include "dma.h"
#include "gpio.h"
#include "can.h"

#define CAN_MSG_MAX 8

typedef struct{
    CAN_RxHeaderTypeDef hdr;
    uint8_t data[CAN_MSG_MAX];
}canMsgInfo;

typedef struct{
    uint32_t type;
    canMsgInfo msg;
}queue_canInfo;

typedef struct{
    uint32_t canFilterId[4];
}canFilterInfo;
    
typedef struct{
    QueueHandle_t Lock;
    canFilterInfo canFilterZone[1];
    uint32_t canFilterMode;
    uint32_t canFilterScale;
    uint32_t canFilterNum;

    uint8_t hostId;

    uint8_t verLocal[5];

    float baseEV;
    float baseEI;

    uint8_t slot;

    float V[2];
    float I[2];
    float T[2];
}Sys_Info;
    
void canFilter_init(Sys_Info *config);
void can_start(void);    
int Can_Send(uint32_t id, uint8_t *data);






#endif
























