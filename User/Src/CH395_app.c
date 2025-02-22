#include "CH395_app.h"
#include "CH395.h"
#include "w5500_app.h"
#include "udp_echoclient.h"

/**********************************************************************************/
/* ���������ļ� */

#define     CH395_DEBUG                0

#define SOCK_UDPS        0

/* ���ñ������� */
HalUdpData_t udp_demo3_recvbuf[REO_MSG_LEN];			  /* ���ݻ����� */

uint8_t g_reo_msg3_st_header=0;
uint8_t g_reo_msg3_st_till=0;
static uint8_t send_flag=0;

uint8_t MyBuffer[UDP_RX_BUFSIZE_T];
struct _SOCK_INF SockInf;                                      /* ����Socket��Ϣ */
struct _CH395_SYS CH395Inf;                                    /* ����CH395��Ϣ */

/* CH395��ض��� */
const uint8_t CH395IPAddr[4] = {192,168,0,112};                         /* CH395IP��ַ */
const uint8_t CH395GWIPAddr[4] = {192,168,0,1};                        /* CH395���� */
const uint8_t CH395IPMask[4] = {255,255,0,0};                        /* CH395�������� */

/* socket ��ض���*/
const uint8_t  BroadcastIP[4] = {192,168,0,180};                     /* UDP �㲥��ַ */
const uint16_t Socket0DesPort = 8899;                                  /* Socket 0Ŀ�Ķ˿� */
const uint16_t Socket0SourPort = 8889;                                 /* Socket 0Դ�˿� */

/**********************************************************************************
* Function Name  : mStopIfError
* Description    : ����ʹ�ã���ʾ������룬��ͣ��
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void mStopIfError(uint8_t iError)
{
    if (iError == CMD_ERR_SUCCESS) return;                           /* �����ɹ� */
#if CH395_DEBUG
    err_log("Error: %02X\n", (uint16_t)iError);                         /* ��ʾ���� */
#endif
    while ( 1 ) 
    {
       HAL_Delay(10);
    }
}

/**********************************************************************************
* Function Name  : InitCH395InfParam
* Description    : ��ʼ��CH395Inf����
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void InitCH395InfParam(void)
{
    memset(&CH395Inf,0,sizeof(CH395Inf));                            /* ��CH395Infȫ������*/
    memcpy(CH395Inf.IPAddr,CH395IPAddr,sizeof(CH395IPAddr));         /* ��IP��ַд��CH395Inf�� */
    memcpy(CH395Inf.GWIPAddr,CH395GWIPAddr,sizeof(CH395GWIPAddr));   /* ������IP��ַд��CH395Inf�� */
    memcpy(CH395Inf.MASKAddr,CH395IPMask,sizeof(CH395IPMask));       /* ����������д��CH395Inf�� */
}

/**********************************************************************************
* Function Name  : InitSocketParam
* Description    : ��ʼ��socket
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void InitSocketParam(void)
{
    /* socket 0ΪUDPģʽ */
    memset(&SockInf,0,sizeof(SockInf));                              /* ��SockInf[0]ȫ������*/
    memcpy(SockInf.IPAddr,BroadcastIP,sizeof(BroadcastIP));          /* �������UDP SERVER���ܣ��轫Ŀ��IP��Ϊ�㲥��ַ255.255.255.255 */
    SockInf.DesPort = Socket0DesPort;                                /* Ŀ�Ķ˿� */
    SockInf.SourPort = Socket0SourPort;                              /* Դ�˿� */
    SockInf.ProtoType = PROTO_TYPE_UDP;                              /* UDPģʽ */
}

/**********************************************************************************
* Function Name  : CH395SocketInitOpen
* Description    : ����CH395 socket ��������ʼ������socket
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void CH395SocketInitOpen(void)
{
    uint8_t i;

    /* socket 0ΪUDPģʽ */
    CH395SetSocketDesIP(SOCK_UDPS,SockInf.IPAddr);                           /* ����socket 0Ŀ��IP��ַ */         
    CH395SetSocketProtType(SOCK_UDPS,SockInf.ProtoType);                     /* ����socket 0Э������ */
    CH395SetSocketDesPort(SOCK_UDPS,SockInf.DesPort);                        /* ����socket 0Ŀ�Ķ˿� */
    CH395SetSocketSourPort(SOCK_UDPS,SockInf.SourPort);                      /* ����socket 0Դ�˿� */
    i = CH395OpenSocket(SOCK_UDPS);                                          /* ��socket 0 */
    mStopIfError(i);                                                 /* ����Ƿ�ɹ� */
	
		UserLog("open socket sucess");
		UserLog("ip:		%d.%d.%d.%d",SockInf.IPAddr[0],SockInf.IPAddr[1],SockInf.IPAddr[2],SockInf.IPAddr[3]);
		UserLog("type:		%d",SockInf.ProtoType);
		UserLog("Desport:	%d",SockInf.DesPort);
		UserLog("Sourport:	%d",SockInf.SourPort);
}



/********************************************************
  * ��������: ch395״̬��ӡ
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: ��
********************************************************/
static void rc_sys_ch395_link_status(void)
{
	static uint8_t eth_ch395_flag = 0,status_old = 0;
	uint8_t status = 0;
	
	status = CH395CMDGetPHYStatus();
	
	if(status_old != status)
	{
		status_old = status;
		if((eth_ch395_flag != 2)&&(status != PHY_DISCONN))
		{
			eth_ch395_flag = 2;
			UserLog("ETH1 LINK UP");
		}
		
		switch(status)
		{
			case PHY_DISCONN:
				eth_ch395_flag = 1;
				UserLog("ETH1 LINK DOWN");
				break;
			case PHY_10M_FLL:
				UserLog("ETH1 AUTO 10M FULL");
				break;
			case PHY_10M_HALF:
				UserLog("ETH1 AUTO 10M HALF");
				break;
			case PHY_100M_FLL:
				UserLog("ETH1 AUTO 100M FULL");
				break;
			case PHY_100M_HALF:
				UserLog("ETH1 AUTO 100M HALF");
				break;
			default:
				UserLog("Unknown ETH1 LINK STATUS");
			break;  
		}
	}
}
/********************************************************
  * ��������: udp���ݷ��ʹ���
	* �������:*SendData:  ��������
									size:  �ֽ���
  * �� �� ֵ: void
  * ˵    ��: ��
********************************************************/
void ch395_send_work(uint8_t *SendData,uint16_t size)
{
	 CH395UDPSendTo(SendData, size,&SockInf.IPAddr[0], SockInf.DesPort, SOCK_UDPS);
}

/**********************************************************************************
* Function Name  : CH395SocketInterrupt
* Description    : CH395 socket �ж�,��ȫ���ж��б�����
* Input          : sockindex
* Output         : None
* Return         : None
**********************************************************************************/
void CH395SocketInterrupt(uint8_t sockindex)
{
   uint8_t  sock_int_socket;
   uint16_t len,len_t;

   sock_int_socket = CH395GetSocketInt(sockindex);                   /* ��ȡsocket ���ж�״̬ */
   if(sock_int_socket & SINT_STAT_SENBUF_FREE)                       /* ���ͻ��������У����Լ���д��Ҫ���͵����� */
   {

   }
   if(sock_int_socket & SINT_STAT_SEND_OK)                           /* ��������ж� */
   {
		 send_flag =0;
   }
   if(sock_int_socket & SINT_STAT_RECV)                              /* �����ж� */
   {
       len = CH395GetRecvLength(sockindex);                          /* ��ȡ��ǰ�����������ݳ��� */
#if CH395_DEBUG
       UserLog("receive len = %d",len);
#endif
       if(len == 0)return;
			 
			 while(len)
			 {
				 len_t = len;
				 if(len_t > UDP_RX_BUFSIZE)len_t = UDP_RX_BUFSIZE;                                       /* MyBuffer����������Ϊ512*/
				 CH395GetRecvData(sockindex,len,MyBuffer);                     /* ��ȡ���� */

				 memset(udp_demo3_recvbuf[g_reo_msg3_st_header].udp_buf,0,UDP_RX_BUFSIZE);  //���ݽ��ջ���������
				 
				 memcpy(udp_demo3_recvbuf[g_reo_msg3_st_header].udp_buf,&MyBuffer,len_t);  //���ݽ��ջ���������
				 udp_demo3_recvbuf[g_reo_msg3_st_header].len=len_t;
			 
				 g_reo_msg3_st_header++;

				 if(g_reo_msg3_st_header >= REO_MSG_LEN){
					 g_reo_msg3_st_header = 0;
				 }
				 
				 len -= len_t;
				
				
			 }
			 return;  			 
   }
   /*
   **�����Ͽ������жϺͳ�ʱ�ж�ʱ��CH395Ĭ���������ڲ������رգ��û�����Ҫ�Լ��رո�Socket����������óɲ������ر�Socket��Ҫ����
   **SOCK_CTRL_FLAG_SOCKET_CLOSE��־λ��Ĭ��Ϊ0��������ñ�־Ϊ1��CH395�ڲ�����Socket���йرմ����û��������жϺͳ�ʱ�ж�ʱ����
   **CH395CloseSocket������Socket���йرգ�������ر����SocketһֱΪ���ӵ�״̬����ʵ���Ѿ��Ͽ������Ͳ�����ȥ�����ˡ�
   */
   if(sock_int_socket & SINT_STAT_CONNECT)                           /* �����жϣ�����TCPģʽ����Ч*/
   {
		 UserLog("test1");
    }
   if(sock_int_socket & SINT_STAT_DISCONNECT)                        /* �Ͽ��жϣ�����TCPģʽ����Ч */
   {
		 UserLog("test2");
   }
   if(sock_int_socket & SINT_STAT_TIM_OUT)                           /* ��ʱ�жϣ�����TCPģʽ����Ч */
   {
		 UserLog("test3");
   }
}

/**********************************************************************************
* Function Name  : CH395GlobalInterrupt
* Description    : CH395ȫ���жϺ���
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void CH395GlobalInterrupt(void)
{
   uint16_t  init_status;
   uint8_t  buf[10]; 
 
    init_status = CH395CMDGetGlobIntStatus_ALL();
    if(init_status & GINT_STAT_UNREACH)                              /* ���ɴ��жϣ���ȡ���ɴ���Ϣ */
    {
      CH395CMDGetUnreachIPPT(buf); 
			err_log("��Ϣ���ɴ�");
			UserLog("UNREACH : %d",buf[0]);
			UserLog("type : %d",buf[1]);
			UserLog("port : %d",buf[2]<<8|buf[3]);
			UserLog("ip : %d.%d.%d.%d",buf[4],buf[5],buf[6],buf[7]);
    }
    if(init_status & GINT_STAT_IP_CONFLI)                            /* ����IP��ͻ�жϣ����������޸�CH395�� IP������ʼ��CH395*/
    {
				err_log("ip error IT");
    }
    if(init_status & GINT_STAT_PHY_CHANGE)                           /* ����PHY�ı��ж�*/
    {
			rc_sys_ch395_link_status();
    }
    if(init_status & GINT_STAT_SOCK0)
    {
        CH395SocketInterrupt(0);                                     /* ����socket 0�ж�*/
    }
    if(init_status & GINT_STAT_SOCK1)
    {
        CH395SocketInterrupt(1);                                     /* ����socket 1�ж�*/
    }
    if(init_status & GINT_STAT_SOCK2)
    {
        CH395SocketInterrupt(2);                                     /* ����socket 2�ж�*/
    }
    if(init_status & GINT_STAT_SOCK3)
    {
        CH395SocketInterrupt(3);                                     /* ����socket 3�ж�*/
    }
    if(init_status & GINT_STAT_SOCK4)
    {
        CH395SocketInterrupt(4);                                     /* ����socket 4�ж�*/
    }
    if(init_status & GINT_STAT_SOCK5)                                
    {
        CH395SocketInterrupt(5);                                     /* ����socket 5�ж�*/
    }
    if(init_status & GINT_STAT_SOCK6)                                
    {
        CH395SocketInterrupt(6);                                     /* ����socket 6�ж�*/
    }
    if(init_status & GINT_STAT_SOCK7)                                
    {
        CH395SocketInterrupt(7);                                     /* ����socket 7�ж�*/
    }
}

/***********************************************************************************
* Function Name  : CH395Init
* Description    : ����CH395��IP,GWIP,MAC�Ȳ���������ʼ��
* Input          : None
* Output         : None
* Return         : ����ִ�н��
**********************************************************************************/
uint8_t  rc_eth_ch395_app_init(void)
{
    uint8_t i;
	
    InitCH395InfParam();  
                                           /* ��ʼ��CH395��ر��� */
		i =CH395CMDGetVer();
		if((i&0xc0)==0x40)
		{
			UserLog("CH395 VER = 0x%X",i&0x3f);
		}
		else
		{
			err_log("CH395 VER ERROR 0x%X",i);
		}
    i = CH395CMDCheckExist(0x65);                      
    if(i != 0x9a)
			return CH395_ERR_UNKNOW;                            /* �����������޷�ͨ������0XFA */
                                                                     /* ����0XFAһ��ΪӲ��������߶�дʱ�򲻶� */
		
    CH395CMDSetIPAddr(CH395Inf.IPAddr);                              /* ����CH395��IP��ַ */
    CH395CMDSetGWIPAddr(CH395Inf.GWIPAddr);                          /* �������ص�ַ */
    CH395CMDSetMASKAddr(CH395Inf.MASKAddr);                          /* �����������룬Ĭ��Ϊ255.255.255.0*/ 
    HAL_Delay(10);  
    i = CH395CMDInitCH395();                                         /* ��ʼ��CH395оƬ */
	
		mStopIfError(i);
		rc_sys_ch395_link_status();
    return i;
}
/********************************************************
  * ��������: sock ״̬��ѯ
  * �������: void
  * �� �� ֵ: void
  * ˵    ��: ��
********************************************************/
void ch395_sock_status(void)
{
	static uint32_t eth_timer = 0;
	static uint8_t Flag_Status=0;
	uint8_t sock_status[2] = {0};
	
	if(rc_sys_timeHasPast(eth_timer, CH395_TIMEOUT)){
		
		eth_timer = rc_sys_timeGet();
		CH395CMDGetSocketStatus(SOCK_UDPS,sock_status);

		switch(sock_status[0])
		{
			case 0:											  // Socket���ڹر�״̬
			{
				InitSocketParam();                                                /* ��ʼ��socket��ر��� */
				CH395SocketInitOpen();
				Flag_Status = 1;
			}break;
			case 5:												  // Socket���ڴ�״̬
				if(Flag_Status == 0)
				{
					CH395CloseSocket(SOCK_UDPS);
				}
				break;
			default:
			break;  
		}
	}
}

/***********************************************************************************
* Function Name  : rc_sys_ch395_app_poll
* Description    : ����CH395��IP,GWIP,MAC�Ȳ���������ʼ��
* Input          : None
* Output         : None
* Return         : ����ִ�н��
**********************************************************************************/
void rc_sys_ch395_app_poll(void)
{
	ch395_sock_status();
	CH395GlobalInterrupt();
	
	if((send_flag == 0) && (g_reo_msg3_st_header != g_reo_msg3_st_till))
	{
		/***********�������ݴ���ʼ************/
		send_flag =1;
		ch395_send_work(udp_demo3_recvbuf[g_reo_msg3_st_till].udp_buf,udp_demo3_recvbuf[g_reo_msg3_st_till].len);
		
		/***********�������ݴ������************/
		g_reo_msg3_st_till++;
		if(g_reo_msg3_st_till >= REO_MSG_LEN){
			g_reo_msg3_st_till = 0;
		}
	
	}
}


