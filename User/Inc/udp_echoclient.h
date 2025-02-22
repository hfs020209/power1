#ifndef __UDP_ECHOCLIENT_H__
#define __UDP_ECHOCLIENT_H__

/* Includes ------------------------------------------------------------------*/
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip.h"

/* Exported macro ------------------------------------------------------------*/
//����IP
#define LWIP_SOURCE_IP0				192
#define LWIP_SOURCE_IP1				168
#define LWIP_SOURCE_IP2				0
#define LWIP_SOURCE_IP3				111

//��������
#define LWIP_NETMASK0					255
#define LWIP_NETMASK1					255
#define LWIP_NETMASK2					0
#define LWIP_NETMASK3					0

//����
#define LWIP_GATEWAY0					192
#define LWIP_GATEWAY1					168
#define LWIP_GATEWAY2					0
#define LWIP_GATEWAY3					1

//MAC��ַ
#define LWIP_MAC0							2
#define LWIP_MAC1							0x80
#define LWIP_MAC2							0xE1
#define LWIP_MAC3							0
#define LWIP_MAC4							0
#define LWIP_MAC5							0


//Ŀ���豸IP
#define LWIP_DEST_IP0					192
#define LWIP_DEST_IP1					168
#define LWIP_DEST_IP2					0
#define LWIP_DEST_IP3					180

#define LWIP_SOURCE_PORT     (uint16_t) 8888   /* UDP�������Ӷ˿� */
#define LWIP_DEST_PORT    	 (uint16_t) 8898   /* UDPԶ�����Ӷ˿� */


#define UDP_RX_BUFSIZE		1472	//����udp���������ݳ���

#define REO_MSG_LEN 4

/* Exported functions ------------------------------------------------------- */

#pragma pack(1)
typedef struct UdpData_t{
	
    uint32_t len;
    uint8_t udp_buf[UDP_RX_BUFSIZE];
} HalUdpData_t;
#pragma pack()

/********************************************************
  * ��������: udp����
  * �������: void
  * �� �� ֵ: void
  * ˵    ��: udp���Ӳ��󶨱��ض˿ڣ��������ⲿIP
********************************************************/
void udp_echoclient_connect(void);

/********************************************************
  * ��������: udp���ͺ���
  * �������: void
  * �� �� ֵ: void
  * ˵    ��: ��udp_demo_sendbuf�ڵ����ݷ���
********************************************************/
void udp_echoclient_send(uint8_t *SendData,uint16_t size);

/********************************************************
  * ��������: udp ������
  * �������: void
  * �� �� ֵ: void
  * ˵    ��: ��
********************************************************/
void rc_sys_udp_app_poll(void);

#endif /* __UDP_ECHOCLIENT_H__ */
