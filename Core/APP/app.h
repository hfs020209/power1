#ifndef __APP_H__
#define __APP_H__
#include "rc_can.h"

typedef enum{
	QUEUE_CAN_ISR_MSG = 5,

	//add QUEUE_ADC...
}QUEUE_MSG_TYPE;

void app_start(void);
Sys_Info *getGlobalMsg(void);
void releaseGlobalMsg(void);
Sys_Info *getGlobalMsgNoneLock(void);
void pushCanQueueFromIsr(queue_canInfo *queue_can);










#endif





















