#ifndef __W5500_APP_H__
#define __W5500_APP_H__

#include "sys_app.h"


/* Exported macro ------------------------------------------------------------*/


//Ŀ���豸IP
#define DEFAULT_DEST_IP_ADDR	{192,168,0,180}
//�豸IP
#define DEFAULT_MAC_ADDR    {0x6C,0x4B,0x90,0xF5,0x83,0xB9}
#define DEFAULT_IP_ADDR     {192,168,0,112}
#define DEFAULT_SUB_MASK    {255,255,0,0}
#define DEFAULT_GW_ADDR     {192,168,0,1}
#define DEFAULT_DNS_ADDR    {0,0,0,0}

#define W5500_SOURCE_PORT    (uint16_t) 8889   /* UDP�������Ӷ˿� */
#define W5500_DEST_PORT    	 (uint16_t) 8899   /* UDPԶ�����Ӷ˿� */

#define W5500_TIMEOUT 1000

#define UDP_RX_BUFSIZE_T		1480	//����udp���������ݳ���(����UDP8�ֽ��ײ��ֽ�)

/* TCP server Loopback test example */
int32_t loopback_tcps(uint8_t sn, uint8_t* buf, uint16_t port);

/* TCP client Loopback test example */
int32_t loopback_tcpc(uint8_t sn, uint8_t* buf, uint8_t* destip, uint16_t destport);

/* UDP Loopback test example */
int32_t loopback_udps(uint8_t sn, uint8_t* buf, uint16_t port);

/********************************************************
  * ��������: CAN��������ʼ��
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: ��ʼCAN���ò�����������
********************************************************/
void rc_eth_w5500_app_init(void);

/********************************************************
  * ��������: udp���ݷ��ʹ���
  * �������: void
  * �� �� ֵ: void
  * ˵    ��: ��
********************************************************/
void w5500_send_work(uint8_t *SendData,uint16_t size);

/********************************************************
  * ��������: w5500���������ж�
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: w5500���������жϴ���
********************************************************/
void HAL_W5500_INT_Callback(void);

/********************************************************
  * ��������: w5500������
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: ���ݻػ�����
********************************************************/
void rc_sys_w5500_app_poll(void);
#endif
