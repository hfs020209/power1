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
#include "check.h"
#include "protocol.h"


static queue_adcInfo adcmsg;    //adc message

static QueueHandle_t canQueueHandle;    //定义队列句柄
static queue_canInfo _queue_canMsg;     //定义结构体，用于承接队列中FIFO 0的数据
static Sys_Info globalMsg = {
    .verLocal = {0, 60, 2, 3},
}; //CAN-filter

static ApiProtocolHandleinfo *can1ProtolHdl;
static int protolcallback(void *obj, uint8_t *frame);



static void CAN_Task(void *arg)
{ 
    int ret;
    
    globalMsg.canFilterMode = CAN_FILTERMODE_IDLIST;
    globalMsg.canFilterScale = CAN_FILTERSCALE_16BIT;
    globalMsg.canFilterZone[0].canFilterId[0] = (globalMsg.slot<<4)&0xF0;
    globalMsg.canFilterZone[0].canFilterId[1] = 0;
    globalMsg.canFilterZone[0].canFilterId[2] = 0;
    globalMsg.canFilterZone[0].canFilterId[3] = 0;
    globalMsg.canFilterNum = 1;

    rc_printf("can task start\n");

    canQueueHandle = xQueueCreate(6, sizeof(queue_canInfo));    //队列创建，可理解为初始化

    canFilter_init(&globalMsg);
    can_start();

    for(;;){
        
        rc_printf("try can\n");
        ret = xQueueReceive(canQueueHandle, &_queue_canMsg, portMAX_DELAY);

        if(ret <=0){
            rc_printf("get no can\n");
            continue;
        }
        
        switch(_queue_canMsg.type){
            default:;break;

            case QUEUE_CAN_ISR_MSG:{
                
                rc_printf("get can msg ID:%x EXT:%x\n",
                    _queue_canMsg.msg.hdr.StdId, _queue_canMsg.msg.hdr.ExtId);
                
                //uint8_t can_data[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};    
                //Can_Send(2, can_data);   
                can1ProtolHdl->process(can1ProtolHdl, _queue_canMsg.msg.data);
            };break;
        }
    }
                	
}


    


static void LED_Handle(void *arg)
{  
		for(;;){
				handle_led_action(LED_C0, 0);
				handle_led_action(LED_C1, 2);
				handle_led_action(LED_C2, 2);
				osDelay(1000);
		}
}






extern uint16_t adcBuf[ADC_MSG_MAX];
static void ADC_Handle(void *arg)
{  
    rc_printf("task 1 start\n");
    can1ProtolHdl = protocolHandleOpen(protolcallback, Can_Send);
    can1ProtolHdl->sendId = globalMsg.hostId;
    vSemaphoreCreateBinary(globalMsg.Lock);

    xTaskCreate(CAN_Task, "can", 0x600, NULL, 1, NULL);
    ADC_EN();
    osDelay(1000);

	for(;;){

        getADC_Conv(&adcmsg);
        globalMsg.V[0] = adcToEV(adcmsg.adc[ADC_CH_EV_1]);
        globalMsg.V[1] = adcToEV(adcmsg.adc[ADC_CH_EV_2]);
        uint8_t *data = (uint8_t*)& globalMsg.V[0];
        can1ProtolHdl->load_SendPkt(can1ProtolHdl, CAN_PROTOL_CMD_KEEPLIVE_A, data);
       #if 0

       
           rc_printf("EI %f\n",globalMsg.V[0]);
            //rc_printf("EV %f\n",globalMsg.V[0]);
					    osDelay(1000);
       #endif
		
	}
}



void app_start(void)
{    
    xTaskCreate(LED_Handle, "task led", 0x600, NULL, 1, NULL);
	xTaskCreate(ADC_Handle, "task adc", 0x600, NULL, 1, NULL);
    //xTaskCreate(CAN_Task, "task can", 0x600, NULL, 1, NULL);
}


void pushCanQueueFromIsr(queue_canInfo *queue_can)
{
    if(canQueueHandle == NULL)
        return;

    xQueueSendFromISR(canQueueHandle, queue_can, NULL);
}

Sys_Info *getGlobalMsg(void)
{
    xSemaphoreTake(globalMsg.Lock, portMAX_DELAY);
    return &globalMsg;
}

void releaseGlobalMsg(void)
{
    xSemaphoreGive(globalMsg.Lock);
}

Sys_Info *getGlobalMsgNoneLock(void)
{
    return &globalMsg;
}

static int protolcallback(void *obj, uint8_t *frame)
{
    protol_pwrCmdInfo *pkt = (void*)frame;
    ApiProtocolHandleinfo *hdl = obj;

    switch(pkt->cmd){

        default:;break;

        case CAN_PROTOL_CMD_VER_REQ:{

            hdl->load_SendPkt(hdl, CAN_PROTOL_CMD_VER_REQ, globalMsg.verLocal);
        };break;
    }

    return pkt->cmd;
}
































