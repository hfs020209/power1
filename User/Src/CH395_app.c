#include "CH395_app.h"
#include "CH395.h"
#include "w5500_app.h"
#include "udp_echoclient.h"

/**********************************************************************************/
/* 包含命令文件 */

#define     CH395_DEBUG                0

#define SOCK_UDPS        0

/* 常用变量定义 */
HalUdpData_t udp_demo3_recvbuf[REO_MSG_LEN];			  /* 数据缓冲区 */

uint8_t g_reo_msg3_st_header=0;
uint8_t g_reo_msg3_st_till=0;
static uint8_t send_flag=0;

uint8_t MyBuffer[UDP_RX_BUFSIZE_T];
struct _SOCK_INF SockInf;                                      /* 保存Socket信息 */
struct _CH395_SYS CH395Inf;                                    /* 保存CH395信息 */

/* CH395相关定义 */
const uint8_t CH395IPAddr[4] = {192,168,0,112};                         /* CH395IP地址 */
const uint8_t CH395GWIPAddr[4] = {192,168,0,1};                        /* CH395网关 */
const uint8_t CH395IPMask[4] = {255,255,0,0};                        /* CH395子网掩码 */

/* socket 相关定义*/
const uint8_t  BroadcastIP[4] = {192,168,0,180};                     /* UDP 广播地址 */
const uint16_t Socket0DesPort = 8899;                                  /* Socket 0目的端口 */
const uint16_t Socket0SourPort = 8889;                                 /* Socket 0源端口 */

/**********************************************************************************
* Function Name  : mStopIfError
* Description    : 调试使用，显示错误代码，并停机
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void mStopIfError(uint8_t iError)
{
    if (iError == CMD_ERR_SUCCESS) return;                           /* 操作成功 */
#if CH395_DEBUG
    err_log("Error: %02X\n", (uint16_t)iError);                         /* 显示错误 */
#endif
    while ( 1 ) 
    {
       HAL_Delay(10);
    }
}

/**********************************************************************************
* Function Name  : InitCH395InfParam
* Description    : 初始化CH395Inf参数
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void InitCH395InfParam(void)
{
    memset(&CH395Inf,0,sizeof(CH395Inf));                            /* 将CH395Inf全部清零*/
    memcpy(CH395Inf.IPAddr,CH395IPAddr,sizeof(CH395IPAddr));         /* 将IP地址写入CH395Inf中 */
    memcpy(CH395Inf.GWIPAddr,CH395GWIPAddr,sizeof(CH395GWIPAddr));   /* 将网关IP地址写入CH395Inf中 */
    memcpy(CH395Inf.MASKAddr,CH395IPMask,sizeof(CH395IPMask));       /* 将子网掩码写入CH395Inf中 */
}

/**********************************************************************************
* Function Name  : InitSocketParam
* Description    : 初始化socket
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void InitSocketParam(void)
{
    /* socket 0为UDP模式 */
    memset(&SockInf,0,sizeof(SockInf));                              /* 将SockInf[0]全部清零*/
    memcpy(SockInf.IPAddr,BroadcastIP,sizeof(BroadcastIP));          /* 如果启用UDP SERVER功能，需将目的IP设为广播地址255.255.255.255 */
    SockInf.DesPort = Socket0DesPort;                                /* 目的端口 */
    SockInf.SourPort = Socket0SourPort;                              /* 源端口 */
    SockInf.ProtoType = PROTO_TYPE_UDP;                              /* UDP模式 */
}

/**********************************************************************************
* Function Name  : CH395SocketInitOpen
* Description    : 配置CH395 socket 参数，初始化并打开socket
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void CH395SocketInitOpen(void)
{
    uint8_t i;

    /* socket 0为UDP模式 */
    CH395SetSocketDesIP(SOCK_UDPS,SockInf.IPAddr);                           /* 设置socket 0目标IP地址 */         
    CH395SetSocketProtType(SOCK_UDPS,SockInf.ProtoType);                     /* 设置socket 0协议类型 */
    CH395SetSocketDesPort(SOCK_UDPS,SockInf.DesPort);                        /* 设置socket 0目的端口 */
    CH395SetSocketSourPort(SOCK_UDPS,SockInf.SourPort);                      /* 设置socket 0源端口 */
    i = CH395OpenSocket(SOCK_UDPS);                                          /* 打开socket 0 */
    mStopIfError(i);                                                 /* 检查是否成功 */
	
		UserLog("open socket sucess");
		UserLog("ip:		%d.%d.%d.%d",SockInf.IPAddr[0],SockInf.IPAddr[1],SockInf.IPAddr[2],SockInf.IPAddr[3]);
		UserLog("type:		%d",SockInf.ProtoType);
		UserLog("Desport:	%d",SockInf.DesPort);
		UserLog("Sourport:	%d",SockInf.SourPort);
}



/********************************************************
  * 函数功能: ch395状态打印
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 无
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
  * 函数功能: udp数据发送处理
	* 输入参数:*SendData:  发送数据
									size:  字节数
  * 返 回 值: void
  * 说    明: 无
********************************************************/
void ch395_send_work(uint8_t *SendData,uint16_t size)
{
	 CH395UDPSendTo(SendData, size,&SockInf.IPAddr[0], SockInf.DesPort, SOCK_UDPS);
}

/**********************************************************************************
* Function Name  : CH395SocketInterrupt
* Description    : CH395 socket 中断,在全局中断中被调用
* Input          : sockindex
* Output         : None
* Return         : None
**********************************************************************************/
void CH395SocketInterrupt(uint8_t sockindex)
{
   uint8_t  sock_int_socket;
   uint16_t len,len_t;

   sock_int_socket = CH395GetSocketInt(sockindex);                   /* 获取socket 的中断状态 */
   if(sock_int_socket & SINT_STAT_SENBUF_FREE)                       /* 发送缓冲区空闲，可以继续写入要发送的数据 */
   {

   }
   if(sock_int_socket & SINT_STAT_SEND_OK)                           /* 发送完成中断 */
   {
		 send_flag =0;
   }
   if(sock_int_socket & SINT_STAT_RECV)                              /* 接收中断 */
   {
       len = CH395GetRecvLength(sockindex);                          /* 获取当前缓冲区内数据长度 */
#if CH395_DEBUG
       UserLog("receive len = %d",len);
#endif
       if(len == 0)return;
			 
			 while(len)
			 {
				 len_t = len;
				 if(len_t > UDP_RX_BUFSIZE)len_t = UDP_RX_BUFSIZE;                                       /* MyBuffer缓冲区长度为512*/
				 CH395GetRecvData(sockindex,len,MyBuffer);                     /* 读取数据 */

				 memset(udp_demo3_recvbuf[g_reo_msg3_st_header].udp_buf,0,UDP_RX_BUFSIZE);  //数据接收缓冲区清零
				 
				 memcpy(udp_demo3_recvbuf[g_reo_msg3_st_header].udp_buf,&MyBuffer,len_t);  //数据接收缓冲区清零
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
   **产生断开连接中断和超时中断时，CH395默认配置是内部主动关闭，用户不需要自己关闭该Socket，如果想配置成不主动关闭Socket需要配置
   **SOCK_CTRL_FLAG_SOCKET_CLOSE标志位（默认为0），如果该标志为1，CH395内部不对Socket进行关闭处理，用户在连接中断和超时中断时调用
   **CH395CloseSocket函数对Socket进行关闭，如果不关闭则该Socket一直为连接的状态（事实上已经断开），就不能再去连接了。
   */
   if(sock_int_socket & SINT_STAT_CONNECT)                           /* 连接中断，仅在TCP模式下有效*/
   {
		 UserLog("test1");
    }
   if(sock_int_socket & SINT_STAT_DISCONNECT)                        /* 断开中断，仅在TCP模式下有效 */
   {
		 UserLog("test2");
   }
   if(sock_int_socket & SINT_STAT_TIM_OUT)                           /* 超时中断，仅在TCP模式下有效 */
   {
		 UserLog("test3");
   }
}

/**********************************************************************************
* Function Name  : CH395GlobalInterrupt
* Description    : CH395全局中断函数
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void CH395GlobalInterrupt(void)
{
   uint16_t  init_status;
   uint8_t  buf[10]; 
 
    init_status = CH395CMDGetGlobIntStatus_ALL();
    if(init_status & GINT_STAT_UNREACH)                              /* 不可达中断，读取不可达信息 */
    {
      CH395CMDGetUnreachIPPT(buf); 
			err_log("消息不可达");
			UserLog("UNREACH : %d",buf[0]);
			UserLog("type : %d",buf[1]);
			UserLog("port : %d",buf[2]<<8|buf[3]);
			UserLog("ip : %d.%d.%d.%d",buf[4],buf[5],buf[6],buf[7]);
    }
    if(init_status & GINT_STAT_IP_CONFLI)                            /* 产生IP冲突中断，建议重新修改CH395的 IP，并初始化CH395*/
    {
				err_log("ip error IT");
    }
    if(init_status & GINT_STAT_PHY_CHANGE)                           /* 产生PHY改变中断*/
    {
			rc_sys_ch395_link_status();
    }
    if(init_status & GINT_STAT_SOCK0)
    {
        CH395SocketInterrupt(0);                                     /* 处理socket 0中断*/
    }
    if(init_status & GINT_STAT_SOCK1)
    {
        CH395SocketInterrupt(1);                                     /* 处理socket 1中断*/
    }
    if(init_status & GINT_STAT_SOCK2)
    {
        CH395SocketInterrupt(2);                                     /* 处理socket 2中断*/
    }
    if(init_status & GINT_STAT_SOCK3)
    {
        CH395SocketInterrupt(3);                                     /* 处理socket 3中断*/
    }
    if(init_status & GINT_STAT_SOCK4)
    {
        CH395SocketInterrupt(4);                                     /* 处理socket 4中断*/
    }
    if(init_status & GINT_STAT_SOCK5)                                
    {
        CH395SocketInterrupt(5);                                     /* 处理socket 5中断*/
    }
    if(init_status & GINT_STAT_SOCK6)                                
    {
        CH395SocketInterrupt(6);                                     /* 处理socket 6中断*/
    }
    if(init_status & GINT_STAT_SOCK7)                                
    {
        CH395SocketInterrupt(7);                                     /* 处理socket 7中断*/
    }
}

/***********************************************************************************
* Function Name  : CH395Init
* Description    : 配置CH395的IP,GWIP,MAC等参数，并初始化
* Input          : None
* Output         : None
* Return         : 函数执行结果
**********************************************************************************/
uint8_t  rc_eth_ch395_app_init(void)
{
    uint8_t i;
	
    InitCH395InfParam();  
                                           /* 初始化CH395相关变量 */
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
			return CH395_ERR_UNKNOW;                            /* 测试命令，如果无法通过返回0XFA */
                                                                     /* 返回0XFA一般为硬件错误或者读写时序不对 */
		
    CH395CMDSetIPAddr(CH395Inf.IPAddr);                              /* 设置CH395的IP地址 */
    CH395CMDSetGWIPAddr(CH395Inf.GWIPAddr);                          /* 设置网关地址 */
    CH395CMDSetMASKAddr(CH395Inf.MASKAddr);                          /* 设置子网掩码，默认为255.255.255.0*/ 
    HAL_Delay(10);  
    i = CH395CMDInitCH395();                                         /* 初始化CH395芯片 */
	
		mStopIfError(i);
		rc_sys_ch395_link_status();
    return i;
}
/********************************************************
  * 函数功能: sock 状态查询
  * 输入参数: void
  * 返 回 值: void
  * 说    明: 无
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
			case 0:											  // Socket处于关闭状态
			{
				InitSocketParam();                                                /* 初始化socket相关变量 */
				CH395SocketInitOpen();
				Flag_Status = 1;
			}break;
			case 5:												  // Socket处于打开状态
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
* Description    : 配置CH395的IP,GWIP,MAC等参数，并初始化
* Input          : None
* Output         : None
* Return         : 函数执行结果
**********************************************************************************/
void rc_sys_ch395_app_poll(void)
{
	ch395_sock_status();
	CH395GlobalInterrupt();
	
	if((send_flag == 0) && (g_reo_msg3_st_header != g_reo_msg3_st_till))
	{
		/***********接收数据处理开始************/
		send_flag =1;
		ch395_send_work(udp_demo3_recvbuf[g_reo_msg3_st_till].udp_buf,udp_demo3_recvbuf[g_reo_msg3_st_till].len);
		
		/***********接收数据处理结束************/
		g_reo_msg3_st_till++;
		if(g_reo_msg3_st_till >= REO_MSG_LEN){
			g_reo_msg3_st_till = 0;
		}
	
	}
}


