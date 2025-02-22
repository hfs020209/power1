#ifndef __UDP_ECHOCLIENT_H__
#define __UDP_ECHOCLIENT_H__

/* Includes ------------------------------------------------------------------*/
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip.h"

/* Exported macro ------------------------------------------------------------*/
//本机IP
#define LWIP_SOURCE_IP0				192
#define LWIP_SOURCE_IP1				168
#define LWIP_SOURCE_IP2				0
#define LWIP_SOURCE_IP3				111

//子网掩码
#define LWIP_NETMASK0					255
#define LWIP_NETMASK1					255
#define LWIP_NETMASK2					0
#define LWIP_NETMASK3					0

//网关
#define LWIP_GATEWAY0					192
#define LWIP_GATEWAY1					168
#define LWIP_GATEWAY2					0
#define LWIP_GATEWAY3					1

//MAC地址
#define LWIP_MAC0							2
#define LWIP_MAC1							0x80
#define LWIP_MAC2							0xE1
#define LWIP_MAC3							0
#define LWIP_MAC4							0
#define LWIP_MAC5							0


//目标设备IP
#define LWIP_DEST_IP0					192
#define LWIP_DEST_IP1					168
#define LWIP_DEST_IP2					0
#define LWIP_DEST_IP3					180

#define LWIP_SOURCE_PORT     (uint16_t) 8888   /* UDP本地连接端口 */
#define LWIP_DEST_PORT    	 (uint16_t) 8898   /* UDP远程连接端口 */


#define UDP_RX_BUFSIZE		1472	//定义udp最大接收数据长度

#define REO_MSG_LEN 4

/* Exported functions ------------------------------------------------------- */

#pragma pack(1)
typedef struct UdpData_t{
	
    uint32_t len;
    uint8_t udp_buf[UDP_RX_BUFSIZE];
} HalUdpData_t;
#pragma pack()

/********************************************************
  * 函数功能: udp连接
  * 输入参数: void
  * 返 回 值: void
  * 说    明: udp连接并绑定本地端口，并连接外部IP
********************************************************/
void udp_echoclient_connect(void);

/********************************************************
  * 函数功能: udp发送函数
  * 输入参数: void
  * 返 回 值: void
  * 说    明: 将udp_demo_sendbuf内的数据发送
********************************************************/
void udp_echoclient_send(uint8_t *SendData,uint16_t size);

/********************************************************
  * 函数功能: udp 任务处理
  * 输入参数: void
  * 返 回 值: void
  * 说    明: 无
********************************************************/
void rc_sys_udp_app_poll(void);

#endif /* __UDP_ECHOCLIENT_H__ */
