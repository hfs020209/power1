#include "w5500_app.h"
#include <stdio.h>
#include "socket.h"
#include "wizchip_conf.h"
#include "udp_echoclient.h"

//#define TEST_W5500

#define SOCK_TCPS        0


uint8_t DstIP[4]=DEFAULT_DEST_IP_ADDR;
uint16_t Det_port=W5500_DEST_PORT;								// 配置远程端口

HalUdpData_t udp_demo2_recvbuf[REO_MSG_LEN];

uint8_t g_reo_msg2_st_header=0;
uint8_t g_reo_msg2_st_till=0;

/********************************************************
  * 函数功能: W5500应用初始化
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 初始W5500参数，并启用
********************************************************/
void rc_eth_w5500_app_init(void)
{
	 UserLog("W5500 test on BearPi board by Mculover666");
	 
	 int ret;
	 ret = w5500_init();
	 if (ret != 0) {
		 UserLog("w5500 init fail, ret is %d", ret);
	 } else {
		 UserLog("w5500 init success");
	 }
}

/********************************************************
  * 函数功能: w5500状态打印
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 无
********************************************************/
static void rc_sys_w5500_link_status(void)
{
	static uint32_t eth_timer = 0;
	static int8_t eth_w5500_flag=0,eth_w5500_oldflag=0;

	uint32_t tmp_t = 0;
	int8_t status = 0;
	
	if(rc_sys_timeHasPast(eth_timer, W5500_TIMEOUT)){
		
		eth_timer = rc_sys_timeGet();
		
		status = ctlwizchip(CW_GET_PHYLINK, (void*)&tmp_t);
		
		if(eth_w5500_flag == 0)
		{
			if(status == 0)
			{
				if(tmp_t == 1)
				{
					eth_w5500_flag = 3;
					UserLog("ETH1 LINK UP");
				}
				else
				{
					eth_w5500_flag = 1;
					UserLog("ETH1 LINK DOWN");
				}
			}
			else
			{
				eth_w5500_flag = 2;
				UserLog("Unknown ETH1 LINK STATUS");
			}
		}
		else
		{
			if(status == 0)
			{
				if((tmp_t == 0)&&(eth_w5500_flag != 1))
				{
					eth_w5500_flag = 1;
					UserLog("ETH1 LINK DOWN");
				}
				else if((tmp_t == 1)&&(eth_w5500_flag != 3))
				{
					eth_w5500_flag = 3;
					UserLog("ETH1 LINK UP");
				}
			}
			else
			{
				if(eth_w5500_flag != 2)
				{
					eth_w5500_flag = 2;
					UserLog("Unknown ETH1 LINK STATUS");
				}
			}
		}
		
		if(eth_w5500_oldflag!=eth_w5500_flag)
		{
			eth_w5500_oldflag = eth_w5500_flag;
			
			wiz_PhyConf phyconf;
			ctlwizchip(CW_GET_PHYCONF, &phyconf);
			
			if(eth_w5500_flag == 3)
			{
				Log("ETH1 ");
				
				if(phyconf.mode == PHY_MODE_AUTONEGO)
				{
					Log("AUTO ");
				}
				else
				{
					Log("FORCE ");
				}
				
				if(phyconf.speed == PHY_SPEED_100)
				{
					Log("100M ");
				}
				else
				{
					Log("10M ");
				}
				
				if(phyconf.duplex == PHY_DUPLEX_FULL)
				{
					UserLog("FULL");
				}
				else
				{
					UserLog("HALF");
				}
			}
			
		}
	}
}

/********************************************************
  * 函数功能: udp数据接收
  * 输入参数: void
  * 返 回 值: void
  * 说    明: 无
********************************************************/
void w5500_receive_poll(void)
{
#ifdef TEST_W5500
	static uint32_t udp_timer = 0;
  static uint32_t udp_len =0;
#endif
	uint16_t len=0;
	uint8_t sock_status = getSn_SR(SOCK_TCPS);

	switch(sock_status)
	{
		case SOCK_UDP:											  // Socket处于连接建立状态
		{

			if(getSn_IR(SOCK_TCPS) & Sn_IR_RECV)   					
			{
				setSn_IR(SOCK_TCPS, Sn_IR_RECV);					// Sn_IR的CON位置1，通知W5500连接已建立
			}
			
			// 数据回环测试程序：数据从上位机服务器发给W5500，W5500接收到数据后再回给服务器
			len=getSn_RX_RSR(SOCK_TCPS);							// len=Socket0接收缓存中已接收和保存的数据大小	
			if(len>0)
			{
				if(len <= UDP_RX_BUFSIZE_T)
				{
#ifdef TEST_W5500
					udp_len+=len;
#endif
					memset(udp_demo2_recvbuf[g_reo_msg2_st_header].udp_buf,0,UDP_RX_BUFSIZE);  //数据接收缓冲区清零
					
					udp_demo2_recvbuf[g_reo_msg2_st_header].len = len-8;
					
					recvfrom(SOCK_TCPS,&udp_demo2_recvbuf[g_reo_msg2_st_header].udp_buf[0], udp_demo2_recvbuf[g_reo_msg2_st_header].len, DstIP,&Det_port);  	// W5500接收来自服务器的数据，并通过SPI发送给MCU
					
					g_reo_msg2_st_header++;

					if(g_reo_msg2_st_header >= REO_MSG_LEN){
						g_reo_msg2_st_header = 0;
					}
				}
				else
				{
					
					debug_log("receive len = %d",len);
					while(len)
					{
						if(len>(UDP_RX_BUFSIZE_T-8))
						{
							udp_demo2_recvbuf[g_reo_msg2_st_header].len = UDP_RX_BUFSIZE_T-8;
							len-=UDP_RX_BUFSIZE_T-8;
						}
						else
						{
							udp_demo2_recvbuf[g_reo_msg2_st_header].len = len-8;
							len =0;
						}
						recvfrom(SOCK_TCPS,&udp_demo2_recvbuf[g_reo_msg2_st_header].udp_buf[0], udp_demo2_recvbuf[g_reo_msg2_st_header].len, DstIP,&Det_port);  	// W5500接收来自服务器的数据，并通过SPI发送给MCU
						
						g_reo_msg2_st_header++;

						if(g_reo_msg2_st_header >= REO_MSG_LEN){
							g_reo_msg2_st_header = 0;
						}
						
					}
				}
			}												
		}break;
		case SOCK_CLOSE_WAIT:												  // Socket处于等待关闭状态
				disconnect(SOCK_TCPS);	
			break;
		case SOCK_CLOSED:                                                         			 // socket处于关闭状态
			if(socket(SOCK_TCPS,Sn_MR_UDP,W5500_SOURCE_PORT,0) == SOCK_TCPS)
			{
				UserLog("set udp socket success");
			}
			else
			{
				err_log("set udp socket fail");
			}	
			break; 
		default:
		break;  
	}
#ifdef TEST_W5500
	if(rc_sys_timeHasPast(udp_timer, 1000)){
			udp_timer = rc_sys_timeGet();
			Log("W5500_len = %08d, ",udp_len);
			udp_len = 0;
	}
#endif
	rc_sys_w5500_link_status();
}
/********************************************************
  * 函数功能: udp数据发送处理
	* 输入参数:*SendData:  发送数据
									size:  字节数
  * 返 回 值: void
  * 说    明: 无
********************************************************/
void w5500_send_work(uint8_t *SendData,uint16_t size)
{
		sendto(SOCK_TCPS,SendData,size, DstIP, Det_port);			// 接收到数据后再回给服务器，完成数据回环
}

/********************************************************
  * 函数功能: w5500任务处理
  * 输入参数: void
  * 返 回 值: 无
  * 说    明: 数据回环测试
********************************************************/
void rc_sys_w5500_app_poll(void)
{
	w5500_receive_poll();
	
	if(g_reo_msg2_st_header != g_reo_msg2_st_till)
	{
		/***********接收数据处理开始************/
		
		w5500_send_work(udp_demo2_recvbuf[g_reo_msg2_st_till].udp_buf,udp_demo2_recvbuf[g_reo_msg2_st_till].len);
		
		/***********接收数据处理结束************/
		g_reo_msg2_st_till++;
		if(g_reo_msg2_st_till >= REO_MSG_LEN){
			g_reo_msg2_st_till = 0;
		}
	
	}

}

