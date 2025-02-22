#ifndef __CH395_APP_H__
#define __CH395_APP_H__

#include "sys_app.h"

#define CH395_TIMEOUT 1000

/* Exported macro ------------------------------------------------------------*/


struct _CH395_SYS
{
    uint8_t   IPAddr[4];                                           /* CH395IP��ַ 32bit*/
    uint8_t   GWIPAddr[4];                                         /* CH395���ص�ַ 32bit*/
    uint8_t   MASKAddr[4];                                         /* CH395�������� 32bit*/
    uint8_t   MacAddr[6];                                          /* CH395MAC��ַ 48bit*/
    uint8_t   PHYStat;                                             /* CH395 PHY״̬�� 8bit*/
    uint8_t   MackFilt;                                            /* CH395 MAC���ˣ�Ĭ��Ϊ���չ㲥�����ձ���MAC 8bit*/
    uint32_t  RetranCount;                                         /* ���Դ��� Ĭ��Ϊ10��*/
    uint32_t  RetranPeriod;                                        /* ��������,��λMS,Ĭ��200MS */
    uint8_t   IntfMode;                                            /* �ӿ�ģʽ */
    uint8_t   UnreachIPAddr[4];                                    /* ���ɵ���IP */
    uint16_t  UnreachPort;                                         /* ���ɵ���˿� */
};

struct _SOCK_INF
{
    uint8_t  IPAddr[4];                                           /* socketĿ��IP��ַ 32bit*/
    uint8_t  MacAddr[6];                                          /* socketĿ���ַ 48bit*/
    uint8_t  ProtoType;                                           /* Э������ */
    uint8_t  ScokStatus;                                          /* socket״̬���ο�scoket״̬���� */
    uint8_t  TcpMode;                                             /* TCPģʽ */
    uint32_t IPRAWProtoType;                                      /* IPRAW Э������ */
    uint16_t DesPort;                                             /* Ŀ�Ķ˿� */
    uint16_t SourPort;                                            /* Ŀ�Ķ˿� */
    uint16_t SendLen;                                             /* �������ݳ��� */
    uint16_t RemLen;                                              /* ʣ�೤�� */
    uint8_t  *pSend;                                              /* ����ָ�� */                                               
};    

/***********************************************************************************
* Function Name  : CH395Init
* Description    : ����CH395��IP,GWIP,MAC�Ȳ���������ʼ��
* Input          : None
* Output         : None
* Return         : ����ִ�н��
**********************************************************************************/
uint8_t  rc_eth_ch395_app_init(void);

/***********************************************************************************
* Function Name  : rc_sys_ch395_app_poll
* Description    : ����CH395��IP,GWIP,MAC�Ȳ���������ʼ��
* Input          : None
* Output         : None
* Return         : ����ִ�н��
**********************************************************************************/
void  rc_sys_ch395_app_poll(void);

#endif
