#include "w5500_app.h"
#include <stdio.h>
#include "socket.h"
#include "wizchip_conf.h"
#include "udp_echoclient.h"

//#define TEST_W5500

#define SOCK_TCPS        0


uint8_t DstIP[4]=DEFAULT_DEST_IP_ADDR;
uint16_t Det_port=W5500_DEST_PORT;								// ����Զ�̶˿�

HalUdpData_t udp_demo2_recvbuf[REO_MSG_LEN];

uint8_t g_reo_msg2_st_header=0;
uint8_t g_reo_msg2_st_till=0;

/********************************************************
  * ��������: W5500Ӧ�ó�ʼ��
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: ��ʼW5500������������
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
  * ��������: w5500״̬��ӡ
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: ��
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
  * ��������: udp���ݽ���
  * �������: void
  * �� �� ֵ: void
  * ˵    ��: ��
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
		case SOCK_UDP:											  // Socket�������ӽ���״̬
		{

			if(getSn_IR(SOCK_TCPS) & Sn_IR_RECV)   					
			{
				setSn_IR(SOCK_TCPS, Sn_IR_RECV);					// Sn_IR��CONλ��1��֪ͨW5500�����ѽ���
			}
			
			// ���ݻػ����Գ������ݴ���λ������������W5500��W5500���յ����ݺ��ٻظ�������
			len=getSn_RX_RSR(SOCK_TCPS);							// len=Socket0���ջ������ѽ��պͱ�������ݴ�С	
			if(len>0)
			{
				if(len <= UDP_RX_BUFSIZE_T)
				{
#ifdef TEST_W5500
					udp_len+=len;
#endif
					memset(udp_demo2_recvbuf[g_reo_msg2_st_header].udp_buf,0,UDP_RX_BUFSIZE);  //���ݽ��ջ���������
					
					udp_demo2_recvbuf[g_reo_msg2_st_header].len = len-8;
					
					recvfrom(SOCK_TCPS,&udp_demo2_recvbuf[g_reo_msg2_st_header].udp_buf[0], udp_demo2_recvbuf[g_reo_msg2_st_header].len, DstIP,&Det_port);  	// W5500�������Է����������ݣ���ͨ��SPI���͸�MCU
					
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
						recvfrom(SOCK_TCPS,&udp_demo2_recvbuf[g_reo_msg2_st_header].udp_buf[0], udp_demo2_recvbuf[g_reo_msg2_st_header].len, DstIP,&Det_port);  	// W5500�������Է����������ݣ���ͨ��SPI���͸�MCU
						
						g_reo_msg2_st_header++;

						if(g_reo_msg2_st_header >= REO_MSG_LEN){
							g_reo_msg2_st_header = 0;
						}
						
					}
				}
			}												
		}break;
		case SOCK_CLOSE_WAIT:												  // Socket���ڵȴ��ر�״̬
				disconnect(SOCK_TCPS);	
			break;
		case SOCK_CLOSED:                                                         			 // socket���ڹر�״̬
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
  * ��������: udp���ݷ��ʹ���
	* �������:*SendData:  ��������
									size:  �ֽ���
  * �� �� ֵ: void
  * ˵    ��: ��
********************************************************/
void w5500_send_work(uint8_t *SendData,uint16_t size)
{
		sendto(SOCK_TCPS,SendData,size, DstIP, Det_port);			// ���յ����ݺ��ٻظ���������������ݻػ�
}

/********************************************************
  * ��������: w5500������
  * �������: void
  * �� �� ֵ: ��
  * ˵    ��: ���ݻػ�����
********************************************************/
void rc_sys_w5500_app_poll(void)
{
	w5500_receive_poll();
	
	if(g_reo_msg2_st_header != g_reo_msg2_st_till)
	{
		/***********�������ݴ���ʼ************/
		
		w5500_send_work(udp_demo2_recvbuf[g_reo_msg2_st_till].udp_buf,udp_demo2_recvbuf[g_reo_msg2_st_till].len);
		
		/***********�������ݴ������************/
		g_reo_msg2_st_till++;
		if(g_reo_msg2_st_till >= REO_MSG_LEN){
			g_reo_msg2_st_till = 0;
		}
	
	}

}

