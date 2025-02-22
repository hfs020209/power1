#ifndef __W5500_APP_H__
#define __W5500_APP_H__

#include "sys_app.h"


/* Exported macro ------------------------------------------------------------*/


//目标设备IP
#define DEFAULT_DEST_IP_ADDR	{192,168,0,180}
//设备IP
#define DEFAULT_MAC_ADDR    {0x6C,0x4B,0x90,0xF5,0x83,0xB9}
#define DEFAULT_IP_ADDR     {192,168,0,112}
#define DEFAULT_SUB_MASK    {255,255,0,0}
#define DEFAULT_GW_ADDR     {192,168,0,1}
#define DEFAULT_DNS_ADDR    {0,0,0,0}

#define W5500_SOURCE_PORT    (uint16_t) 8889   /* UDP本地连接端口 */
#define W5500_DEST_PORT    	 (uint16_t) 8899   /* UDP远程连接端口 */

#define W5500_TIMEOUT 1000

#define UDP_RX_BUFSIZE_T		1480	//定义udp最大接收数据长度(包含UDP8字节首部字节)

/* TCP server Loopback test example */
int32_t loopback_tcps(uint8_t sn, uint8_t* buf, uint16_t port);

/* TCP client Loopback test example */
int32_t loopback_tcpc(uint8_t sn, uint8_t* buf, uint8_t* destip, uint16_t destport);

/* UDP Loopback test example */
int32_t loopback_udps(uint8_t sn, uint8_t* buf, uint16_t port);

/********************************************************
  * 函数功能: CAN过滤器初始化
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 初始CAN配置参数，并启用
********************************************************/
void rc_eth_w5500_app_init(void);

/********************************************************
  * 函数功能: udp数据发送处理
  * 输入参数: void
  * 返 回 值: void
  * 说    明: 无
********************************************************/
void w5500_send_work(uint8_t *SendData,uint16_t size);

/********************************************************
  * 函数功能: w5500接收数据中断
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: w5500接收数据中断处理
********************************************************/
void HAL_W5500_INT_Callback(void);

/********************************************************
  * 函数功能: w5500任务处理
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 数据回环测试
********************************************************/
void rc_sys_w5500_app_poll(void);
#endif
