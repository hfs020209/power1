#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#define PROTOL_BUF 20

#define CAN_PROTOL_HDR 0xD3

#define CAN_PROTOL_CMD_KEEPLIVE_V 0
#define CAN_PROTOL_CMD_KEEPLIVE_A 1
#define CAN_PROTOL_CMD_KEEPLIVE_T 2
#define CAN_PROTOL_CMD_VER_REQ 3

#pragma pack(1)     //使用 #pragma pack(1) 后，成员之间 无填充，结构体总大小等于所有成员大小之和。
typedef struct
{
    uint8_t hdr;
    uint8_t cmd;
    uint8_t rev[5];
    uint8_t crc;
}protol_pwrCmdInfo;

typedef struct
{
    uint8_t hdr;
    uint8_t cmd;
    uint8_t data[5];
    uint8_t crc;
}protol_pwrAckInfo;

typedef int parseCallBack(void*, uint8_t *);
typedef int protolSendPort(uint32_t, uint8_t *);

typedef struct
{
    QueueHandle_t Lock;

    uint32_t cmdTxBufMax;
    uint8_t cmdTxBuf[PROTOL_BUF];

    uint32_t sendId;

    int(*sendPort)(uint32_t, uint8_t *);
    int(*callback)(void*, uint8_t *);
    int(*load_SendPkt)(void *hdl, uint8_t cmd, uint8_t *data);
    int(*process)(void *hdl, uint8_t *msg);
}ApiProtocolHandleinfo;             //封装函数


ApiProtocolHandleinfo* protocolHandleOpen(
        parseCallBack *call, protolSendPort *port);


#endif



























