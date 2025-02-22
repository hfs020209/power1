#include "main.h"
#include <string.h>
#include <stdio.h>
#include "usart_app.h"
#include "udp_echoclient.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//#define TEST_TY8512

struct udp_pcb *upcb;

uint8_t udp_demo_flag;

HalUdpData_t udp_demo_recvbuf[REO_MSG_LEN];

uint8_t g_reo_msg_st_header=0;
uint8_t g_reo_msg_st_till=0;

/* Private function prototypes -----------------------------------------------*/

void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);

#ifdef TEST_TY8512
static uint32_t udp_len2 =0;
#endif
/* Private functions ---------------------------------------------------------*/

/********************************************************
  * 函数功能: udp连接
  * 输入参数: void
  * 返 回 值: void
  * 说    明: udp连接并绑定本地端口，并连接外部IP
********************************************************/
void udp_echoclient_connect(void)
{
  ip_addr_t DestIPaddr;
  ip_addr_t SourceIPaddr;
  err_t err;
  
  /* Create a new UDP control block  */
  upcb = udp_new();
  
  if (upcb!=NULL)
  {
    /*assign destination IP address */
		IP4_ADDR( &DestIPaddr, lwipdev.remoteip[0], lwipdev.remoteip[1], lwipdev.remoteip[2], lwipdev.remoteip[3]);
    
    /* configure destination IP address and port */
    err = udp_connect(upcb, &DestIPaddr, LWIP_DEST_PORT);
		
		if (err == ERR_OK)
		{
			IP4_ADDR( &SourceIPaddr, lwipdev.ip[0], lwipdev.ip[1], lwipdev.ip[2], lwipdev.ip[3]);
			
			err = udp_bind(upcb, &SourceIPaddr, LWIP_SOURCE_PORT);
			
			
			if (err == ERR_OK)
			{
				/* Set a receive callback for the upcb */
				udp_recv(upcb, udp_receive_callback, NULL);  
				udp_demo_flag |= 1<<5;			//标记已经连接上
				UserLog("STATUS:Connected");
			}
			else
			{
				//udp_remove(upcb);
			}
		}
  }
}

/********************************************************
  * 函数功能: udp链接关闭
  * 输入参数: PCB接收数据的udp_pcb
  * 返 回 值: void
  * 说    明: udp连接断开
********************************************************/
void udp_connection_close(struct udp_pcb *upcb)
{
	udp_disconnect(upcb); 
	udp_remove(upcb);			//断开UDP连接 
	udp_demo_flag &= ~(1<<5);	//标记连接断开
	UserLog("STATUS:Diconnected");
}

/********************************************************
  * 函数功能: udp发送函数
  * 输入参数: *SendData:  发送数据
									size:  字节数
  * 返 回 值: void
  * 说    明: 将udp_demo_sendbuf内的数据发送
********************************************************/
void udp_echoclient_send(uint8_t *SendData,uint16_t size)
{
  struct pbuf *p;
	
  /* allocate pbuf from pool*/
  p = pbuf_alloc(PBUF_TRANSPORT,size, PBUF_POOL);
  
  if (p != NULL)
  {
    /* copy data to pbuf */
    pbuf_take(p, (char*)SendData, size);
    
    /* send udp data */
    udp_send(upcb, p); 
    
    /* free pbuf */
    pbuf_free(p);
  }
}

/********************************************************
  * 函数功能: udp数据接收处理
  * 输入参数: arg用户提供的参数(udp_pcb.recv_arg)
						 PCB接收数据的udp_pcb
						 p接收到的数据包缓冲区
						 addr接收到报文的远端IP地址
						 port接收数据包的远端端口
  * 返 回 值: void
  * 说    明: 无
********************************************************/
void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
  /*increment message count */
	struct pbuf *q;
 
	if(p!=NULL)	//接收到不为空的数据时
	{
		memset(udp_demo_recvbuf[g_reo_msg_st_header].udp_buf,0,UDP_RX_BUFSIZE);  //数据接收缓冲区清零
		for(q=p;q!=NULL;q=q->next)  //遍历完整个pbuf链表
		{
			//判断要拷贝到UDP_DEMO_RX_BUFSIZE中的数据是否大于UDP_DEMO_RX_BUFSIZE的剩余空间，如果大于
			//的话就只拷贝UDP_DEMO_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据
			if(q->tot_len > UDP_RX_BUFSIZE)
				break;
			else
			{
#ifdef TEST_TY8512
				udp_len2 +=q->tot_len;
#endif
				memcpy(udp_demo_recvbuf[g_reo_msg_st_header].udp_buf,q->payload,q->tot_len);
				udp_demo_recvbuf[g_reo_msg_st_header].len = q->tot_len;
				g_reo_msg_st_header++;

				if(g_reo_msg_st_header >= REO_MSG_LEN){
					g_reo_msg_st_header = 0;
				}	
			}
		}
		
		
		upcb->remote_ip = *addr; 				//记录远程主机的IP地址
		upcb->remote_port = port;  			//记录远程主机的端口号
        
    lwipdev.remoteip[0]=upcb->remote_ip.addr&0xff; 		//IADDR4
		lwipdev.remoteip[1]=(upcb->remote_ip.addr>>8)&0xff; //IADDR3
		lwipdev.remoteip[2]=(upcb->remote_ip.addr>>16)&0xff;//IADDR2
		lwipdev.remoteip[3]=(upcb->remote_ip.addr>>24)&0xff;//IADDR1 
		udp_demo_flag|=1<<6;	//标记接收到数据了
 
		pbuf_free(p);//释放内存
	}
	else
	{
		udp_disconnect(upcb); 
		udp_demo_flag &= ~(1<<5);	//标记连接断开
		UserLog("STATUS:Diconnected");
	}
}

/********************************************************
  * 函数功能: udp 任务处理
  * 输入参数: void
  * 返 回 值: void
  * 说    明: 无
********************************************************/
void rc_sys_udp_app_poll(void)
{
#ifdef TEST_TY8512
	static uint32_t udp_timer = 0;
#endif
	MX_LWIP_Process();

	if((udp_demo_flag & 1<<5)==0)
	{
		UserLog("Try reconnecting");
		udp_echoclient_connect();
	}
	
	if(udp_demo_flag&1<<6)//是否收到数据?
	{
		if(g_reo_msg_st_header != g_reo_msg_st_till)
		{
			/***********接收数据处理开始************/
			
			udp_echoclient_send(udp_demo_recvbuf[g_reo_msg_st_till].udp_buf,udp_demo_recvbuf[g_reo_msg_st_till].len);
			/***********接收数据处理结束************/
			g_reo_msg_st_till++;
			if(g_reo_msg_st_till >= REO_MSG_LEN){
				g_reo_msg_st_till = 0;
			}
		
		}
		udp_demo_flag&=~(1<<6);//标记数据已经被处理了.
	} 
	
#ifdef TEST_TY8512
		if(rc_sys_timeHasPast(udp_timer, 1000)){
			udp_timer = rc_sys_timeGet();
			UserLog("8512_len = %08d",udp_len2);
			udp_len2 = 0;
	}
#endif
}

