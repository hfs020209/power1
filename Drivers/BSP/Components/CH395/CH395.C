/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395功能演示
**********************************************************************************/

/**********************************************************************************
CH395 TCP/IP 协议族接口
MSC51 演示程序，用于演示Socket0工作在UDP模式下，单片机收到数据后回传 。
上传。MCS51@24MHZ,KEIL 3.51
**********************************************************************************/
/* 头文件包含*/
#include "CH395.h"

#define xCH395CmdStart( )         HAL_GPIO_WritePin(W5500_CS_PORT, W5500_CS_PIN, GPIO_PIN_RESET);
#define xEndCH395Cmd()            HAL_GPIO_WritePin(W5500_CS_PORT, W5500_CS_PIN, GPIO_PIN_SET);


uint8_t xCH395_SPI_DMA_READ_Byte(void) 
{
	uint8_t RxData;
	HAL_SPI_Receive_DMA(&hspi2, &RxData, 1);
    while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY) {
  }
	return RxData;
}
	
void xCH395_SPI_DMA_WRITE_Byte(uint8_t TxData)
{
	HAL_SPI_Transmit_DMA(&hspi2, &TxData, 1);
    while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY) {
  }
}

void xCH395_SPI_DMA_READ(uint8_t* RxData, uint16_t NumByteToWrite)   
{
	HAL_SPI_Receive_DMA(&hspi2, RxData, NumByteToWrite);
		while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY) {
  }
} 

void xCH395_SPI_DMA_WRITE(uint8_t* TxData, uint16_t NumByteToWrite)   
{
	HAL_SPI_Transmit_DMA(&hspi2, TxData, NumByteToWrite);
  	while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY) {
  }
}

/********************************************************************************
* Function Name  : CH395CMDReset
* Description    : 复位CH395芯片
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDReset(void)
{
	xCH395CmdStart();
  xCH395_SPI_DMA_WRITE_Byte(CMD00_RESET_ALL);
  xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395CMDSleep
* Description    : 使CH395进入睡眠状态
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSleep(void)
{
	xCH395CmdStart();
  xCH395_SPI_DMA_WRITE_Byte(CMD00_ENTER_SLEEP);
  xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSleep
* Description    : 获取芯片以及固件版本号，1字节，高四位表示芯片版本，
                   低四位表示固件版本
* Input          : None
* Output         : None
* Return         : 1字节芯片及固件版本号
*******************************************************************************/
uint8_t CH395CMDGetVer(void)
{
	uint8_t i=0;
	xCH395CmdStart();
	xCH395_SPI_DMA_WRITE_Byte(CMD01_GET_IC_VER);
  i = xCH395_SPI_DMA_READ_Byte();
  xEndCH395Cmd();
  return i;
}

/********************************************************************************
* Function Name  : CH395CMDCheckExist
* Description    : 测试命令，用于测试硬件以及接口通讯
* Input          : testdata 1字节测试数据
* Output         : None
* Return         : 硬件OK，返回 testdata按位取反
*******************************************************************************/
uint8_t CH395CMDCheckExist(uint8_t testdata)
{
	uint8_t i=0;
	uint8_t data[2]={CMD11_CHECK_EXIST,testdata};
	
	xCH395CmdStart();
	xCH395_SPI_DMA_WRITE(data,2);
	i = xCH395_SPI_DMA_READ_Byte();
	xEndCH395Cmd();
	return i;
}

/********************************************************************************
* Function Name  : CH395CMDSetPHY
* Description    : 设置PHY，主要设置CH395 PHY为100/10M 或者全双工半双工，CH395默
                    为自动协商。
* Input          : phystat 参考PHY 命令参数/状态
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetPHY(uint8_t phystat)
{
	uint8_t data[2]={CMD10_SET_PHY,phystat};
	
	xCH395CmdStart();
	xCH395_SPI_DMA_WRITE(data,2);
	xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395CMDGetPHYStatus
* Description    : 获取PHY的状态
* Input          : None
* Output         : None
* Return         : 当前CH395PHY状态，参考PHY参数/状态定义
*******************************************************************************/
uint8_t CH395CMDGetPHYStatus(void)
{
  uint8_t i=0;
  xCH395CmdStart();
	xCH395_SPI_DMA_WRITE_Byte(CMD01_GET_PHY_STATUS);
	i = xCH395_SPI_DMA_READ_Byte();
  xEndCH395Cmd();
  return i;
}

/*******************************************************************************
* Function Name  : CH395CMDGetGlobIntStatus
* Description    : 获取全局中断状态，收到此命令CH395自动取消中断，0x43及以下版本使用
* Input          : None
* Output         : None
* Return         : 返回当前的全局中断状态
*******************************************************************************/
uint8_t CH395CMDGetGlobIntStatus(void)
{
	uint8_t init_status=0;

	xCH395CmdStart();
	xCH395_SPI_DMA_WRITE_Byte(CMD01_GET_GLOB_INT_STATUS);
	init_status = xCH395_SPI_DMA_READ_Byte();
	xEndCH395Cmd();
	return  init_status;
}

/********************************************************************************
* Function Name  : CH395CMDSetUartBaudRate
* Description    : 设置CH395串口波特率，仅在串口模式下有效
* Input          : baudrate 串口波特率
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetUartBaudRate(uint32_t baudrate)
{
	uint8_t data[4]={0};
	data[0] = (uint8_t)CMD31_SET_BAUDRATE;
	data[1] = (uint8_t)baudrate;
	data[2] = (uint8_t)((uint16_t)baudrate >> 8);
	data[3] = (uint8_t)((uint16_t)baudrate >> 16);
	
	xCH395CmdStart();
	xCH395_SPI_DMA_WRITE(data,4);
//	i = xCH395_SPI_DMA_READ_Byte();
  xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395GetCmdStatus
* Description    : 获取命令执行状态，某些命令需要等待命令执行结果
* Input          : None
* Output         : None
* Return         : 返回上一条命令执行状态
*******************************************************************************/
uint8_t CH395GetCmdStatus(void)
{
    uint8_t i=0;

    xCH395CmdStart();
		xCH395_SPI_DMA_WRITE_Byte(CMD01_GET_CMD_STATUS);
		i = xCH395_SPI_DMA_READ_Byte();
		xEndCH395Cmd();
    return i;
}

/********************************************************************************
* Function Name  : CH395CMDSetIPAddr
* Description    : 设置CH395的IP地址
* Input          : ipaddr 指IP地址
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetIPAddr(uint8_t *ipaddr)
{
		uint8_t data[5]={0};
		data[0] = CMD40_SET_IP_ADDR;
		memcpy(&data[1],ipaddr,4);
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,5);
		xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSetGWIPAddr
* Description    : 设置CH395的网关IP地址
* Input          : ipaddr 指向网关IP地址
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetGWIPAddr(uint8_t *gwipaddr)
{
		uint8_t data[5]={0};
		data[0] = CMD40_SET_GWIP_ADDR;
		memcpy(&data[1],gwipaddr,4);
		
    xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,5);
		xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSetMASKAddr
* Description    : 设置CH395的子网掩码，默认为255.255.255.0
* Input          : maskaddr 指子网掩码地址
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetMASKAddr(uint8_t *maskaddr)
{
		uint8_t data[5]={0};
		data[0] = CMD40_SET_MASK_ADDR;
		memcpy(&data[1],maskaddr,4);
		
    xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,5);
		xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSetMACAddr
* Description    : 设置CH395的MAC地址。
* Input          : amcaddr MAC地址指针
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetMACAddr(uint8_t *amcaddr)
{
		uint8_t data[5]={0};
		data[0] = CMD60_SET_MAC_ADDR;
		memcpy(&data[1],amcaddr,4);
		
    xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,5);
		xEndCH395Cmd();
    HAL_Delay(100); 
}

/********************************************************************************
* Function Name  : CH395CMDGetMACAddr
* Description    : 获取CH395的MAC地址。
* Input          : amcaddr MAC地址指针
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDGetMACAddr(uint8_t *amcaddr)
{
    xCH395CmdStart();
		xCH395_SPI_DMA_WRITE_Byte(CMD06_GET_MAC_ADDR);
		xCH395_SPI_DMA_READ(amcaddr,4);
		xEndCH395Cmd();
 }

/*******************************************************************************
* Function Name  : CH395CMDSetMACFilt
* Description    : 设置MAC过滤。
* Input          : filtype 参考 MAC过滤
                   table0 Hash0
                   table1 Hash1
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetMACFilt(uint8_t filtype,uint32_t table0,uint32_t table1)
{
		uint8_t data[10]={0};
		data[0] = CMD90_SET_MAC_FILT;
		data[1] = filtype;
		data[2] = (uint8_t)table0;
		data[3] = (uint8_t)((uint16_t)table0 >> 8);
		data[4] = (uint8_t)((uint16_t)table0 >> 16);
		data[5] = (uint8_t)((uint16_t)table0 >> 24);
		data[6] = (uint8_t)table1;
		data[7] = (uint8_t)((uint16_t)table1 >> 8);
		data[8] = (uint8_t)((uint16_t)table1 >> 16);
		data[9] = (uint8_t)((uint16_t)table1 >> 24);
	
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,10);
		xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDGetUnreachIPPT
* Description    : 获取不可达信息 (IP,Port,Protocol Type)
* Input          : list 保存获取到的不可达
                        第1个字节为不可达代码，请参考 不可达代码(CH395INC.H)
                        第2个字节为IP包协议类型
                        第3-4字节为端口号
                        第4-8字节为IP地址
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDGetUnreachIPPT(uint8_t *list)
{
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE_Byte(CMD08_GET_UNREACH_IPPORT);
		xCH395_SPI_DMA_READ(list,4);
		xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDGetRemoteIPP
* Description    : 获取远端的IP和端口地址，一般在TCP Server模式下使用
* Input          : sockindex Socket索引
                   list 保存IP和端口
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDGetRemoteIPP(uint8_t sockindex,uint8_t *list)
{
		uint8_t data[2]={CMD06_GET_REMOT_IPP_SN, sockindex};

		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,2);
		xCH395_SPI_DMA_READ(list,6);
		xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395SetSocketDesIP
* Description    : 设置socket n的目的IP地址
* Input          : sockindex Socket索引
                   ipaddr 指向IP地址
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketDesIP(uint8_t sockindex,uint8_t *ipaddr)
{
		uint8_t data[6]={0};
		data[0] = CMD50_SET_IP_ADDR_SN;
		data[1] = sockindex;
		memcpy(&data[2],ipaddr,4);
		
    xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,6);
		xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395SetSocketProtType
* Description    : 设置socket 的协议类型
* Input          : sockindex Socket索引
                   prottype 协议类型，请参考 socket协议类型定义(CH395INC.H)
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketProtType(uint8_t sockindex,uint8_t prottype)
{
		uint8_t data[3]={0};
		data[0] = CMD20_SET_PROTO_TYPE_SN;
		data[1] = sockindex;
		data[2] = prottype;
		
    xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,3);
		xEndCH395Cmd();
}

/*******************************************************************************

* Function Name  : CH395SetSocketDesPort
* Description    : 设置socket n的协议类型
* Input          : sockindex Socket索引
                   desprot 2字节目的端口
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketDesPort(uint8_t sockindex,uint16_t desprot)
{
		uint8_t data[4]={0};
		data[0] = CMD30_SET_DES_PORT_SN;
		data[1] = sockindex;
		data[2] = (uint8_t)desprot;
		data[3] = (uint8_t)(desprot >> 8);
		
    xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,4);
		xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395SetSocketSourPort
* Description    : 设置socket n的协议类型
* Input          : sockindex Socket索引
                   desprot 2字节源端口
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketSourPort(uint8_t sockindex,uint16_t surprot)
{
		uint8_t data[4]={0};
		data[0] = CMD30_SET_SOUR_PORT_SN;
		data[1] = sockindex;
		data[2] = (uint8_t)surprot;
		data[3] = (uint8_t)(surprot >> 8);
		
    xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,4);
		xEndCH395Cmd();
}

/******************************************************************************
* Function Name  : CH395SetSocketIPRAWProto
* Description    : IP模式下，socket IP包协议字段
* Input          : sockindex Socket索引
                   prototype IPRAW模式1字节协议字段
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketIPRAWProto(uint8_t sockindex,uint8_t prototype)
{
		uint8_t data[3]={0};
		data[0] = CMD20_SET_IPRAW_PRO_SN;
		data[1] = sockindex;
		data[2] = prototype;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,3);
		xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395EnablePing
* Description    : 开启/关闭 PING
* Input          : enable : 1  开启PING
                          ：0  关闭PING
* Output         : None
* Return         : None
*******************************************************************************/
void CH395EnablePing(uint8_t enable)
{
		uint8_t data[2]={0};
		data[0] = CMD01_PING_ENABLE;
		data[1] = enable;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,2);
		xEndCH395Cmd();
}


/********************************************************************************
* Function Name  : CH395SendData
* Description    : 向发送缓冲区写数据
* Input          : sockindex Socket索引
                   databuf  数据缓冲区
                   len   长度
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SendData(uint8_t sockindex,uint8_t *databuf,uint16_t len)
{
		uint8_t data[4]={0};
		data[0] = CMD30_WRITE_SEND_BUF_SN;
		data[1] = sockindex;
		data[2] = (uint8_t)len;
		data[3] = (uint8_t)(len>>8);
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,4);
		xCH395_SPI_DMA_WRITE(databuf,len);
		xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395GetRecvLength
* Description    : 获取接收缓冲区长度
* Input          : sockindex Socket索引
* Output         : None
* Return         : 返回接收缓冲区有效长度
*******************************************************************************/
uint16_t CH395GetRecvLength(uint8_t sockindex)
{
    uint16_t i;
		uint8_t data[2]={0};
		data[0] = CMD12_GET_RECV_LEN_SN;
		data[1] = sockindex;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,2);
		xCH395_SPI_DMA_READ((uint8_t*)&i,2);
		xEndCH395Cmd();
		
    return i;
}

/*******************************************************************************
* Function Name  : CH395ClearRecvBuf
* Description    : 清除接收缓冲区
* Input          : sockindex Socket索引
* Output         : None
* Return         : None
*******************************************************************************/
void CH395ClearRecvBuf(uint8_t sockindex)
{
		uint8_t data[2]={0};
		data[0] = CMD10_CLEAR_RECV_BUF_SN;
		data[1] = sockindex;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,2);
		xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395GetRecvLength
* Description    : 读取接收缓冲区数据
* Input          : sockindex Socket索引
                   len   长度
                   pbuf  缓冲区
* Output         : None
* Return         : None
*******************************************************************************/
void CH395GetRecvData(uint8_t sockindex,uint16_t len,uint8_t *pbuf)
{
    if(!len)return;
	
		uint8_t data[4]={0};
		data[0] = CMD30_READ_RECV_BUF_SN;
		data[1] = sockindex;
		data[2] = (uint8_t)len;
		data[3] = (uint8_t)(len>>8);
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,4);
		xCH395_SPI_DMA_READ(pbuf,len);
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSetRetryCount
* Description    : 设置重试次数
* Input          : count 重试值，最大为20次
* Output         : None
* Return         : None
********************************************************************************/
void CH395CMDSetRetryCount(uint8_t count)
{
		uint8_t data[2]={0};
		data[0] = CMD10_SET_RETRAN_COUNT;
		data[1] = count;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,2);
		xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSetRetryPeriod
* Description    : 设置重试周期
* Input          : period 重试周期单位为毫秒，最大1000ms
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetRetryPeriod(uint16_t period)
{
		uint8_t data[3]={0};
		data[0] = CMD20_SET_RETRAN_PERIOD;
		data[1] = (uint8_t)period;
		data[2] = (uint8_t)(period>>8);
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,3);
		xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDGetSocketStatus
* Description    : 获取socket
* Input          : None
* Output         : socket n的状态信息，第1字节为socket 打开或者关闭
                   第2字节为TCP状态
* Return         : None
*******************************************************************************/
void CH395CMDGetSocketStatus(uint8_t sockindex,uint8_t *status)
{
		uint8_t data[2]={0};
		data[0] = CMD12_GET_SOCKET_STATUS_SN;
		data[1] = sockindex;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,2);
		xCH395_SPI_DMA_READ(status,2);
		xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDInitCH395
* Description    : 初始化CH395芯片。
* Input          : None
* Output         : None
* Return         : 返回执行结果
*******************************************************************************/
uint8_t CH395CMDInitCH395(void)
{
    uint8_t i = 0;
    uint8_t s = 0;

    xCH395CmdStart();
		xCH395_SPI_DMA_WRITE_Byte(CMD0W_INIT_CH395);
		xEndCH395Cmd();
    while(1)
    {
        HAL_Delay(10);                                                 /* 延时查询，建议2MS以上*/
        s = CH395GetCmdStatus();                                     /* 不能过于频繁查询*/
        if(s !=CH395_ERR_BUSY)break;                                 /* 如果CH395芯片返回忙状态*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* 超时退出,本函数需要500MS以上执行完毕 */
    }
    return s;
}

/*******************************************************************************
* Function Name  : CH395OpenSocket
* Description    : 打开socket，此命令需要等待执行成功
* Input          : sockindex Socket索引
* Output         : None
* Return         : 返回执行结果
*******************************************************************************/
uint8_t  CH395OpenSocket(uint8_t sockindex)
{
    uint8_t i = 0;
    uint8_t s = 0;
		
		uint8_t data[2]={0};
		data[0] = CMD1W_OPEN_SOCKET_SN;
		data[1] = sockindex;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,2);
		xEndCH395Cmd();
		
    while(1)
    {
        HAL_Delay(5);                                                 /* 延时查询，建议2MS以上*/
        s = CH395GetCmdStatus();                                     /* 不能过于频繁查询*/
        if(s !=CH395_ERR_BUSY)break;                                 /* 如果CH395芯片返回忙状态*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* 超时退出*/
    }
    return s;
}

/*******************************************************************************
* Function Name  : CH395CloseSocket
* Description    : 关闭socket，
* Input          : sockindex Socket索引
* Output         : None
* Return         : 返回执行结果
*******************************************************************************/
uint8_t  CH395CloseSocket(uint8_t sockindex)
{
    uint8_t i = 0;
    uint8_t s = 0;
		uint8_t data[2]={0};
		data[0] = CMD1W_CLOSE_SOCKET_SN;
		data[1] = sockindex;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,2);
		xEndCH395Cmd();
    while(1)
    {
        HAL_Delay(5);                                                 /* 延时查询，建议2MS以上*/
        s = CH395GetCmdStatus();                                     /* 不能过于频繁查询*/
        if(s !=CH395_ERR_BUSY)break;                                 /* 如果CH395芯片返回忙状态*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* 超时退出*/
    }
    return s;
}

/********************************************************************************
* Function Name  : CH395TCPConnect
* Description    : TCP连接，仅在TCP模式下有效，此命令需要等待执行成功
* Input          : sockindex Socket索引
* Output         : None
* Return         : 返回执行结果
*******************************************************************************/
uint8_t CH395TCPConnect(uint8_t sockindex)
{
    uint8_t i = 0;
    uint8_t s = 0;
		uint8_t data[2]={0};
		data[0] = CMD1W_TCP_CONNECT_SN;
		data[1] = sockindex;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,2);
		xEndCH395Cmd();
    while(1)
    {
        HAL_Delay(5);                                                 /* 延时查询，建议2MS以上*/
        s = CH395GetCmdStatus();                                     /* 不能过于频繁查询*/
        if(s !=CH395_ERR_BUSY)break;                                 /* 如果CH395芯片返回忙状态*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* 超时退出*/
    }
    return s;
}

/******************************************************************************
* Function Name  : CH395TCPListen
* Description    : TCP监听，仅在TCP模式下有效，此命令需要等待执行成功
* Input          : sockindex Socket索引
* Output         : None
* Return         : 返回执行结果
*******************************************************************************/
uint8_t CH395TCPListen(uint8_t sockindex)
{
    uint8_t i = 0;
    uint8_t s = 0;
		uint8_t data[2]={0};
		data[0] = CMD1W_TCP_LISTEN_SN;
		data[1] = sockindex;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,2);
		xEndCH395Cmd();
    while(1)
    {
        HAL_Delay(5);                                                 /* 延时查询，建议2MS以上*/
        s = CH395GetCmdStatus();                                     /* 不能过于频繁查询*/
        if(s !=CH395_ERR_BUSY)break;                                 /* 如果CH395芯片返回忙状态*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* 超时退出*/
    }
    return s;
}

/********************************************************************************
* Function Name  : CH395TCPDisconnect
* Description    : TCP断开，仅在TCP模式下有效，此命令需要等待执行成功
* Input          : sockindex Socket索引
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t CH395TCPDisconnect(uint8_t sockindex)
{
    uint8_t i = 0;
    uint8_t s = 0;
		uint8_t data[2]={0};
		data[0] = CMD1W_TCP_DISNCONNECT_SN;
		data[1] = sockindex;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,2);
		xEndCH395Cmd();
    while(1)
    {
        HAL_Delay(5);                                                 /* 延时查询，建议2MS以上*/
        s = CH395GetCmdStatus();                                     /* 不能过于频繁查询*/
        if(s !=CH395_ERR_BUSY)break;                                 /* 如果CH395芯片返回忙状态*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* 超时退出*/
    }
    return s;
}

/*******************************************************************************
* Function Name  : CH395GetSocketInt
* Description    : 获取socket n的中断状态
* Input          : sockindex   socket索引
* Output         : None
* Return         : 中断状态
*******************************************************************************/
uint8_t CH395GetSocketInt(uint8_t sockindex)
{
    uint8_t intstatus;
		uint8_t data[2]={0};
		data[0] = CMD11_GET_INT_STATUS_SN;
		data[1] = sockindex;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,2);
		intstatus = xCH395_SPI_DMA_READ_Byte();
		xEndCH395Cmd();
    return intstatus;
}

/*******************************************************************************
* Function Name  : CH395CRCRet6Bit
* Description    : 对多播地址进行CRC运算，并取高6位。
* Input          : mac_addr   MAC地址
* Output         : None
* Return         : 返回CRC32的高6位
*******************************************************************************/
uint8_t CH395CRCRet6Bit(uint8_t *mac_addr)
{
    int32_t perByte;
    int32_t perBit;
    const uint32_t poly = 0x04C11DB7;
    uint32_t crc_value = 0xFFFFFFFF;
    uint8_t c;
    for ( perByte = 0; perByte < 6; perByte ++ ) 
    {
        c = *(mac_addr++);
        for ( perBit = 0; perBit < 8; perBit++ ) 
        {
            crc_value = (crc_value<<1)^((((crc_value>>31)^c)&0x01)?poly:0);
            c >>= 1;
        }
    }
    crc_value=crc_value>>26;                                      
    return ((uint8_t)crc_value);
}

/******************************************************************************
* Function Name  : CH395DHCPEnable
* Description    : 启动/停止DHCP
* Input          : flag   1:启动DHCP;0：停止DHCP
* Output         : None
* Return         : 执行状态
*******************************************************************************/
uint8_t  CH395DHCPEnable(uint8_t flag)
{
    uint8_t i = 0;
    uint8_t s;
		uint8_t data[2]={0};
		data[0] = CMD10_DHCP_ENABLE;
		data[1] = flag;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,2);
		xEndCH395Cmd();
    while(1)
    {
        HAL_Delay(20);
        s = CH395GetCmdStatus();                                     /* 不能过于频繁查询*/
        if(s !=CH395_ERR_BUSY)break;                                 /* 如果CH395芯片返回忙状态*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* 超时退出*/
    }
    return s;
}

/******************************************************************************
* Function Name  : CH395GetDHCPStatus
* Description    : 获取DHCP状态
* Input          : None
* Output         : None
* Return         : DHCP状态，0为成功，其他值表示错误
*******************************************************************************/
uint8_t CH395GetDHCPStatus(void)
{
    uint8_t status;
	
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE_Byte(CMD01_GET_DHCP_STATUS);
		status = xCH395_SPI_DMA_READ_Byte();
		xEndCH395Cmd();
    return status;
}

/*******************************************************************************
* Function Name  : CH395GetIPInf
* Description    : 获取IP，子网掩码和网关地址
* Input          : None
* Output         : 12个字节的IP,子网掩码和网关地址
* Return         : None
*******************************************************************************/
void CH395GetIPInf(uint8_t *addr)
{
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE_Byte(CMD014_GET_IP_INF);
		xCH395_SPI_DMA_READ(addr,20);
		xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395WriteGPIOAddr
* Description    : 写GPIO寄存器
* Input          : regadd   寄存器地址
*                ：regval   寄存器值
* Output         : None
* Return         : None
*******************************************************************************/
void CH395WriteGPIOAddr(uint8_t regadd,uint8_t regval)
{
		uint8_t data[3]={0};
		data[0] = CMD20_WRITE_GPIO_REG;
		data[1] = regadd;
		data[2] = regval;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,3);
		xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395ReadGPIOAddr
* Description    : 读GPIO寄存器
* Input          : regadd   寄存器地址
* Output         : None
* Return         : 寄存器的值
*******************************************************************************/
uint8_t CH395ReadGPIOAddr(uint8_t regadd)
{
    uint8_t i;
		uint8_t data[2]={0};
		data[0] = CMD10_READ_GPIO_REG;
		data[1] = regadd;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,2);
		i = xCH395_SPI_DMA_READ_Byte();
		xEndCH395Cmd();
    return i;
}

/*******************************************************************************
* Function Name  : CH395EEPROMErase
* Description    : 擦除EEPROM
* Input          : None
* Output         : None
* Return         : 执行状态
*******************************************************************************/
uint8_t CH395EEPROMErase(void)
{
    uint8_t i;    
		xCH395CmdStart();
    xCH395_SPI_DMA_WRITE_Byte(CMD00_EEPROM_ERASE);
		xEndCH395Cmd();
    while(1)
    {
        HAL_Delay(20);
       i = CH395GetCmdStatus();
       if(i == CH395_ERR_BUSY)continue;
       break;
    }
    return i;
}

/*******************************************************************************
* Function Name  : CH395EEPROMWrite
* Description    : 写EEPROM
* Input          : eepaddr  EEPROM地址
*                ：buf      缓冲区地址
*                ：len      长度
* Output         : None
* Return         : 执行状态
*******************************************************************************/
uint8_t CH395EEPROMWrite(uint16_t eepaddr,uint8_t *buf,uint8_t len)
{
    uint8_t i;
		uint8_t data[4]={0};
		data[0] = CMD30_EEPROM_WRITE;
		data[1] = (uint8_t)eepaddr;
		data[2] = (uint8_t)(eepaddr >> 8);
		data[3] = len;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,4);
		xCH395_SPI_DMA_WRITE(buf,len);
		xEndCH395Cmd();
		
    while(1)
    {
       HAL_Delay(20);
       i = CH395GetCmdStatus();
       if(i == CH395_ERR_BUSY)continue;
       break;
    }
    return i;
}
  
/*******************************************************************************
* Function Name  : CH395EEPROMRead
* Description    : 写EEPROM
* Input          : eepaddr  EEPROM地址
*                ：buf      缓冲区地址
*                ：len      长度
* Output         : None
* Return         : None
*******************************************************************************/
void CH395EEPROMRead(uint16_t eepaddr,uint8_t *buf,uint8_t len)
{
		uint8_t data[4]={0};
		data[0] = CMD30_EEPROM_READ;
		data[1] = (uint8_t)eepaddr;
		data[2] = (uint8_t)(eepaddr >> 8);
		data[3] = len;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,4);
		xCH395_SPI_DMA_READ(buf,len);
		xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395SetTCPMss
* Description    : 设置TCP MSS值
* Input          : tcpmss 
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetTCPMss(uint16_t tcpmss)
{
		uint8_t data[3]={0};
		data[0] = CMD20_SET_TCP_MSS;
		data[1] = (uint8_t)tcpmss;
		data[2] = (uint8_t)(tcpmss >> 8);
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,3);
		xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395SetSocketRecvBuf
* Description    : 设置Socket接收缓冲区
* Input          : sockindex  socket索引
                 ：startblk   起始地址
                 ：blknum     单位缓冲区个数 ，单位为512字节
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketRecvBuf(uint8_t sockindex,uint8_t startblk,uint8_t blknum)
{
		uint8_t data[4]={0};
		data[0] = CMD30_SET_RECV_BUF;
		data[1] = sockindex;
		data[2] = startblk;
		data[3] = blknum;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,4);
		xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395SetSocketSendBuf
* Description    : 设置Socket发送缓冲区
* Input          : sockindex  socket索引
                 ：startblk   起始地址
                 ：blknum     单位缓冲区个数
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketSendBuf(uint8_t sockindex,uint8_t startblk,uint8_t blknum)
{
		uint8_t data[4]={0};
		data[0] = CMD30_SET_SEND_BUF;
		data[1] = sockindex;
		data[2] = startblk;
		data[3] = blknum;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,4);
		xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395UDPSendTo
* Description    : UDP向指定的IP和端口发送数据
* Input          : buf     : 发送数据缓冲区
                   len     : 发送数据长度
				   ip      : 目标IP
				   port    : 目标端口
				   sockeid : socket索引值
* Output         : None
* Return         : None
*******************************************************************************/
void CH395UDPSendTo(uint8_t *buf,uint32_t len,uint8_t *ip,uint16_t port,uint8_t sockindex)
{
    CH395SetSocketDesIP(sockindex,ip);                            /* 设置socket 0目标IP地址 */         
    CH395SetSocketDesPort(sockindex,port);
    CH395SendData(sockindex,buf,len);    
}

/*******************************************************************************
* Function Name  : CH395SetStartPara
* Description    : 设置CH395启动参数
* Input          : mdata
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetStartPara(uint32_t mdata)
{
		uint8_t data[5]={0};
		data[0] = CMD40_SET_FUN_PARA;
		data[1] = (uint8_t)mdata;
		data[2] = (uint8_t)(mdata>>8);
		data[3] = (uint8_t)(mdata>>16);
		data[4] = (uint8_t)(mdata>>24);
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,5);
		xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395CMDGetGlobIntStatus
* Description    : 获取全局中断状态，收到此命令CH395自动取消中断,0x44及以上版本使用
* Input          : None
* Output         : None
* Return         : 返回当前的全局中断状态
*******************************************************************************/
uint16_t CH395CMDGetGlobIntStatus_ALL(void)
{
		uint16_t init_status;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE_Byte(CMD02_GET_GLOB_INT_STATUS_ALL);
		xCH395_SPI_DMA_READ((uint8_t*)&init_status,2);
		xEndCH395Cmd();
		return 	init_status;
}

/*******************************************************************************
* Function Name  : CH395SetKeepLive
* Description    : 设置keepalive功能
* Input          : sockindex Socket号
*                  cmd 0：关闭 1：开启
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetKeepLive(uint8_t sockindex,uint8_t cmd)
{
		uint8_t data[3]={0};
		data[0] = CMD20_SET_KEEP_LIVE_SN;
		data[1] = sockindex;
		data[2] = cmd;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,3);
		xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395KeepLiveCNT
* Description    : 设置keepalive重试次数
* Input          : cnt 重试次数（）
* Output         : None
* Return         : None
*******************************************************************************/
void CH395KeepLiveCNT(uint8_t cnt)
{
		uint8_t data[2]={0};
		data[0] = CMD10_SET_KEEP_LIVE_CNT;
		data[1] = cnt;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,2);
		xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395KeepLiveIDLE
* Description    : 设置KEEPLIVE空闲
* Input          : idle 空闲时间（单位：ms）
* Output         : None
* Return         : None
*******************************************************************************/
void CH395KeepLiveIDLE(uint32_t idle)
{
		uint8_t data[5]={0};
		data[0] = CMD40_SET_KEEP_LIVE_IDLE;
		data[1] = (uint8_t)idle;
		data[2] = (uint8_t)(idle>>8);
		data[3] = (uint8_t)(idle>>16);
		data[4] = (uint8_t)(idle>>24);
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,5);
		xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395KeepLiveINTVL
* Description    : 设置KeepLive间隔时间 
* Input          : intvl 间隔时间（单位：ms）
* Output         : None
* Return         : None
*******************************************************************************/
void CH395KeepLiveINTVL(uint32_t intvl)
{
		uint8_t data[5]={0};
		data[0] = CMD40_SET_KEEP_LIVE_INTVL;
		data[1] = (uint8_t)intvl;
		data[2] = (uint8_t)(intvl>>8);
		data[3] = (uint8_t)(intvl>>16);
		data[4] = (uint8_t)(intvl>>24);
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,5);
		xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395SetTTLNum
* Description    : 设置TTL
* Input          : sockindex Socket号
*                  TTLnum:TTL数
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetTTLNum(uint8_t sockindex,uint8_t TTLnum)
{
		uint8_t data[3]={0};
		data[0] = CMD20_SET_TTL;
		data[1] = sockindex;
		data[2] = TTLnum;
		
		xCH395CmdStart();
		xCH395_SPI_DMA_WRITE(data,3);
		xEndCH395Cmd();
}

/**************************** endfile *************************************/
