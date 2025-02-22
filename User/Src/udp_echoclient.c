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
  * ��������: udp����
  * �������: void
  * �� �� ֵ: void
  * ˵    ��: udp���Ӳ��󶨱��ض˿ڣ��������ⲿIP
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
				udp_demo_flag |= 1<<5;			//����Ѿ�������
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
  * ��������: udp���ӹر�
  * �������: PCB�������ݵ�udp_pcb
  * �� �� ֵ: void
  * ˵    ��: udp���ӶϿ�
********************************************************/
void udp_connection_close(struct udp_pcb *upcb)
{
	udp_disconnect(upcb); 
	udp_remove(upcb);			//�Ͽ�UDP���� 
	udp_demo_flag &= ~(1<<5);	//������ӶϿ�
	UserLog("STATUS:Diconnected");
}

/********************************************************
  * ��������: udp���ͺ���
  * �������: *SendData:  ��������
									size:  �ֽ���
  * �� �� ֵ: void
  * ˵    ��: ��udp_demo_sendbuf�ڵ����ݷ���
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
  * ��������: udp���ݽ��մ���
  * �������: arg�û��ṩ�Ĳ���(udp_pcb.recv_arg)
						 PCB�������ݵ�udp_pcb
						 p���յ������ݰ�������
						 addr���յ����ĵ�Զ��IP��ַ
						 port�������ݰ���Զ�˶˿�
  * �� �� ֵ: void
  * ˵    ��: ��
********************************************************/
void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
  /*increment message count */
	struct pbuf *q;
 
	if(p!=NULL)	//���յ���Ϊ�յ�����ʱ
	{
		memset(udp_demo_recvbuf[g_reo_msg_st_header].udp_buf,0,UDP_RX_BUFSIZE);  //���ݽ��ջ���������
		for(q=p;q!=NULL;q=q->next)  //����������pbuf����
		{
			//�ж�Ҫ������UDP_DEMO_RX_BUFSIZE�е������Ƿ����UDP_DEMO_RX_BUFSIZE��ʣ��ռ䣬�������
			//�Ļ���ֻ����UDP_DEMO_RX_BUFSIZE��ʣ�೤�ȵ����ݣ�����Ļ��Ϳ������е�����
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
		
		
		upcb->remote_ip = *addr; 				//��¼Զ��������IP��ַ
		upcb->remote_port = port;  			//��¼Զ�������Ķ˿ں�
        
    lwipdev.remoteip[0]=upcb->remote_ip.addr&0xff; 		//IADDR4
		lwipdev.remoteip[1]=(upcb->remote_ip.addr>>8)&0xff; //IADDR3
		lwipdev.remoteip[2]=(upcb->remote_ip.addr>>16)&0xff;//IADDR2
		lwipdev.remoteip[3]=(upcb->remote_ip.addr>>24)&0xff;//IADDR1 
		udp_demo_flag|=1<<6;	//��ǽ��յ�������
 
		pbuf_free(p);//�ͷ��ڴ�
	}
	else
	{
		udp_disconnect(upcb); 
		udp_demo_flag &= ~(1<<5);	//������ӶϿ�
		UserLog("STATUS:Diconnected");
	}
}

/********************************************************
  * ��������: udp ������
  * �������: void
  * �� �� ֵ: void
  * ˵    ��: ��
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
	
	if(udp_demo_flag&1<<6)//�Ƿ��յ�����?
	{
		if(g_reo_msg_st_header != g_reo_msg_st_till)
		{
			/***********�������ݴ���ʼ************/
			
			udp_echoclient_send(udp_demo_recvbuf[g_reo_msg_st_till].udp_buf,udp_demo_recvbuf[g_reo_msg_st_till].len);
			/***********�������ݴ������************/
			g_reo_msg_st_till++;
			if(g_reo_msg_st_till >= REO_MSG_LEN){
				g_reo_msg_st_till = 0;
			}
		
		}
		udp_demo_flag&=~(1<<6);//��������Ѿ���������.
	} 
	
#ifdef TEST_TY8512
		if(rc_sys_timeHasPast(udp_timer, 1000)){
			udp_timer = rc_sys_timeGet();
			UserLog("8512_len = %08d",udp_len2);
			udp_len2 = 0;
	}
#endif
}

