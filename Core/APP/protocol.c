#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "main.h"
#include "cmsis_os.h"
#include "common.h"
#include "hal.h"
#include "app.h"
#include "check.h"
#include "protocol.h"

static int processCanPkt(void *obj, uint8_t *msg)
{
    uint8_t crc;
    protol_pwrCmdInfo *pkt = (void*)msg;
    ApiProtocolHandleinfo *hdl = obj;
    int ret;

    crc = Byte_Sum_Check(msg,7);

    if(pkt->hdr != CAN_PROTOL_HDR)
        return -1;

    if(crc != pkt->crc){

        rc_printf("crc err: %x - %x\n", crc, pkt->crc);
        return -2;
    }

    ret = hdl->callback(hdl,msg);

    return ret;
}


static int loadSendPktBuf(void *obj, uint8_t cmd, uint8_t *data)
{
    protol_pwrAckInfo *pkt;
    ApiProtocolHandleinfo *hdl = obj;
    int ret;

    xSemaphoreTake(hdl->Lock, portMAX_DELAY);

    pkt = (void*)hdl->cmdTxBuf;
    memset(pkt, 0, sizeof(protol_pwrAckInfo));
    pkt->hdr = CAN_PROTOL_HDR;
    pkt->cmd = cmd;

    if(data)
        memcpy(pkt->data, data, 5);

    pkt->crc = Byte_Sum_Check((void*)pkt, 7);

    ret = hdl->sendPort(hdl->sendId, (void*)pkt);

    xSemaphoreGive(hdl->Lock);

    return ret;



ApiProtocolHandleinfo* protocolHandleOpen(parseCallBack *call, protolSendPort *port)
{
    ApiProtocolHandleinfo *hdl;
    hdl = pvPortMalloc(sizeof(ApiProtocolHandleinfo));
    memset(hdl, 0, sizeof(ApiProtocolHandleinfo));

    vSemaphoreCreateBinary(hdl->Lock);
    hdl->sendPort = port;
    hdl->callback = call;
    hdl->cmdTxBufMax = PROTOL_BUF;
    hdl->load_SendPkt = loadSendPktBuf;
    hdl->process = processCanPkt;

    return hdl;
}


